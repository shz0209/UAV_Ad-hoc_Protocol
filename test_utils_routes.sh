g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*routes.cpp test_utils/test_utils_routes.cpp -o utils_routes 
./utils_routes
