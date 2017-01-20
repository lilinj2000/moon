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

  std::string tick_sub_addr;
  std::string order_sub_addr;
  std::string push_addr;

  int basis_queue_size;
  double basis_delta_open;
  double profit_close;
  double fee_per_volume;
  double fee_per_money;
  double volume_multiple;

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

 private:
  std::unique_ptr<Options> options_;
};

}  // namespace moon

#endif
