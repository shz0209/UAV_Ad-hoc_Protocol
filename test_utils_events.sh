g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*events.cpp test_utils/test_utils_events.cpp -o utils_events 
./utils_events
