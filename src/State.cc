// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "State.hh"
#include "Context.hh"
#include "Server.hh"
#include "Tick.hh"

namespace moon {

State* State::createState(StateID id, Context* context) {
  switch(id) {
    case STATE_SHORT_POSITION_WITHOUT_ORDER:
      return new ShortPositionWithoutOrderState(context);

    case STATE_SHORT_POSITION_WITH_ORDER:
      return new ShortPositionWithOrderState(context);

    case STATE_POSITION_WITH_ORDER:
      return new PositionWithOrderState(context);

    case STATE_POSITION_WITHOUT_ORDER:
      return new PositionWithoutOrderState(context);

    default:
      throw std::runtime_error("Invalid state id");
  }
}

void State::toNextState(StateID state_id) {
  context_->setStateID(state_id);
}

void ShortPositionWithoutOrderState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionWithoutOrderState::handleMDInfo()";

  if (context()->server()->tick()->basisEvent(md_instru1, md_instru2)) {
    toNextState(STATE_SHORT_POSITION_WITH_ORDER);
  }
}

void ShortPositionWithoutOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"ShortPositionWithoutOrderState::handleOrderInfo()";

  MOON_ERROR <<"Error, unexpected order happened.";
}

void ShortPositionWithOrderState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionWithOrderState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void ShortPositionWithOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"ShortPositionWithOrderState::handleOrderInfo()";

  if (context()->server()->order()->updateOrder(order)) {
    toNextState(STATE_POSITION_WITHOUT_ORDER);
  }
}

void PositionWithOrderState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionWithOrderState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void PositionWithoutOrderState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionWithoutOrderState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

};  // namespace moon
