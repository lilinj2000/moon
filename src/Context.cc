// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Context.hh"

namespace moon {

Context::Context(Server* server):
    server_(server) {
  MOON_TRACE <<"Context::Context()";

  state_id_ = STATE_SHORT_POSITION_WITHOUT_ORDER;

  // register the state
  registerState(STATE_SHORT_POSITION_WITHOUT_ORDER);
  registerState(STATE_SHORT_POSITION_WITH_ORDER);
  registerState(STATE_POSITION_WITH_ORDER);
  registerState(STATE_POSITION_WITHOUT_ORDER);
}

void Context::registerState(StateID state_id) {
  states_[state_id] = State::createState(state_id, this);
}

void Context::handleMDInfo(const MDInfo& md_instru1,
                           const MDInfo& md_instru2) {
  MOON_TRACE <<"Context::handleMDInfo()";

  std::unique_lock<std::mutex> lck(state_mutex_);
  states_[state_id_]->handleMDInfo(md_instru1, md_instru2);
}

void Context::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"Context::handleOrderInfo()";

  std::unique_lock<std::mutex> lck(state_mutex_);
  states_[state_id_]->handleOrderInfo(order);
}

void Context::handleTradeInfo(const TradeInfo& trade) {
  MOON_TRACE <<"Context::handleTradeInfo()";

  std::unique_lock<std::mutex> lck(state_mutex_);
  states_[state_id_]->handleTradeInfo(trade);
}

void Context::setStateID(StateID state_id) {
  MOON_TRACE <<"Context::setStateID()";

  if (state_id != state_id_) {
    MOON_DEBUG <<"======= state changed: "
               <<state_id_ <<" ----> "
               <<state_id
               <<" =======";

    state_id_ = state_id;
  }
}

};  // namespace moon
