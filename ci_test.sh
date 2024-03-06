echo '------------------------------------------------------------'
echo "Test script: test.sh"
bash test.sh

echo '------------------------------------------------------------'
echo "Test script: g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*.cpp test_utils/test_utils_events.cpp -o utils_events"
g++  -I/usr/include/libnl3 -lnl-route-3 -lnl-3 -std=c++11 src/*.cpp test_utils/test_utils_events.cpp -o utils_events
if [ $? -eq 0 ]; then
    echo "Compilation successful"
else
    echo "Compilation failed"
    exit 1
fi

echo '------------------------------------------------------------'
echo "All test passed."
echo "You can merge this pull request."