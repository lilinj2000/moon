// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "State.hh"
#include "Context.hh"

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

  // context()->basisEvent(long_basis, short_basis);
}

void ShortPositionOpenState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"ShortPositionOpenState::handleMDInfo()";

  // context()->pushBasis(long_basis, short_basis);
}

void PositionInitState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionInitState::handleMDInfo()";

  // context()->pushBasis(long_basis, short_basis);
}

void PositionCloseState::handleMDInfo(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"PositionCloseState::handleMDInfo()";

  // context()->pushBasis(long_basis, short_basis);
}

};  // namespace moon
