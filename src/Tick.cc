// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Tick.hh"
#include "Server.hh"
#include "json/json.hh"

namespace moon {

Tick::Tick(Server* server):
    server_(server) {
  MOON_TRACE <<"Tick::Tick()";

  md_instru1_.reset(new MDInfo());
  md_instru2_.reset(new MDInfo());

  md_instru1_->instru = server_->config()->options()->instru1;
  md_instru2_->instru = server_->config()->options()->instru2;

  std::string filter = md_instru1_->instru;
  filter += "|" + md_instru2_->instru;

  MOON_DEBUG <<"filter: " <<filter;
  subject::Options tick_options {
    filter,
        server_->config()->options()->tick_sub_addr
        };
  tick_service_.reset(subject::Service::createService(tick_options, this));
}

Tick::~Tick() {
  MOON_TRACE <<"Tick::~Tick()";
}

void Tick::onMessage(const std::string& msg) {
  MOON_TRACE <<"Tick::onMessage()";

  MOON_DEBUG <<msg;

  json::Document doc;
  json::fromString(msg, &doc);

  auto itr = doc.MemberBegin();
  std::string key = itr->name.GetString();
  json::Value& data = doc[key.data()];

  // OnRtnDepthMarketData
  if (key == "OnRtnDepthMarketData") {
    auto itr = data.FindMember("CThostFtdcDepthMarketDataField");
    if (itr != data.MemberEnd()) {
      std::string instru = (itr->value)["InstrumentID"].GetString();
      std::string bid_price1 = (itr->value)["BidPrice1"].GetString();
      std::string ask_price1 = (itr->value)["AskPrice1"].GetString();
      std::string time_stamp = (itr->value)["UpdateTime"].GetString();
      time_stamp += ".";
      time_stamp += (itr->value)["UpdateMillisec"].GetString();

      if (instru == md_instru1_->instru) {
        md_instru1_->time_stamp = time_stamp;
        md_instru1_->bid_price1 = std::stod(bid_price1);
        md_instru1_->ask_price1 = std::stod(ask_price1);
      } else if (instru == md_instru2_->instru) {
        md_instru2_->time_stamp = time_stamp;
        md_instru2_->bid_price1 = std::stod(bid_price1);
        md_instru2_->ask_price1 = std::stod(ask_price1);
      }

      if (!md_instru1_->time_stamp.empty()
          && !md_instru2_->time_stamp.empty()) {

        server_->context()->handleMDInfo(*md_instru1_,
                                        *md_instru2_);
      }
    }
  }
}

void Tick::basisEvent(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"Tick::basisEvent()";

  double long_basis = md_instru2.bid_price1
      - md_instru1.ask_price1;

  double short_basis = md_instru2.ask_price1
      - md_instru1.bid_price1;

  int queue_size = server_->config()->options()->basis_queue_size;

  double delta_open = server_->config()->options()->basis_delta_open;

  const std::string buy = "0";
  const std::string sell = "1";
  int volume = 1;

  if (triggerOpen(long_basis, long_basis_queue_,
                  queue_size, delta_open)) {
    // buy instru1, sell instru2
    server_->order()->instruOpen(md_instru1.instru,
                                 buy,
                                 md_instru1.ask_price1,
                                 volume);

    server_->order()->instruOpen(md_instru2.instru,
                                 sell,
                                 md_instru2.bid_price1,
                                 volume);
  } else if (triggerOpen(short_basis,
                         short_basis_queue_,
                         queue_size, delta_open)) {
    // sell instru1, buy instru2
    server_->order()->instruOpen(md_instru1.instru,
                                 sell,
                                 md_instru1.bid_price1,
                                 volume);

    server_->order()->instruOpen(md_instru2.instru,
                                 buy,
                                 md_instru2.ask_price1,
                                 volume);
  }

  pushBasis(md_instru1, md_instru2);
}

bool Tick::triggerOpen(double basis,
                          const BasisQueue& basis_queue,
                          int queue_size,
                          double delta) {
  MOON_TRACE <<"Tick::triggerOpen()";

  if (basis_queue.size() >= queue_size) {
    double ma = std::accumulate(basis_queue.begin(),
                                basis_queue.end(),
                                0) / basis_queue.size();

    if (std::fabs(basis - ma) >= delta) {
      return true;
    }
  }

  return false;
}

void Tick::pushBasis(const MDInfo& md_instru1, const MDInfo& md_instru2) {
  MOON_TRACE <<"Tick::pushBasis()";

  double long_basis = md_instru2.bid_price1
      - md_instru1.ask_price1;

  double short_basis = md_instru2.ask_price1
      - md_instru1.bid_price1;

  int basis_queue_size = server_->config()->options()->basis_queue_size;

  while (long_basis_queue_.size() >= basis_queue_size) {
    long_basis_queue_.pop_front();
  }

  while (short_basis_queue_.size() >= basis_queue_size) {
    short_basis_queue_.pop_front();
  }
  
  long_basis_queue_.push_back(long_basis);

  short_basis_queue_.push_back(short_basis);
}

}  // namespace moon
