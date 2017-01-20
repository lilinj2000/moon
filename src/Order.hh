// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_ORDER_HH
#define MOON_ORDER_HH

#include "soil/MsgQueue.hh"
#include "subject/Service.hh"
#include "zod/PushService.hh"

namespace moon {

class Server;

class Order {
 public:
  Order(Server* server);

  ~Order();

  void instruOpen(const std::string& instru,
                  const std::string& direct,
                  double price,
                  int volume);

  void instruClose(const std::string& instru,
                   const std::string& direct,
                   double price,
                   int volume);
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

 private:
  Server* server_;

  class ReqOrderServiceCallback {
   public:
    ReqOrderServiceCallback(Order* order):
        order_(order) {
    }

    void msgCallback(const std::string *);

   private:
    Order* order_;
  };
  friend class ReqOrderServiceCallback;
  std::unique_ptr<ReqOrderServiceCallback> req_order_callback_;
  std::unique_ptr<zod::PushService> req_order_service_;
  std::unique_ptr<soil::MsgQueue<std::string, ReqOrderServiceCallback> > req_order_queue_;

  class RspOrderServiceCallback : public subject::ServiceCallback {
   public:
    RspOrderServiceCallback(Order* order):
        order_(order) {
    }

    virtual void onMessage(const std::string& msg);

   private:
    Order* order_;
  };
  friend class RspOrderServiceCallback;
  std::unique_ptr<RspOrderServiceCallback> rsp_order_callback_;
  std::unique_ptr<subject::Service> rsp_order_service_;
  std::unique_ptr<soil::MsgQueue<std::string, RspOrderServiceCallback> > rsp_order_queue_;

};

}  // namespace moon

#endif
