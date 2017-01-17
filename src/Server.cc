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

  push_service_.reset(zod::PushService::create(config_->options()->push_addr));

  std::string filter = config_->options()->instru1;
  filter += "|" + config_->options()->instru2;

  MOON_DEBUG <<"filter: " <<filter;
  subject::Options md_options {
    filter,
        config_->options()->md_sub_addr
        };
  md_callback_.reset(new MDServiceCallback(this));
  md_service_.reset(subject::Service::createService(md_options, md_callback_.get()));

  subject::Options trade_options {
    filter,
        config_->options()->trade_sub_addr
        };
  trade_callback_.reset(new TradeServiceCallback(this));
  trade_service_.reset(subject::Service::createService(trade_options, trade_callback_.get()));
}

Server::~Server() {
  MOON_TRACE <<"Server::~Server()";
}

void Server::MDServiceCallback::onMessage(const std::string& msg) {
  MOON_TRACE <<"Server::MDServiceCallback::onMessage()";

  server_->index_->pushMsg(new std::string(msg));
}

void Server::TradeServiceCallback::onMessage(const std::string& msg) {
  MOON_TRACE <<"Server::TradeServiceCallback::onMessage()";
}

};  // namespace moon
