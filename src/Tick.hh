// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_TICK_HH
#define MOON_TICK_HH

#include <deque>
#include "subject/Service.hh"

namespace moon {

typedef std::deque<double> BasisQueue;

class Server;

typedef struct {
  std::string instru;
  std::string time_stamp;
  double bid_price1;
  double ask_price1;
}MDInfo;

class Tick : public subject::ServiceCallback {
 public:
  Tick(Server *);

  ~Tick();

  virtual void onMessage(const std::string& msg);

  bool basisEvent(const MDInfo&, const MDInfo&);

  void pushBasis(const MDInfo&, const MDInfo&);

 protected:
  bool triggerOpen(double basis, const BasisQueue&,
                   int queue_size, double delta);

 private:
  Server* server_;

  std::string instru1_;
  std::string instru2_;

  std::unique_ptr<MDInfo> md_instru1_;
  std::unique_ptr<MDInfo> md_instru2_;

  std::unique_ptr<subject::Service> tick_service_;

  // basis
  BasisQueue long_basis_queue_;
  BasisQueue short_basis_queue_;
};
  
} // namespace moon

#endif
