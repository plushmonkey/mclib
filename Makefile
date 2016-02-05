CXXFLAGS=-std=c++11 -O2 -I./lib/utf8/include -I/usr/include/jsoncpp
LIBS=-lssl -lcrypto -lcurl -lz -ljsoncpp

SRC=$(wildcard mclib/*.cpp mclib/Entity/*.cpp mclib/Network/*.cpp mclib/Packets/*.cpp)

mcclient: $(SRC:.cpp=.o)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)

clean:
	-rm -f mclib/*.o
	-rm -f mclib/*/*.o
