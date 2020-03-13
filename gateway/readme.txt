Kompilacja g++:
g++ -O3 -std=c++11 gateway.cpp -lboost_thread -lboost_system -pthread -o server

Crow wymaga bibliotek Boosta do działania. Pakiet:
sudo apt-get install libboost-all-dev

Endpointy:
http://localhost:8080/dashboard 
http://localhost:8080/hello
