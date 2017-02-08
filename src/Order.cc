// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Order.hh"
#include "Server.hh"
#include "json/json.hh"

namespace moon {

Order::Order(Server* server) :
    server_(server) {
  MOON_TRACE <<"Order::Order()";

  req_order_queue_.reset(new soil::MsgQueue<std::string, Order>(this));

  req_order_service_.reset(zod::PushService::create(
      server_->config()->options()->push_addr));

  std::string filter = server_->config()->options()->instru1;
  filter += "|" + server_->config()->options()->instru2;
  subject::Options order_options {
    filter,
        server_->config()->options()->order_sub_addr
        };
  rsp_order_service_.reset(
      subject::Service::createService(
          order_options, this));
}

Order::~Order() {
  MOON_TRACE <<"Order::~Order()";
}

void Order::instruOpen(const std::string& instru,
                       const std::string& direct,
                       double price,
                       int volume) {
  MOON_TRACE <<"Order::instruOpen()";

  std::string offset_flag = "0";
  std::string msg = buildOrderMsg(offset_flag,
                                  instru, direct,
                                  price, volume);
  pushReqOrderMsg(new std::string(msg));

  inputOrder(instru, direct, price, volume);
}

void Order::instruClose(const std::string& instru,
                       const std::string& direct,
                       double price,
                       int volume) {
  MOON_TRACE <<"Order::instruClose()";

  std::string offset_flag = "3";
  std::string msg = buildOrderMsg(offset_flag,
                                  instru, direct,
                                  price, volume);
  pushReqOrderMsg(new std::string(msg));

  inputOrder(instru, direct, price, volume);
}

std::string Order::buildOrderMsg(
    const std::string& offset_flag,
    const std::string& instru,
    const std::string& direct,
    double price,
    int volume) {
  MOON_TRACE <<"Order::buildOrderMsg()";

  json::Document doc;

  json::Value v_order;
  json::addMember<const std::string&>(&v_order, "direct", direct, &doc);
  json::addMember<const std::string&>(&v_order,
                                      "offset_flag",
                                      offset_flag, &doc);
  json::addMember<const std::string&>(&v_order,
                                      "hedge_flag",
                                      "1", &doc);
  json::addMember<const std::string&>(&v_order,
                                      "instru",
                                      instru, &doc);
  json::addMember<const std::string&>(&v_order,
                                      "price",
                                      std::to_string(price), &doc);
  json::addMember<const std::string&>(&v_order,
                                      "volume",
                                      std::to_string(volume), &doc);

  json::addMember<const json::Value&>(&doc, "order", v_order);

  return json::toString(doc);
}

void Order::inputOrder(const std::string& instru,
                       const std::string& direct,
                       double price,
                       int volume) {
  MOON_TRACE <<"Order::inputOrder()";

  OrderInfo record {
    instru, direct, price, volume,
        "", "", "", ""
  };
  orders_[instru] = record;
}

int Order::updateOrder(const OrderInfo& order) {
  MOON_TRACE <<"Order::updateOrder()";

  auto i_iter = orders_.find(order.instru);
  if (i_iter != orders_.end()) {
    i_iter->second.order_local_id = order.order_local_id;
    i_iter->second.order_sys_id = order.order_sys_id;
    i_iter->second.order_status = order.order_status;
    i_iter->second.status_msg = order.status_msg;
  } else {
    MOON_ERROR <<"unexpected order - " <<order.instru;
  }

  if (order.order_status == "0") {
    // the order all traded
    orders_.erase(i_iter);
  }

  return state();
}

int Order::updatePosition(const TradeInfo& trade) {
  MOON_TRACE <<"Order::updatePosition()";

  auto i_iter = positions_.find(trade.instru);
  if (i_iter != positions_.end()) {
    if (trade.offset_flag == "3"
        || trade.offset_flag == "1") {  // close
      positions_.erase(i_iter);
    } else {
      MOON_ERROR <<"unexpected trade - " <<trade.instru;
    }
  } else {
    if (trade.offset_flag == "0")  {  // open
      positions_[trade.instru] = trade;
    }
  }

  return state();
}

int Order::state() {
  MOON_TRACE <<"Order::state()";

  bool short_position = positions_.empty();
  bool without_order = orders_.empty();

  if (short_position && without_order) {
    return 1;
  } else if (short_position && !without_order) {
    return 2;
  } else if (!short_position && !without_order) {
    return 3;
  } else {
    return 4;
  }
}

double Order::calProfit(const MDInfo& md_instru1,
                     const MDInfo& md_instru2) {
  MOON_TRACE <<"Order::calProfit()";

  double volume_multiple = server_->config()->options()->volume_multiple;
  double fee_per_volume = server_->config()->options()->fee_per_volume;
  double fee_per_money = server_->config()->options()->fee_per_money;

  const std::string buy = "0";
  const std::string sell = "1";

  double profit = 0.0;
  for (const auto& position : positions_) {
    double last_price = 0;

    if (position.second.direct == buy) {
      last_price = md_instru1.bid_price1;
      if (position.first == md_instru2.instru) {
        last_price = md_instru2.bid_price1;
      }
      
      profit += (last_price - position.second.price)
          * position.second.volume
          * volume_multiple;

    } else if (position.second.direct == sell) {
      last_price = md_instru1.ask_price1;
      if (position.first == md_instru2.instru) {
        last_price = md_instru2.ask_price1;
      }

      profit += (position.second.price - last_price)
          * position.second.volume
          * volume_multiple;
    }

    // calculate fee
    if (fee_per_volume > 0) {
      profit -= position.second.volume
          * fee_per_volume * 2;
    } else if (fee_per_money > 0) {
      profit -= (last_price + position.second.price)
          * position.second.volume
          * volume_multiple * fee_per_money;
    }
  }

  return profit;
}

// req order
void Order::msgCallback(const std::string* msg) {
  MOON_TRACE <<"Order::msgCallback()";

  req_order_service_->sendMsg(*msg);
}

// rtn order && trade
void Order::onMessage(const std::string& msg) {
  MOON_TRACE <<"Order::onMessage()";

  MOON_DEBUG <<msg;

  json::Document doc;
  json::fromString(msg, &doc);

  auto itr = doc.MemberBegin();
  std::string key = itr->name.GetString();
  json::Value& data = doc[key.data()];

  // OnRtnOrder
  if (key == "OnRtnOrder") {
    auto itr = data.FindMember("CThostFtdcOrderField");
    if (itr != data.MemberEnd()) {
      std::string instru = (itr->value)["InstrumentID"].GetString();
      std::string order_local_id = (itr->value)["OrderLocalID"].GetString();
      std::string order_sys_id = (itr->value)["OrderSysID"].GetString();
      std::string order_status = (itr->value)["OrderStatus"].GetString();
      std::string status_msg = (itr->value)["StatusMsg"].GetString();

      OrderInfo order {
        instru, "", 0, 0,
            order_local_id, order_sys_id,
            order_status, status_msg
            };
      server_->context()->handleOrderInfo(order);
    }
  } else if (key == "OnRtnTrade") {
    auto itr = data.FindMember("CThostFtdcTradeField");
    if (itr != data.MemberEnd()) {
      std::string instru = (itr->value)["InstrumentID"].GetString();
      std::string direct = (itr->value)["Direction"].GetString();
      double price = std::stod((itr->value)["Price"].GetString());
      int volume = std::stoi((itr->value)["Volume"].GetString());
      std::string offset_flag = (itr->value)["OffsetFlag"].GetString();
      std::string trade_id = (itr->value)["TradeID"].GetString();
      std::string order_sys_id = (itr->value)["OrderSysID"].GetString();
      std::string order_local_id = (itr->value)["OrderLocalID"].GetString();

      TradeInfo trade {
        instru, direct, price, volume,
            offset_flag, trade_id,
            order_sys_id, order_local_id
            };
      server_->context()->handleTradeInfo(trade);
    }
  }
}


}  // namespace moon
