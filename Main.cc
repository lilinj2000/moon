#include "MoonServer.hh"
#include "soil/Condition.hh"

int main(int argc, char* argv[])
{
  std::auto_ptr<moon::MoonServer> server(new moon::MoonServer(argc, argv));

  server->start();

  std::auto_ptr<soil::Condition> timer( soil::Condition::create() );

  while(true )
    timer->wait(5000);

  return 0;
}
