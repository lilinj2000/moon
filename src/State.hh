// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_STATE_HH
#define MOON_STATE_HH

#include "Tick.hh"
#include "Order.hh"

namespace moon {

class Context;

typedef enum {
  STATE_SHORT_POSITION_WITHOUT_ORDER = 10,
  STATE_SHORT_POSITION_WITH_ORDER,
  STATE_POSITION_WITH_ORDER = 100,
  STATE_POSITION_WITHOUT_ORDER
}StateID;

class State {
 public:
  State(Context* context):
      context_(context) {
  }

  virtual ~State() {
  }

  Context* context() {
    return context_;
  }

  virtual void toNextState(StateID);

  virtual void handleMDInfo(const MDInfo&, const MDInfo&) = 0;

  virtual void handleOrderInfo(const OrderInfo&) = 0;

  virtual void handleTradeInfo(const TradeInfo&) = 0;

  static State* createState(StateID id, Context* context);

 private:
  Context* context_;
};

class ShortPositionWithoutOrderState : public State {
 public:
  ShortPositionWithoutOrderState(Context* context):
      State(context) {
  }

  virtual ~ShortPositionWithoutOrderState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);

  virtual void handleOrderInfo(const OrderInfo&);
};

class ShortPositionWithOrderState : public State {
 public:
  ShortPositionWithOrderState(Context* context):
      State(context) {
  }

  virtual ~ShortPositionWithOrderState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);

  virtual void handleOrderInfo(const OrderInfo&);
};

class PositionWithOrderState : public State {
 public:
  PositionWithOrderState(Context* context):
      State(context) {
  }

  virtual ~PositionWithOrderState() {
  }
  
  virtual void handleMDInfo(const MDInfo&, const MDInfo&);

  virtual void handleOrderInfo(const OrderInfo&);
};

class PositionWithoutOrderState : public State {
 public:
  PositionWithoutOrderState(Context* context):
      State(context) {
  }

  virtual ~PositionWithoutOrderState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);

  virtual void handleOrderInfo(const OrderInfo&);
};

};  // namespace moon

#endif
