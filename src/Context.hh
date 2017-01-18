// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_CONTEXT_HH
#define MOON_CONTEXT_HH

#include <map>
#include <queue>
#include <mutex>
#include "State.hh"

namespace moon {

class Server;

class Context {
 public:
  Context(Server* server);

  ~Context() {
  }

  void handleMDInfo(const MDInfo&, const MDInfo&);

  void basisEvent(double long_basis, double short_basis);
  void pushBasis(double long_basis, double short_basis);

 private:
  Server* server_;

  friend class State;

  std::mutex state_mutex_;
  StateID state_id_;
  std::map<StateID, State*> states_;

  // basis
  std::queue<double> long_basis_queue_;
  std::queue<double> short_basis_queue_;
};

};  // namespace moon

#endif
