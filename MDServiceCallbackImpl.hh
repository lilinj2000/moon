#ifndef MOON_MDSERVICE_CALLBACK_IMPL_HH
#define MOON_MDSERVICE_CALLBACK_IMPL_HH

#include "cata/MDService.hh"
#include <boost/atomic.hpp>

namespace moon
{

class MoonServer;

class MDServiceCallbackImpl : public cata::MDServiceCallback
{
 public:
  MDServiceCallbackImpl(MoonServer* server);

  virtual ~MDServiceCallbackImpl() {}

  // MDServiceCallback
  virtual void onRspSubMarketData(const std::string& instru, bool success){}
  virtual void onRspSubQuoteData(const std::string& instru, bool success) {}
  virtual void onRspUnsubMarketData(const std::string& instru, bool success) {}
  virtual void onRspUnsubQuoteData(const std::string& instru, bool success) {}
  virtual void onRtnMarketData(const cata::DepthMarketData* data);
  virtual void onRtnQuoteData() {}
  virtual void onRspError(int errord_id, const std::string& error_msg) {}

 private:
  MoonServer* server_;

  boost::atomic<unsigned int> md_seq_;
}; 

};  // namespace moon

#endif  // MOON_MDSERVICE_CALLBACK_IMPL_HH
