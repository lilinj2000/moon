
States list
-----------
1. short\_postion\_without\_order

2. short\_postion\_with\_order

3. position\_with\_order

4. position\_without\_order

1st: trigger open position
    trigger open       part order be traded	   all orders be traded
1  --------------> 2   ---------------------> 3   ---------------------->4
                      | all orders be traded without postion      rtn trade
                       --------------------------------------> 1 ------------> 4
1: receive md,   
   decide whether trigger open position,   
       if open position happened, the state will be changed to 2   
   then, update md info
1: should no rtn order
1: receive rtn trade with position, the state will be changed to 4

2: receive md, and update md info
2: receive rtn order, and update order info
2: receive rtn trade, and update position info, 
   if the part order be traded, the state will be changed to 3

3: receive md, and update md info
3: receive rtn order, and update order info
   if all orders be traded, the state will be changed to 4
3: receive rtn trade, and update position info, 


2nd: trigger close position

   trigger close      no position with order    all order be traded           
4  -------------> 3  ----------------------> 2 ---------------------> 1
                  |   all orders be traded with position       no position without order
		   --------------------------------------> 4  ---------------------------> 1
4: receive md, 
   decide whether trigger close position, 
       if close position happened, the state will be changed to 3
   then, update md info
4: should no rtn order 
4: receive rtn trade, if no position, the state will be changed to 1

3: receive md, and update md info
3: receive rtn order, and update order info
   if all the orders be traded and with position,the state will be changed to 4
3: receive rtn trade, and update position info, 
   if no position with dorder, the state will be changed to 2

2: receive md, and update md info
2: receive rtn order, and update order info
   if all orders be traded, the state will be changed to 1
3: no rtn trade happed.



Summary
--------
  <-------------------------------->
/                                  \
1  <------> 2  <-------> 3  <-----> 4


    --------------------------> 4
  /
1 ------------> 2


    ------------> 1
  /
2 -----------> 3


    ------------> 2
  /
3 -----------> 4

    --------------------------> 1
  /
4 ------------> 3


Limitation
-----------
1. only support instrument of shfe, and only 1 volume

2. all the orders will be traded successful