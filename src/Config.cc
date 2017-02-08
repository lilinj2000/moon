// Copyright (c) 2010
// All rights reserved.

#include <fstream>
#include <iostream>
#include "boost/program_options.hpp"

#include "Config.hh"
#include "Log.hh"

namespace moon {

Options::Options():
    config_options_("MoonConfigOptions") {
  namespace po = boost::program_options;

  config_options_.add_options()
      ("moon.instru1", po::value<std::string>(&instru1),
       "instrument 1")
      ("moon.instru2", po::value<std::string>(&instru2),
       "instrument 2")

      ("moon.tick_sub_addr", po::value<std::string>(&tick_sub_addr),
       "tick sub address")
      ("moon.order_sub_addr", po::value<std::string>(&order_sub_addr),
       "trade sub address")
      ("moon.push_addr", po::value<std::string>(&push_addr),
       "push address")
      ("moon.basis_queue_size", po::value<int>(&basis_queue_size),
       "basis queue size")
      ("moon.basis_delta_open", po::value<double>(&basis_delta_open),
       "basis delta open")
      ("moon.profit_close", po::value<double>(&profit_close),
       "profit close")
      ("moon.fee_per_volume", po::value<double>(&fee_per_volume),
       "fee per volume")
      ("moon.fee_per_money", po::value<double>(&fee_per_money),
       "fee per money")
      ("moon.volume_multiple", po::value<double>(&volume_multiple),
       "volume multiple")
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

  std::unique_ptr<soil::Config> config(soil::Config::create());
  config->registerOptions(options_.get());

  config->configFile() = "moon.cfg";
  config->loadConfig(argc, argv);

  // init the log
  MOON_LOG_INIT(options_->log_cfg);

  return;
}

Config::~Config() {
}

};  // namespace moon
