// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_ORDER_HH
#define MOON_ORDER_HH

#include <map>
#include "soil/MsgQueue.hh"
#include "subject/Service.hh"
#include "zod/PushService.hh"

namespace moon {

class Server;

typedef struct {
  std::string instru;
  std::string direct;
  double price;
  int volume;
  std::string order_local_id;
  std::string order_sys_id;
  std::string order_status;
  std::string status_msg;
}OrderInfo;

typedef struct {
  std::string instru;
  std::string direct;
  double price;
  int volume;
}PositionInfo;

typedef std::map<std::string, OrderInfo> OrdersType;
typedef std::map<std::string, PositionInfo> PositionsType;

class Order : public subject::ServiceCallback {
 public:
  Order(Server* server);

  ~Order();

  // rsp order
  virtual void onMessage(const std::string& msg);

  // req order
  void msgCallback(const std::string *);

  void instruOpen(const std::string& instru,
                  const std::string& direct,
                  double price,
                  int volume);

  void instruClose(const std::string& instru,
                   const std::string& direct,
                   double price,
                   int volume);

  bool updateOrder(const OrderInfo& order);

protected:
  void pushReqOrderMsg(std::string* msg) {
    req_order_queue_->pushMsg(msg);
  }

  std::string buildOrderMsg(
      const std::string& offset_flag,
      const std::string& instru,
      const std::string& direct,
      double price,
      int volume);

  void inputOrder(const std::string& instru,
                 const std::string& direct,
                 double price,
                 int volume);

 private:
  Server* server_;

  std::unique_ptr<soil::MsgQueue<std::string, Order> > req_order_queue_;
  std::unique_ptr<zod::PushService> req_order_service_;

  std::unique_ptr<subject::Service> rsp_order_service_;

  OrdersType orders_;

  PositionsType positions_;
};

}  // namespace moon

#endif
