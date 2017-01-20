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

void Context::basisEvent(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"Context::basisEvent()";

  double long_basis = md_instru2.bid_price1
      - md_instru1.ask_price1;

  double short_basis = md_instru2.ask_price1
      - md_instru1.bid_price1;

  int queue_size = server_->config()->options()->basis_queue_size;

  double delta_open = server_->config()->options()->basis_delta_open;

  if (triggerOpen(long_basis, long_basis_queue_,
                  queue_size, delta_open)) {
    // buy instru1, sell instru2
  } else if (triggerOpen(short_basis,
                         short_basis_queue_,
                         queue_size, delta_open)) {
    // sell instru1, buy instru2
  }

  pushBasis(md_instru1, md_instru2);
}

bool Context::triggerOpen(double basis,
                          const BasisQueue& basis_queue,
                          int queue_size,
                          double delta) {
  MOON_TRACE <<"Context::triggerOpen()";

  if (basis_queue.size() >= queue_size) {
    double ma = std::accumulate(basis_queue.begin(),
                                basis_queue.end(),
                                0) / basis_queue.size();

    if (std::fabs(basis - ma) >= delta) {
      return true;
    }
  }

  return false;
}

void Context::pushBasis(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"Context::pushBasis()";

  double long_basis = md_instru2.bid_price1
      - md_instru1.ask_price1;

  double short_basis = md_instru2.ask_price1
      - md_instru1.bid_price1;

  int basis_queue_size = server_->config()->options()->basis_queue_size;

  while (long_basis_queue_.size() >= basis_queue_size) {
    long_basis_queue_.pop_front();
  }

  while (short_basis_queue_.size() >= basis_queue_size) {
    short_basis_queue_.pop_front();
  }
  
  long_basis_queue_.push_back(long_basis);

  short_basis_queue_.push_back(short_basis);
}

};  // namespace moon
