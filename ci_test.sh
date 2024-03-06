echo '------------------------------------------------------------'
echo "Test script: test.sh"
bash test.sh

echo '------------------------------------------------------------'
echo "g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*.cpp test_utils/test_utils_events.cpp -o utils_events"
g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*.cpp test_utils/test_utils_events.cpp -o utils_events

echo '------------------------------------------------------------'
echo "All test passed."
echo "You can merge this pull request."