Compilation in g++:
g++ -O3 -std=c++11 gateway.cpp -lboost_thread -lboost_system -pthread -o server

Crow requires Boost to work:
sudo apt-get install libboost-all-dev

Endpoints:
http://localhost:8080/dashboard 
http://localhost:8080/hello
