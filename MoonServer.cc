#include "MoonServer.hh"
#include "MoonLog.hh"
#include "MDServiceCallbackImpl.hh"
#include "TraderServiceCallbackImpl.hh"
#include "Strategy.hh"
#include <boost/thread.hpp>

namespace moon
{

MoonServer::MoonServer(int argc, char* argv[]):
    server_running_(false)
{

  config_.reset( new MoonConfig(argc, argv) );

  md_callback_.reset( new MDServiceCallbackImpl(this) );
  
  md_service_.reset( ctp::MDService::createService(config_->ctpMDOptions(), md_callback_.get()) );


  trader_callback_.reset( new TraderServiceCallbackImpl(this) );
  trader_service_.reset( ctp::TraderService::createService(config_->ctpTraderOptions(), trader_callback_.get()) );

  server_timer_.reset( soil::Condition::create() );
  
  strategy_.reset( new Strategy(config_->moonOptions(), this) );
}

MoonServer::~MoonServer()
{
}

void MoonServer::start()
{

  if( !server_running_ )
  {
    server_running_ = true;

    server_thread_.reset(new boost::thread(&MoonServer::run, this));
  }

  
}

void MoonServer::stop()
{
  server_running_ = false;
  
  server_timer_->notifyOne();
}

void MoonServer::run()
{
  ctp::InstrumentSet instrus;
  instrus.insert( config_->moonOptions()->instru1 );
  instrus.insert( config_->moonOptions()->instru2 );
  
  md_service_->subMarketData( instrus );


  while( server_running_ )
  {
    server_timer_->wait(2000);
  }

}

void MoonServer::orderInsert(const std::string& instrument,
                             double price, int volume,
                             bool is_buy, bool is_closed)
{
  MOON_CUSTOM <<"order insert:"
              <<" instrument - " <<instrument
              <<" price - " <<price
              <<" volume - " <<volume
              <<" is_buy - " <<std::boolalpha <<is_buy
              <<" is_closed - " <<std::boolalpha <<is_closed;

  std::string instrument_id = instrument;
  ctp::OrderPriceType price_type = ctp::LIMIT_PRICE;
  ctp::DirectionType direction = ctp::BUY;
  double quote_price = price;
  if( !is_buy )
  {
    direction = ctp::SELL;
    quote_price -= config_->moonOptions()->delta_price;
  }
  else
  {
    quote_price += config_->moonOptions()->delta_price;
  }

  ctp::OpenCloseFlagType oc = ctp::OC_OPEN;
  if( is_closed )
  {
    oc = ctp::OC_CLOSE_TODAY;
  }
  
  // double price = 4680;
  // int volume = 1;
  ctp::TimeConditionType tc = ctp::TC_GFD;
  std::string specific_date;
  ctp::VolumeConditionType vc = ctp::VC_AV;
  int min_volume = 1;
  ctp::ContingentConditionType cc = ctp::CC_IMMEDIATELY;
  double stop_price = 0;
  ctp::ForceCloseReasonType fc = ctp::FC_NOT_FORCFE_CLOSE;
  int auto_suspend = 0;

  int order_ref = trader_service_->orderInsert(instrument_id,
                                               price_type,
                                               direction, oc,
                                               quote_price,
                                               volume, tc,
                                               specific_date,
                                               vc, min_volume,
                                               cc, stop_price,
                                               fc, auto_suspend);

  {
    boost::unique_lock<boost::mutex> lock(trade_info_mutex_);
    TradeInfo info(instrument, is_buy, price, volume);
    trade_info_[order_ref] = info;
  }
}

void MoonServer::updateTradeInfo(int order_ref, double price, int volume)
{
  MOON_CUSTOM <<"updateTradeInfo ... !!!!";
  
  boost::unique_lock<boost::mutex> lock(trade_info_mutex_);

  std::map<int, TradeInfo>::iterator i_iter = trade_info_.find(order_ref);

  if( i_iter!=trade_info_.end() )
  {
    i_iter->second.t_price_ = price;
    i_iter->second.t_volume_ += volume;
  }
  else
  {
    MOON_CUSTOM <<"trade order_ref - " <<order_ref <<" not found !!!";
    return ;
  }

  bool all_traded = true;
  double spread = 0;
  for(i_iter=trade_info_.begin(); i_iter!=trade_info_.end(); i_iter++)
  {
    MOON_CUSTOM <<"trade info - volume_: " <<i_iter->second.volume_
                <<" t_volume_: " <<i_iter->second.t_volume_;
    
    if( i_iter->second.volume_!=i_iter->second.t_volume_ )
    {
      all_traded = false;
      break;
    }
    else
    {
      if( i_iter->second.instru_== config_->moonOptions()->instru2 )
      {
        spread += i_iter->second.t_price_;
      }
      else
      {
        spread -= i_iter->second.t_price_;
      }

      MOON_CUSTOM <<"trade info - t_price_: " <<i_iter->second.t_price_
                  <<" spread: " <<spread;

    }
  }

  if( all_traded )
  {
    trade_info_.clear();
    
    strategy_->updateStatus(spread);
  }
}

};  // namespace moon
