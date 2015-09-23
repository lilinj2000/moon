#include "TraderServiceCallbackImpl.hh"
#include "MoonServer.hh"
#include "MoonLog.hh"
#include "Strategy.hh"

namespace moon
{

TraderServiceCallbackImpl::TraderServiceCallbackImpl(MoonServer* server):
    server_(server)
{
}

void TraderServiceCallbackImpl::onRspOrderInsert(int order_ref)
{
  MOON_CUSTOM <<"onRspOrderInsert - order_ref: " <<order_ref <<" order insert success!!!";

}

void TraderServiceCallbackImpl::onRtnOrder(int order_ref, const std::string& status_msg)
{
  MOON_CUSTOM <<"onRtnOrder => order_ref:" <<order_ref <<" status_msg:" <<status_msg ;
}

void TraderServiceCallbackImpl::onRtnTrade(
    int order_ref, double price, int volume)
{
  MOON_CUSTOM <<"onRtnTrade => order_ref:" <<order_ref
              <<" price:" <<price
              <<" volume:" <<volume;

  server_->updateTradeInfo(order_ref, price, volume);

}


};  // namespace moon
