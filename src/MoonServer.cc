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
  
  md_service_.reset( cata::MDService::createService(config_->cataMDOptions(), md_callback_.get()) );


  trader_callback_.reset( new TraderServiceCallbackImpl(this) );
  trader_service_.reset( cata::TraderService::createService(config_->cataTraderOptions(), trader_callback_.get()) );

  server_timer_.reset( soil::STimer::create() );
  
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
  cata::InstrumentSet instrus;
  instrus.insert( config_->moonOptions()->instru1 );
  instrus.insert( config_->moonOptions()->instru2 );
  
  md_service_->subMarketData( instrus );


  while( server_running_ )
  {
    server_timer_->wait(2000);
  }

}


void MoonServer::updateTradeInfo(int order_ref, const std::string& instru, bool is_buy, double price, int volume)
{
  boost::unique_lock<boost::mutex> lock(trade_info_mutex_);
  
  TradeInfo info(instru, is_buy, price, volume);
  
  trade_info_[order_ref] = info;
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
