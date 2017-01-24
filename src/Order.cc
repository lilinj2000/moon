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
  rsp_order_service_.reset(subject::Service::createService(order_options, this));
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

  updateOrders(instru, direct, price, volume);
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

  updateOrders(instru, direct, price, volume);
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
  json::addMember<const std::string&>(&v_order, "offset_flag", offset_flag, &doc);
  json::addMember<const std::string&>(&v_order, "hedge_flag", "1", &doc);
  json::addMember<const std::string&>(&v_order, "instru", instru, &doc);
  json::addMember<const std::string&>(&v_order, "price", std::to_string(price), &doc);
  json::addMember<const std::string&>(&v_order, "volume", std::to_string(volume), &doc);

  json::addMember<const json::Value&>(&doc, "order", v_order);

  return json::toString(doc);
}

void Order::updateOrders(const std::string& instru,
                         const std::string& direct,
                         double price,
                         int volume) {
  MOON_TRACE <<"Order::updateOrders()";
  
  OrderInfo record {
    instru, direct, price, volume, 0.0, 0
  };
  orders_[instru] = record;
}

// req order
void Order::msgCallback(const std::string* msg) {
  MOON_TRACE <<"Order::msgCallback()";

  req_order_service_->sendMsg(*msg);
}

// rsp order
void Order::onMessage(const std::string& msg) {
  MOON_TRACE <<"Order::onMessage()";

}


}  // namespace moon
