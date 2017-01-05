// Copyright (c) 2010
// All rights reserved.

#include "Server.hh"
#include "Log.hh"

namespace moon {

Server::Server(int argc, char* argv[]) {
  MOON_TRACE <<"Server::Server()";

  config_.reset(new Config(argc, argv));

  index_.reset(new Index(this,
                         config_->options()->instru1,
                         config_->options()->instru2));

  std::string filter = config_->options()->instru1;
  filter += "|" + config_->options()->instru2;

  MOON_DEBUG <<"filter: " <<filter;
  subject::Options options {
    filter,
        config_->options()->md_sub_addr
        };

  subject_service_.reset(subject::Service::createService(options, this));
}

Server::~Server() {
}

void Server::onMessage(const std::string& msg) {
  MOON_TRACE <<"Server::onMessage()";

  index_->pushMsg(new std::string(msg));
}

};  // namespace moon
