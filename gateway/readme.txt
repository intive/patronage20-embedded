Compilation in g++:

g++ -std=c++11 -lboost_system -pthread -lstdc++ -lssl -lcrypto -lmosquitto -o server

or

type: 'make' in terminal


//Libs

sudo apt-get install libboost-all-dev
sudo apt-get install libmosquitto-dev 
sudo apt-get install mosquitto-dev

//this ones i'm not sure because program wouln't compile before i installed these, but whatever :D
sudo apt-get install libasio-dev
sudo apt-get install libssl-dev
sudo apt-get install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

