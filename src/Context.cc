// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Context.hh"

namespace moon {

Context::Context(Server* server):
    server_(server) {
  MOON_TRACE <<"Context::Context()";

  state_id_ = STATE_SHORT_POSITION_INIT;
  // regist the state
  states_[STATE_SHORT_POSITION_INIT] = State::createState(STATE_SHORT_POSITION_INIT, this);
  states_[STATE_SHORT_POSITION_OPEN] = State::createState(STATE_SHORT_POSITION_OPEN, this);
  states_[STATE_POSITION_INIT] = State::createState(STATE_SHORT_POSITION_OPEN, this);
  states_[STATE_POSITION_CLOSE] = State::createState(STATE_SHORT_POSITION_OPEN, this);
}

void Context::handleMDInfo(const MDInfo& md_instru1,
                           const MDInfo& md_instru2) {
  MOON_TRACE <<"Context::handleMDInfo()";

  std::unique_lock<std::mutex> lck(state_mutex_);
  states_[state_id_]->handleMDInfo(md_instru1, md_instru2);
}

};  // namespace moon
