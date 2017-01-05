// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_CONFIG_HH
#define MOON_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace moon {

namespace po = boost::program_options;

class Options : public soil::Options {
 public:
  Options();

  virtual ~Options();

  virtual po::options_description* configOptions();

  std::string instru1;
  std::string instru2;

  int spread_queue_size;

  double spread_ma_lower_boundary;
  double close_spread_lower_boundary;

  int max_wait_ticker_size;

  double delta_price;

  std::string log_cfg;

 private:
  boost::program_options::options_description config_options_;
};

class Config {
 public:
  explicit Config(int argc = 0, char* argv[] = nullptr);
  ~Config();

  Options* options() {
    return options_.get();
  }

  soil::Options* cataMDOptions() {
    return cata_md_options_.get();
  }

  soil::Options* cataTraderOptions() {
    return cata_trader_options_.get();
  }

 private:
  std::unique_ptr<Options> options_;

  std::unique_ptr<soil::Options> cata_md_options_;

  std::unique_ptr<soil::Options> cata_trader_options_;
};

}  // namespace moon

#endif
