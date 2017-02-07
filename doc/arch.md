Files
------
Config.hh/.cc
Log.hh
Main.cc

Context.hh/.cc
State.hh/.cc
Order.hh/.cc
Tick.hh/.cc
Server.hh/.cc


Classes
--------

Server  .--------> Config   .------> Options
        .--------> Tick  ------------->|
        .--------> Order <------------>|
        .--------> Context  .------> State

Threads
--------

1. MD thread

2. Request order thread

3. Rtn order & Rtn Trade thread


