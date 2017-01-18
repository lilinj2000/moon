// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_STATE_HH
#define MOON_STATE_HH

#include "Tick.hh"

namespace moon {

class Context;

typedef enum {
  STATE_SHORT_POSITION_INIT = 10,
  STATE_SHORT_POSITION_OPEN,
  STATE_POSITION_INIT = 100,
  STATE_POSITION_CLOSE
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

  virtual void handleMDInfo(const MDInfo&, const MDInfo&) = 0;

  static State* createState(StateID id, Context* context);

 private:
  Context* context_;
};

class ShortPositionState : public State {
 public:
  ShortPositionState(Context* context):
      State(context) {
  }

  virtual ~ShortPositionState() {
  }
};

class ShortPositionInitState : public ShortPositionState {
 public:
  ShortPositionInitState(Context* context):
      ShortPositionState(context) {
  }

  virtual ~ShortPositionInitState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);
};

class ShortPositionOpenState : public ShortPositionState {
 public:
  ShortPositionOpenState(Context* context):
      ShortPositionState(context) {
  }

  virtual ~ShortPositionOpenState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);
};

class PositionState : public State {
 public:
  PositionState(Context* context):
      State(context) {
  }

  virtual ~PositionState() {
  }
};

class PositionInitState : public PositionState {
 public:
  PositionInitState(Context* context):
      PositionState(context) {
  }

  virtual ~PositionInitState() {
  }
  
  virtual void handleMDInfo(const MDInfo&, const MDInfo&);
};

class PositionCloseState : public PositionState {
 public:
  PositionCloseState(Context* context):
      PositionState(context) {
  }

  virtual ~PositionCloseState() {
  }

  virtual void handleMDInfo(const MDInfo&, const MDInfo&);
};

};  // namespace moon

#endif
