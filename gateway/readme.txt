Compilation in g++:
g++ -O3 -std=c++11 gateway.cpp -lboost_thread -lboost_system -pthread -o server

With ssl on:
g++ -O3 -std=c++11 gateway.cpp -Wall -g - -pthread -lssl -lcrypto -lboost_system -o server

And static (will yield some warnings but that's OK):
g++ -O3 -std=c++11 gateway.cpp -Wall -g --static -pthread -lssl -lcrypto -lboost_system -ldl -o server

And with MQTT lib:
g++ -O3 -std=c++11 gateway.cpp  -I ./dependencies/include/ -L ./dependencies/lib/ -B ./dependencies/lib/ -lpaho-mqtt3cs -o server

Dependencies:
sudo apt-get install libboost-all-dev

sudo ln -s ./dependencies/lib/libpaho-mqtt3cs.so.1 /usr/local/lib/libpaho-mqtt3cs.so.1

//this ones i'm not sure because program wouln't compile before i installed these, but whatever :D
sudo apt-get install libasio-dev
sudo apt-get install libssl-dev
sudo apt-get install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

