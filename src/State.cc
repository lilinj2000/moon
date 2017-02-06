// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "State.hh"
#include "Context.hh"
#include "Server.hh"
#include "Tick.hh"

namespace moon {

State* State::createState(StateID id, Context* context) {
  switch (id) {
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
  MOON_TRACE <<"State::toNextState()";

  context_->setStateID(state_id);
}

void State::toNextState(int id) {
  MOON_TRACE <<"State::toNextState()";

  StateID state_id;
  switch (id) {
    case 1:
      state_id = STATE_SHORT_POSITION_WITHOUT_ORDER;
      break;

    case 2:
      state_id = STATE_SHORT_POSITION_WITH_ORDER;
      break;

    case 3:
      state_id = STATE_POSITION_WITH_ORDER;
      break;

    case 4:
      state_id = STATE_POSITION_WITHOUT_ORDER;
      break;

    default:
      std::string msg = "invalid state id "
          + std::to_string(id);
      throw std::runtime_error(msg);
  }

  toNextState(state_id);
}

void ShortPositionWithoutOrderState::handleMDInfo(
    const MDInfo& md_instru1,
    const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionWithoutOrderState::handleMDInfo()";

  if (context()->server()->tick()->basisEvent(md_instru1, md_instru2)) {
    toNextState(STATE_SHORT_POSITION_WITH_ORDER);
  }
}

void ShortPositionWithoutOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"ShortPositionWithoutOrderState::handleOrderInfo()";

  MOON_ERROR <<"State short position without order, "
             <<"should be no order happened.  instru: "
             <<order.instru;
}

void ShortPositionWithoutOrderState::handleTradeInfo(const TradeInfo& trade) {
  MOON_TRACE <<"ShortPositionWithoutOrderState::handleTradeInfo()";

  toNextState(context()->server()->order()->updatePosition(trade));
}

void ShortPositionWithOrderState::handleMDInfo(
    const MDInfo& md_instru1,
    const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionWithOrderState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void ShortPositionWithOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"ShortPositionWithOrderState::handleOrderInfo()";

  toNextState(context()->server()->order()->updateOrder(order));
}

void ShortPositionWithOrderState::handleTradeInfo(const TradeInfo& trade) {
  MOON_TRACE <<"ShortPositionWithOrderState::handleTradeInfo()";

  toNextState(context()->server()->order()->updatePosition(trade));
}

void PositionWithOrderState::handleMDInfo(
    const MDInfo& md_instru1,
    const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionWithOrderState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void PositionWithOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"PositionWithOrderState::handleOrderInfo()";

  toNextState(context()->server()->order()->updateOrder(order));
}

void PositionWithOrderState::handleTradeInfo(const TradeInfo& trade) {
  MOON_TRACE <<"PositionWithOrderState::handleTradeInfo()";

  toNextState(context()->server()->order()->updatePosition(trade));
}

void PositionWithoutOrderState::handleMDInfo(
    const MDInfo& md_instru1,
    const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionWithoutOrderState::handleMDInfo()";
  // to do
  //  decide whether trigger close and update md info
  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void PositionWithoutOrderState::handleOrderInfo(const OrderInfo& order) {
  MOON_TRACE <<"PositionWithOrderState::handleOrderInfo()";

  MOON_ERROR <<"State position without order, "
             <<"should be no order happened.  instru: "
             <<order.instru;
}

void PositionWithoutOrderState::handleTradeInfo(const TradeInfo& trade) {
  MOON_TRACE <<"PositionWithoutOrderState::handleTradeInfo()";

  toNextState(context()->server()->order()->updatePosition(trade));
}

};  // namespace moon
