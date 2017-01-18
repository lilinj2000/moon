// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_SERVER_HH
#define MOON_SERVER_HH

#include <memory>
#include "Config.hh"
#include "Tick.hh"
#include "Context.hh"
#include "subject/Service.hh"
#include "zod/PushService.hh"

namespace moon {

class Server {
 public:
  Server(int argc, char* argv[]);

  ~Server();

  Context* context() {
    return context_.get();
  }

  Config* config() {
    return config_.get();
  }

 private:
  std::unique_ptr<Config> config_;

  class MDServiceCallback : public subject::ServiceCallback {
   public:
    MDServiceCallback(Server* server):
        server_(server) {
    }

    virtual void onMessage(const std::string& msg);

   private:
    Server* server_;
  };
  friend class MDServiceCallback;
  std::unique_ptr<MDServiceCallback> md_callback_;
  std::unique_ptr<subject::Service> md_service_;

  class TradeServiceCallback : public subject::ServiceCallback {
   public:
    TradeServiceCallback(Server* server):
        server_(server) {
    }

    virtual void onMessage(const std::string& msg);

   private:
    Server* server_;
  };
  friend class TradeServiceCallback;
  std::unique_ptr<TradeServiceCallback> trade_callback_;
  std::unique_ptr<subject::Service> trade_service_;

  std::unique_ptr<zod::PushService> push_service_;

  std::unique_ptr<Tick> tick_;

  std::unique_ptr<Context> context_;
};

};  // namespace moon

#endif
