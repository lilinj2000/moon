// Copyright (c) 2010
// All rights reserved.

#include <fstream>
#include <iostream>
#include "boost/program_options.hpp"

#include "Config.hh"
#include "Log.hh"
#include "cata/MDService.hh"
#include "cata/TraderService.hh"

namespace moon {

Options::Options():
    config_options_("MoonConfigOptions") {
  namespace po = boost::program_options;

  config_options_.add_options()
      ("moon.instru1", po::value<std::string>(&instru1),
       "instrument 1")
      ("moon.instru2", po::value<std::string>(&instru2),
       "instrument 2")

      ("moon.spread_queue_size", po::value<int>(&spread_queue_size),
       "spread queue size")

      ("moon.spread_ma_lower_boundary",
       po::value<double>(&spread_ma_lower_boundary),
       "spread ma lower boundary")

      ("moon.close_spread_lower_boundary",
       po::value<double>(&close_spread_lower_boundary),
       "close spread lower boundary")

      ("moon.max_wait_ticker_size",
       po::value<int>(&max_wait_ticker_size),
       "max wait ticker size")

      ("moon.delta_price",
       po::value<double>(&delta_price),
       "delta price")

      ("moon.log_cfg",
       po::value<std::string>(&log_cfg),
       "log config file");

  return;
}

Options::~Options() {
}

po::options_description* Options::configOptions() {
  return &config_options_;
}

Config::Config(int argc, char* argv[]) {
  options_.reset(new Options());
  cata_md_options_.reset(cata::MDService::createOptions());
  cata_trader_options_.reset(cata::TraderService::createOptions());

  std::unique_ptr<soil::Config> config(soil::Config::create());
  config->registerOptions(options_.get());
  config->registerOptions(cata_md_options_.get());
  config->registerOptions(cata_trader_options_.get());

  config->configFile() = "moon.cfg";
  config->loadConfig(argc, argv);

  // init the log
  MOON_LOG_INIT(options_->log_cfg);

  return;
}

Config::~Config() {
}

};  // namespace moon
