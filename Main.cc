#include "MoonServer.hh"
#include "soil/STimer.hh"

int main(int argc, char* argv[])
{
  std::unique_ptr<moon::MoonServer> server(new moon::MoonServer(argc, argv));

  server->start();

  std::unique_ptr<soil::STimer> timer( soil::STimer::create() );

  while(true )
    timer->wait(5000);

  return 0;
}
