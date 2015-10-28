CXXFLAGS=-std=c++11 -O2 -I./lib/utf8/include -I/usr/include/jsoncpp
LIBS=-lssl -lcrypto -lcurl -lz -ljsoncpp

SRC=$(wildcard mc/*.cpp mc/Entity/*.cpp mc/Network/*.cpp mc/Packets/*.cpp)

mcclient: $(SRC:.cpp=.o)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)

clean:
	-rm -f mc/*.o
