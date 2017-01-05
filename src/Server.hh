// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_SERVER_HH
#define MOON_SERVER_HH

#include <memory>
#include "Config.hh"
#include "Index.hh"
#include "subject/Service.hh"

namespace moon {

class Server : public subject::ServiceCallback {
 public:
  Server(int argc, char* argv[]);

  ~Server();

  virtual void onMessage(const std::string& msg);

 private:
  std::unique_ptr<Config> config_;

  std::unique_ptr<subject::Service> subject_service_;
  std::unique_ptr<Index> index_;
};

};  // namespace moon

#endif
