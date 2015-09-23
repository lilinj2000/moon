#ifndef MOON_TRADERSERVICE_CALLBACK_IMPL_HH
#define MOON_TRADERSERVICE_CALLBACK_IMPL_HH

#include "ctp/TraderService.hh"

namespace moon
{

class MoonServer;

class TraderServiceCallbackImpl : public ctp::TraderServiceCallback
{
 public:
  TraderServiceCallbackImpl(MoonServer* server);

  virtual ~TraderServiceCallbackImpl() {}

  // TraderServiceCallback
  virtual void onRspError(int errord_id, const std::string& error_msg) {}
  
  virtual void onRspOrderInsert(int order_ref);

  virtual void onRtnOrder(int order_ref, const std::string& status_msg);

  virtual void onRtnTrade(int order_ref, double price, int volume);

 private:
  MoonServer* server_;
  
}; 

};  // namespace moon

#endif  // MOON_MDSERVICE_CALLBACK_IMPL_HH
