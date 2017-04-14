CXXFLAGS=-std=c++14 -Wall -fPIC -O2 -I./mclib/include -I./lib/utf8/include -I/usr/include/jsoncpp
LIBS=-lssl -lcrypto -lcurl -lz -ljsoncpp
CXX=clang++

SRC=$(shell find mclib -type f -name *.cpp)

libmc.so: $(SRC:.cpp=.o)
	$(CXX) -shared -o $@ $(CXXFLAGS) $^ $(LIBS)

CLIENT_SRC=$(wildcard client/*.cpp)

mcclient: $(CLIENT_SRC:.cpp=.o)
	$(CXX) -o $@ $(CXXFLAGS) $^ -Wl,-rpath,. -L. -lmc -ljsoncpp

clean:
	-rm -f client/*.o
	-rm -f mclib/*.o
	-rm -f mclib/*/*.o
