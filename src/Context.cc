// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Context.hh"
#include "Server.hh"

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

void Context::basisEvent(double long_basis, double short_basis) {
  MOON_TRACE <<"Context::basisEvent()";

  int basis_queue_size = server_->config()->options()->basis_queue_size;

  if (long_basis_queue_.size() >= basis_queue_size) {
    // double ma = std::accumulate(long_basis_queue_.begin(),
    //                             long_basis_queue_.end(),
    //                             0) / long_basis_queue_.size();
    // double basis_delta_open = server_->config()->options()->basis_delta_open;
    // if (std::fabs(long_basis - ma)) >= basis_delta_open) {
    // // just do open for long basis
    // return;
  }
}

void Context::pushBasis(double long_basis, double short_basis) {
  MOON_TRACE <<"Context::pushBasis()";

  int basis_queue_size = server_->config()->options()->basis_queue_size;

  while (long_basis_queue_.size() >= basis_queue_size) {
    long_basis_queue_.pop();
  }

  while (short_basis_queue_.size() >= basis_queue_size) {
    short_basis_queue_.pop();
  }
  
  long_basis_queue_.push(long_basis);

  short_basis_queue_.push(short_basis);
}

};  // namespace moon
