#include "MDServiceCallbackImpl.hh"
#include "MoonServer.hh"
#include "MoonLog.hh"
#include "Strategy.hh"


#include <boost/chrono.hpp>

namespace moon
{

MDServiceCallbackImpl::MDServiceCallbackImpl(MoonServer* server):
    server_(server),
    md_seq_(0) {
}

void MDServiceCallbackImpl::onRtnMarketData(const cata::DepthMarketData* data) {
  std::unique_ptr<TickerInfo> aInfo(new TickerInfo());
  
  aInfo->instru = data->InstrumentID;
  aInfo->bid_price1 = data->BidPrice1;
  aInfo->bid_volume1 = data->BidVolume1;
  aInfo->ask_price1 = data->AskPrice1;
  aInfo->ask_volume1 = data->AskVolume1;
  aInfo->time_stamp = data->time_stamp;
  aInfo->seq = ++md_seq_;

  MOON_CUSTOM <<*aInfo;

  server_->strategy()->pushTickerInfo( aInfo.release() );
}

};  // namespace moon
