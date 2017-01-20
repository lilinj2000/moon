// Copyright (c) 2010
// All rights reserved.

#include "Server.hh"
#include "Log.hh"

namespace moon {

Server::Server(int argc, char* argv[]) {
  MOON_TRACE <<"Server::Server()";

  config_.reset(new Config(argc, argv));

  tick_.reset(new Tick(this,
                         config_->options()->instru1,
                         config_->options()->instru2));

  context_.reset(new Context(this));
}

Server::~Server() {
  MOON_TRACE <<"Server::~Server()";
}

};  // namespace moon
