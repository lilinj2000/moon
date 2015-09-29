#include "MoonConfig.hh"
#include "MoonLog.hh"
#include "cata/MDService.hh"
#include "cata/TraderService.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace moon
{

MoonOptions::MoonOptions():
    config_options_("FlashTCPConfigOptions")
{

  namespace po = boost::program_options;

  config_options_.add_options()
      ("moon.instru1", po::value<std::string>(&instru1), 
       "instrument 1")
      ("moon.instru2", po::value<std::string>(&instru2), 
       "instrument 2")

      ("moon.spread_queue_size", po::value<int>(&spread_queue_size), 
       "spread queue size")

      ("moon.spread_ma_lower_boundary", po::value<double>(&spread_ma_lower_boundary), 
       "spread ma lower boundary")

      ("moon.close_spread_lower_boundary", po::value<double>(&close_spread_lower_boundary), 
       "close spread lower boundary")

      ("moon.max_wait_ticker_size", po::value<int>(&max_wait_ticker_size), 
       "max wait ticker size")

      ("moon.delta_price", po::value<double>(&delta_price), 
       "delta price")

      ("moon.log_cfg", po::value<std::string>(&log_cfg), 
       "log config file")
      ;

  return;
  
}

MoonOptions::~MoonOptions()
{
}

po::options_description* MoonOptions::configOptions()
{
  return &config_options_;
}

MoonConfig::MoonConfig(int argc, char* argv[])
{
  moon_options_.reset(new MoonOptions());
  cata_md_options_.reset( cata::MDService::createOptions() );
  cata_trader_options_.reset( cata::TraderService::createOptions() );
      
  std::auto_ptr<soil::Config> config( soil::Config::create() );
  config->registerOptions( moon_options_.get() );
  config->registerOptions( cata_md_options_.get() );
  config->registerOptions( cata_trader_options_.get() );

  config->configFile() = "moon.cfg";
  config->loadConfig(argc, argv);
  
  // init the log
  MOON_LOG_INIT( moon_options_->log_cfg );
  
  return;
}

MoonConfig::~MoonConfig()
{
}

};  
