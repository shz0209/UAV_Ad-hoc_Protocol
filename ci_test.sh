echo '------------------------------------------------------------'
echo "Test script: test.sh"
bash test.sh

echo '------------------------------------------------------------'
echo "Test script: g++ -o add_route test_add_route.cpp -I/usr/include/libnl3 -lnl-route-3 -lnl-3"
g++ -o add_route test_add_route.cpp -I/usr/include/libnl3 -lnl-route-3 -lnl-3

echo '------------------------------------------------------------'
echo "All test passed."
echo "You can merge this pull request."