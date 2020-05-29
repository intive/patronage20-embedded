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

//edit $PATH to avoid absolute filepath
type sudo gedit /etc/bash.bashtc - in text file put additional line at a bottom:
PATH=$PATH:<your path to the place where 'gateway' directory from repository is>
this will make absolute path is no more necessary
