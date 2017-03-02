CXXFLAGS=-std=c++11 -Wall -fPIC -O2 -I./lib/utf8/include -I/usr/include/jsoncpp
LIBS=-lssl -lcrypto -lcurl -lz -ljsoncpp
CXX=clang++

SRC=$(wildcard mclib/*.cpp mclib/Entity/*.cpp mclib/Network/*.cpp mclib/Packets/*.cpp)

libmc.so: $(SRC:.cpp=.o)
	$(CXX) -shared -o $@ $(CXXFLAGS) $^ $(LIBS)

CLIENT_SRC=$(wildcard client/*.cpp)

mcclient: $(CLIENT_SRC:.cpp=.o)
	$(CXX) -o $@ $(CXXFLAGS) $^ -Wl,-rpath . -L. -lmc -ljsoncpp

clean:
	-rm -f client/*.o
	-rm -f mclib/*.o
	-rm -f mclib/*/*.o
