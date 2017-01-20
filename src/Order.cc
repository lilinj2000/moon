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

  req_order_callback_.reset(new ReqOrderServiceCallback(this));
  req_order_queue_.reset(new soil::MsgQueue<std::string, ReqOrderServiceCallback>(req_order_callback_.get()));

  req_order_service_.reset(zod::PushService::create(
      server_->config()->options()->push_addr));

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

void Order::ReqOrderServiceCallback::msgCallback(const std::string* msg) {
  MOON_TRACE <<"Order::ReqOrderServiceCallback::msgCallback()";

  order_->req_order_service_->sendMsg(*msg);
}


}  // namespace moon
