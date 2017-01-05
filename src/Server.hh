// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_SERVER
#define MOON_SERVER

#include "cata/MDService.hh"
#include "cata/TraderService.hh"

#include "Config.hh"

#include <memory>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/scoped_ptr.hpp>
#include "soil/STimer.hh"

namespace moon {

class Strategy;
class MDServiceCallbackImpl;
class TraderServiceCallbackImpl;

struct TradeInfo {
  std::string instru_;
  bool is_buy_;
  double price_;
  int volume_;

  double t_price_;
  int t_volume_;

  TradeInfo(const std::string& instru, bool is_buy,
            double price, double volume ):
      instru_(instru),
      is_buy_(is_buy),
      price_(price),
      volume_(volume),
      t_price_(-1),
      t_volume_(0) {
  }

  TradeInfo() {
  }
  
  TradeInfo(const TradeInfo& tradeInfo) {
    instru_ = tradeInfo.instru_;
    is_buy_ = tradeInfo.is_buy_;
    price_ = tradeInfo.price_;
    volume_ = tradeInfo.volume_;
    t_price_ = tradeInfo.t_price_;
    t_volume_ = tradeInfo.t_volume_;
  }
};

class Server {
 public:
  Server(int argc, char* argv[]);

  ~Server();
  
  void start();

  void stop();

  void updateTradeInfo(int order_ref, const std::string& instru, bool is_buy, double price, int volume);
  
  void updateTradeInfo(int order_ref, double price, int volume);

  Strategy* strategy() {
    return strategy_.get();
  }

  cata::TraderService* traderService() {
    return trader_service_.get();
  }

 private:
  void run();

  std::unique_ptr<Config> config_;
  
  std::unique_ptr<MDServiceCallbackImpl> md_callback_;
  std::unique_ptr<cata::MDService> md_service_;

  std::unique_ptr<TraderServiceCallbackImpl> trader_callback_;
  std::unique_ptr<cata::TraderService> trader_service_;

  std::unique_ptr<Strategy> strategy_;

  boost::mutex trade_info_mutex_;
  std::map<int, TradeInfo> trade_info_;

  bool server_running_;
  std::unique_ptr<soil::STimer> server_timer_;

  boost::scoped_ptr<boost::thread> server_thread_;
};

};  // namespace moon

#endif
