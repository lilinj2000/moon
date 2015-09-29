#ifndef MOON_CONFIG_HH
#define MOON_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace moon
{

namespace po = boost::program_options;

class MoonOptions : public soil::Options
{
 public:

  MoonOptions();
  
  virtual ~MoonOptions();

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

class MoonConfig
{
 public:
  
  MoonConfig(int argc=0, char* argv[]=NULL);
  ~MoonConfig();

  MoonOptions* moonOptions()
  {
    return moon_options_.get();
  }

  soil::Options* cataMDOptions()
  {
    return cata_md_options_.get();
  }

  soil::Options* cataTraderOptions()
  {
    return cata_trader_options_.get();
  }

 private:
  std::auto_ptr<MoonOptions> moon_options_;
  
  std::auto_ptr<soil::Options> cata_md_options_;

  std::auto_ptr<soil::Options> cata_trader_options_;
};

}  


#endif 
