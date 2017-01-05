// Copyright (c) 2010
// All rights reserved.

#ifndef MOON_INDEX_HH
#define MOON_INDEX_HH

#include <queue>
#include "soil/MsgQueue.hh"

namespace moon {

class Server;

typedef struct {
  std::string instru;
  std::string time_stamp;
  double bid_price1;
  double ask_price1;
}MDInfo;

class Index {
 public:
  Index(Server *,
        const std::string& instru1,
        const std::string& instru2);

  ~Index();

  void msgCallback(const std::string *);

  void pushMsg(std::string* msg) {
    md_queue_->pushMsg(msg);
  }

 private:
  Server* server_;

  std::string instru1_;
  std::string instru2_;

  std::unique_ptr<MDInfo> md_instru1_;
  std::unique_ptr<MDInfo> md_instru2_;

  std::unique_ptr<soil::MsgQueue<std::string, Index> > md_queue_;

  std::queue<double> ma1_queue_;
  std::queue<double> ma2_queue_;
};
  
} // namespace moon

#endif
