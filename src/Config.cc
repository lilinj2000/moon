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
      ("moon.md_sub_addr", po::value<std::string>(&md_sub_addr),
       "md sub address")
      ("moon.trade_sub_addr", po::value<std::string>(&trade_sub_addr),
       "trade sub address")
      ("moon.push_addr", po::value<std::string>(&push_addr),
       "push address")
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
