Compilation in g++:
g++ -O3 -std=c++11 gateway.cpp -lboost_thread -lboost_system -pthread -o server

With ssl on:
g++ -O3 -std=c++11 gateway.cpp -Wall -g -I/usr/include/boost -pthread -lcrypto -lssl -lboost_system -o server

Dependencies:
sudo apt-get install libboost-all-dev

//this ones i'm not sure because program wouln't compile before i installed these, but whatever :D
sudo apt-get install libasio-dev
sudo apt-get install libssl-dev
sudo apt-get install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

Endpoints:
http://localhost:8080/dashboard 
http://localhost:8080/hello
