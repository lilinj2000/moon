#include <memory>

#include "gtest/gtest.h"
#include "MoonServer.hh"
#include "MoonLog.hh"

#include "soil/boost.hh"
#include "soil/Condition.hh"

namespace moon
{

class MoonServerTest : public ::testing::Test
{
public:
  MoonServerTest()
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

  soil::Condition timer_;
  
};

TEST_F(MoonServerTest, startMoonServerTest)
{
  std::auto_ptr<MoonServer> server( new MoonServer(0, NULL) );

  server->start();
  
  timer_.wait(100000);

  server->stop();

  ASSERT_TRUE( true );
}

}  // namespace moon
