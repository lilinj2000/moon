// Copyright (c) 2010
// All rights reserved.

#include "Log.hh"
#include "Index.hh"
#include "json/json.hh"

namespace moon {

Index::Index(Server* server,
             const std::string& instru1,
             const std::string& instru2) :
    server_(server),
    instru1_(instru1),
    instru2_(instru2){
  MOON_TRACE <<"Index::Index()";

  md_queue_.reset(new soil::MsgQueue<std::string, Index>(this));

  md_instru1_.reset(new MDInfo());
  md_instru2_.reset(new MDInfo());
}

Index::~Index() {
  MOON_TRACE <<"Index::~Index()";
}

void Index::msgCallback(const std::string* msg) {
  MOON_TRACE <<"Index::msgCallback()";

  MOON_DEBUG <<*msg;

  json::Document doc;
  json::fromString(*msg, &doc);

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

      if (instru == instru1_) {
        md_instru1_->instru = instru;
        md_instru1_->time_stamp = time_stamp;
        md_instru1_->bid_price1 = std::stod(bid_price1);
        md_instru1_->ask_price1 = std::stod(ask_price1);
      } else if (instru == instru2_) {
        md_instru2_->instru = instru;
        md_instru2_->time_stamp = time_stamp;
        md_instru2_->bid_price1 = std::stod(bid_price1);
        md_instru2_->ask_price1 = std::stod(ask_price1);
      }

      if (!md_instru1_->time_stamp.empty()
           && (md_instru1_->time_stamp
               == md_instru2_->time_stamp)) {
        // compute the ma
        ma1_queue_.push(md_instru2_->bid_price1
                        - md_instru1_->ask_price1);

        ma2_queue_.push(md_instru2_->ask_price1
                        - md_instru1_->bid_price1);

        md_instru1_->time_stamp = "";
      }
    }
  }

}

}  // namespace moon
