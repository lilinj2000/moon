// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_CONTEXT_HH
#define MOON_CONTEXT_HH

#include <map>
#include <deque>
#include <mutex>
#include "State.hh"

namespace moon {

typedef std::deque<double> BasisQueue;

class Server;

class Context {
 public:
  Context(Server* server);

  ~Context() {
  }

  void handleMDInfo(const MDInfo&, const MDInfo&);

  void basisEvent(const MDInfo&, const MDInfo&);
  void pushBasis(const MDInfo&, const MDInfo&);

 protected:
  bool triggerOpen(double basis, const BasisQueue&,
                   int queue_size, double delta);

 private:
  Server* server_;

  friend class State;

  std::mutex state_mutex_;
  StateID state_id_;
  std::map<StateID, State*> states_;

  // basis
  BasisQueue long_basis_queue_;
  BasisQueue short_basis_queue_;
};

};  // namespace moon

#endif
