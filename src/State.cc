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
    case STATE_SHORT_POSITION_INIT:
      return new ShortPositionInitState(context);

    case STATE_SHORT_POSITION_OPEN:
      return new ShortPositionOpenState(context);

    case STATE_POSITION_INIT:
      return new PositionInitState(context);

    case STATE_POSITION_CLOSE:
      return new PositionCloseState(context);

    default:
      throw std::runtime_error("Invalid state id");
  }
}

void ShortPositionInitState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionInitState::handleMDInfo()";

  context()->server()->tick()->basisEvent(md_instru1, md_instru2);
}

void ShortPositionOpenState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionOpenState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void PositionInitState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionInitState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

void PositionCloseState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionCloseState::handleMDInfo()";

  context()->server()->tick()->pushBasis(md_instru1, md_instru2);
}

};  // namespace moon
