#include <memory>

#include "gtest/gtest.h"
#include "Config.hh"
#include "MoonLog.hh"

namespace moon
{

class ConfigTest : public ::testing::Test
{
public:
  ConfigTest()
  {
    MOON_LOG_INIT("log.cfg");
  }
  
  void SetUp()
  {
  }

  void TearDown()
  {
  }

 protected:
};

TEST_F(ConfigTest, defaultConfigTest)
{
  Config cfg;

  ASSERT_EQ( cfg.logCfg(), "./log.cfg" );

  const MDServiceCfg& md_cfg = cfg.mdServiceCfg();
  
  ASSERT_EQ( md_cfg.flow_path_, "./mdflow/" );
  ASSERT_EQ( md_cfg.front_address_, "tcp://180.168.146.181:10210" );
  ASSERT_EQ( md_cfg.broker_id_, "0148" );
  ASSERT_EQ( md_cfg.user_id_, "00095" );
  ASSERT_EQ( md_cfg.password_, "888888" );

  const TraderServiceCfg& trader_cfg = cfg.traderServiceCfg();
  
  ASSERT_EQ( trader_cfg.flow_path_, "./traderflow/" );
  ASSERT_EQ( trader_cfg.front_address_, "tcp://180.168.146.181:10200" );
  ASSERT_EQ( trader_cfg.broker_id_, "0148" );
  ASSERT_EQ( trader_cfg.user_id_, "00095" );
  ASSERT_EQ( trader_cfg.password_, "888888" );

  const StrategyCfg& strategy_cfg = cfg.strategyCfg();
  ASSERT_EQ( strategy_cfg.instru1_, "IF1505" );
  ASSERT_EQ( strategy_cfg.instru2_, "IF1502" );
  ASSERT_EQ( strategy_cfg.spread_queue_size_, 100 );
  ASSERT_EQ( strategy_cfg.spread_ma_lower_boundary_, 2.5 );
  ASSERT_EQ( strategy_cfg.close_spread_lower_boundary_, 0.6 );
  ASSERT_EQ( strategy_cfg.max_wait_ticker_size_, 500 );

}

}  // namespace moon
