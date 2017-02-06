// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_CONTEXT_HH
#define MOON_CONTEXT_HH

#include <map>
#include <mutex>
#include "State.hh"

namespace moon {

class Server;

class Context {
 public:
  explicit Context(Server* server);

  ~Context() {
  }

  Server* server() {
    return server_;
  }

  void handleMDInfo(const MDInfo&, const MDInfo&);

  void handleOrderInfo(const OrderInfo&);

  void handleTradeInfo(const TradeInfo&);

  void setStateID(StateID state_id);

 protected:
  void registerState(StateID);

 private:
  Server* server_;

  friend class State;

  std::mutex state_mutex_;
  StateID state_id_;
  std::map<StateID, State*> states_;
};

};  // namespace moon

#endif
