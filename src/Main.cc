// Copyright (c) 2010
// All rights reserved.

#include "Server.hh"
#include "soil/Pause.hh"

int main(int argc, char* argv[]) {
  std::unique_ptr<moon::Server> server;
  server.reset(new moon::Server(argc, argv));

  std::unique_ptr<soil::Pause> pause(soil::Pause::create());

  return 0;
}
