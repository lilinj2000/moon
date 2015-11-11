#include "Strategy.hh"
#include "MoonLog.hh"
#include "MoonServer.hh"

#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <numeric>

namespace moon
{

Strategy::Strategy(MoonOptions* options, MoonServer* server):
    options_(options),
    status_(INIT_STATUS),
    t_spread_(-1),
    server_(server),
    wait_ticker_count_(0)
{
  exec_thread_.reset(new boost::thread(&Strategy::run, this));
}

Strategy::~Strategy()
{
  instru_ticker_queue_.close();
  // instru2_ticker_queue_.close();

  exec_thread_->join();
}

void Strategy::pushTickerInfo(TickerInfo* ticker)
{
  MOON_TRACE <<"Strategy::pushTickerInfo";
  
  std::unique_ptr<TickerInfo> p_ticker(ticker) ;
  
  if( p_ticker->instru == options_->instru1
      || p_ticker->instru == options_->instru2)
  {
    instru_ticker_queue_.push( p_ticker.release() );
  }
  // else if( p_ticker->instru_ == cfg_.instru2_ )
  // {
  //   instru_ticker_queue_.push_back( p_ticker.release() );
  // }
  else
  {
    MOON_WARNING <<"ignore the ticker: " <<p_ticker->instru;
  }

  MOON_DEBUG <<"Strategy::pushTickerInfo done";

}

void Strategy::updateStatus(double spread)
{
  MOON_CUSTOM <<"update status the t_spread is " <<spread;
  
  t_spread_ = spread;

  if( status_==ORDER_GOING )
  {
    wait_ticker_count_ = 0;
    
    status_ = ORDER_TURNOVER;
  }
  else if( status_==ORDER_CLOSE_GOING )
  {
    status_ = INIT_STATUS;
  }
  else
    MOON_CUSTOM <<"update status " <<status_  <<" !!!!";
}

void Strategy::run()
{
  try
  {
    do
    {
      // TickerInfo* instru1_ticker = NULL;
      // TickerInfo* instru2_ticker = NULL;

      TickerInfo* cur_ticker = NULL;

      boost::queue_op_status res = instru_ticker_queue_.wait_pull(cur_ticker);

      if( res==boost::queue_op_status::closed )
        break;
      
      boost::scoped_ptr<TickerInfo>  instru1(cur_ticker);

      TickerInfo* next_ticker = NULL;
      boost::scoped_ptr<TickerInfo>  instru2;
      do
      {
        boost::queue_op_status res = instru_ticker_queue_.wait_pull(next_ticker);
        
        if( res==boost::queue_op_status::closed )
          break;
      
        instru2.reset(next_ticker);

        if( instru2->instru==instru1->instru )
        {
          instru1.swap(instru2);
          next_ticker = NULL;
        }
        else
        {
          boost::posix_time::time_duration duration = instru2->time_stamp - instru1->time_stamp;
          MOON_DEBUG <<"instru2->seq: " <<instru2->seq <<"\t"
                     <<"instru1->seq: " <<instru1->seq;
          
          // MOON_DEBUG <<"duration ms: " <<ms;

          if( duration.total_milliseconds()<1000 )
          {
            break;
          }
        }
      }while(true);

      if( instru1->instru==options_->instru2 )
      {
        instru1.swap(instru2);
      }
          
      switch(status_)
      {
        case INIT_STATUS:
          statusInit(instru1.get(), instru2.get());
          break;
          
        case ORDER_GOING:
          statusOrderGoing(instru1.get(), instru2.get());
          break;

        case ORDER_TURNOVER:
          statusOrderTurnOver(instru1.get(), instru2.get());
          break;

        case ORDER_CLOSE_GOING:
          statusOrderCloseGoing(instru1.get(), instru2.get());
          break;

        default:
          MOON_CUSTOM <<"invalid status !!!";
      }
    }while( true );
  }
  catch( ... )
  {
  }
}

void Strategy::statusInit(TickerInfo* instru1, TickerInfo* instru2)
{

  MOON_CUSTOM <<"statusInit .... !!!!";
  
  double spread = instru2->bid_price1 - instru1->ask_price1;

  MOON_CUSTOM <<"Strategy::statusInit " <<instru2->bid_price1 <<" - " <<instru1->ask_price1  <<" = " << spread;
  
  if( spread_queue_.size()>=options_->spread_queue_size )
  {
    double ma = std::accumulate(spread_queue_.begin(), spread_queue_.end(), 0.0)/spread_queue_.size();

    MOON_CUSTOM <<"ma: " <<ma;
        
    if( spread>(ma + options_->spread_ma_lower_boundary) )
    {
      MOON_CUSTOM <<"go .... !!!!!";

      int order_ref1 = server_->traderService()->orderOpenSell(instru2->instru, instru2->bid_price1, 1);
      server_->updateTradeInfo(order_ref1, instru2->instru, false, instru2->bid_price1, 1);
      
      int order_ref2 = server_->traderService()->orderOpenBuy(instru1->instru, instru1->ask_price1, 1);
      server_->updateTradeInfo(order_ref2, instru1->instru, true, instru1->ask_price1, 1);

      updateStatus( ORDER_GOING );
    }

    spread_queue_.pop_front();
  }

  spread_queue_.push_back( spread );
}

void Strategy::statusOrderGoing(TickerInfo* instru1, TickerInfo* instru2)
{
  MOON_CUSTOM <<"statusOrderGoing ... !!!!";
  
  double spread = instru2->bid_price1 - instru1->ask_price1;

  if( spread_queue_.size()>=options_->spread_queue_size )
  {
    spread_queue_.pop_front();
  }

  spread_queue_.push_back( spread );
}

void Strategy::statusOrderTurnOver(TickerInfo* instru1, TickerInfo* instru2)
{
  double spread = instru2->bid_price1 - instru1->ask_price1;
  
  if( spread_queue_.size()>=options_->spread_queue_size )
  {
    spread_queue_.pop_front();
  }

  spread_queue_.push_back( spread );

  // compute whether close the order
  spread = instru2->ask_price1 - instru1->bid_price1;
  MOON_CUSTOM <<"Strategy::statusOrderTurnOver " <<instru2->ask_price1 <<" - " <<instru1->bid_price1 <<" = " << spread;

  int wait_ticker_count = ++wait_ticker_count_;
  
  MOON_CUSTOM <<"statusOrderTurnOver - t_spread: " <<t_spread_
              <<" spread: " <<spread
              <<" close_lower_boundary: " <<options_->close_spread_lower_boundary
              <<" wait_ticker_count: " <<wait_ticker_count;
  
  if( t_spread_-spread > options_->close_spread_lower_boundary
      || wait_ticker_count >= options_->max_wait_ticker_size )
  {
    MOON_CUSTOM <<"spread reduced to the lower boundary, or the max wait ticker reached, Go close the order !!!! ";

    int order_ref1 = server_->traderService()->orderCloseBuy(instru2->instru, instru2->ask_price1, 1);
    server_->updateTradeInfo(order_ref1, instru2->instru, true, instru2->bid_price1, 1);
    
    int order_ref2 = server_->traderService()->orderCloseSell(instru1->instru, instru1->bid_price1, 1);
    server_->updateTradeInfo(order_ref2, instru1->instru, false, instru1->ask_price1, 1);
    
    updateStatus( ORDER_CLOSE_GOING );
  }

}

void Strategy::statusOrderCloseGoing(TickerInfo* instru1, TickerInfo* instru2)
{
  MOON_CUSTOM <<"statusOrderCloseGoing ... !!!";
  
  double spread = instru2->bid_price1 - instru1->ask_price1;

  if( spread_queue_.size()>=options_->spread_queue_size )
  {
    spread_queue_.pop_front();
  }

  spread_queue_.push_back( spread );
}

};  // namespace moon
