#ifndef MOON_STRATEGY_HH
#define MOON_STRATEGY_HH

#include "MoonConfig.hh"
#include "soil/DateTime.hh"
#include <boost/thread/sync_queue.hpp>
#include <boost/scoped_ptr.hpp>
#include <atomic>
#include <deque>

namespace moon
{

typedef struct
{
  std::string instru;
  double bid_price1;
  double bid_volume1;
  double ask_price1;
  double ask_volume1;

  soil::DateTime time_stamp;
  
  unsigned int seq;
}TickerInfo;

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, TickerInfo const& aTicker)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"TickerInfo\": {" <<std::endl;
  os <<"        \"instru\": \"" <<aTicker.instru  <<"\"," <<std::endl;
  os <<"        \"bid_price1\": \"" <<aTicker.bid_price1  <<"\"," <<std::endl;
  os <<"        \"bid_volume1\": \"" <<aTicker.bid_volume1  <<"\"," <<std::endl;
  os <<"        \"ask_price1\": \"" <<aTicker.ask_price1  <<"\"," <<std::endl;
  os <<"        \"ask_volume1\": \"" <<aTicker.ask_volume1  <<"\"," <<std::endl;
  os <<"        \"time_stamp\": \"" <<aTicker.time_stamp  <<"\"," <<std::endl;
  os <<"        \"seq\": \"" <<aTicker.seq  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}


class MoonServer;

class Strategy
{
 public:
  
  Strategy(MoonOptions* options, MoonServer* server);
  
  ~Strategy();

  void pushTickerInfo(TickerInfo* ticker);

  typedef enum
  {
    INIT_STATUS,
    ORDER_GOING,
    ORDER_TURNOVER,
    ORDER_CLOSE_GOING
  }Status;

  void updateStatus(Status status)
  {
    status_ = status;
  }

  void updateStatus(double spread);

 protected:
  
  void statusInit(TickerInfo* instru1, TickerInfo* instru2);
  
  void statusOrderGoing(TickerInfo* instru1, TickerInfo* instru2);
  
  void statusOrderTurnOver(TickerInfo* instru1, TickerInfo* instru2);

  void statusOrderCloseGoing(TickerInfo* instru1, TickerInfo* instru2);
  
 private:

  void run();
  

  boost::sync_queue<TickerInfo*> instru_ticker_queue_;
  // boost::sync_queue<TickerInfo*> instru2_ticker_queue_;

  std::deque<double> spread_queue_;
  std::unique_ptr<boost::thread> exec_thread_;

  MoonOptions* options_;


  Status status_;
  double t_spread_;
  
  MoonServer* server_;

  std::atomic_int wait_ticker_count_;
};

}  // namespace moon


#endif // MOON_STATEGY_HH_
