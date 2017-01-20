// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_SERVER_HH
#define MOON_SERVER_HH

#include <memory>
#include "Config.hh"
#include "Tick.hh"
#include "Context.hh"

namespace moon {

class Server {
 public:
  Server(int argc, char* argv[]);

  ~Server();

  Context* context() {
    return context_.get();
  }

  Config* config() {
    return config_.get();
  }

 private:
  std::unique_ptr<Config> config_;

  std::unique_ptr<Tick> tick_;

  std::unique_ptr<Context> context_;
};

};  // namespace moon

#endif
