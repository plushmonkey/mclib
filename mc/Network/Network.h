#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_

#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#error TODO: Linux network stuff
#endif

#include "Socket.h"
#include "IPAddress.h"
#include "UDPSocket.h"
#include "TCPSocket.h"

namespace Network {

class NetworkInitializer {
private:
public:
    NetworkInitializer();
    ~NetworkInitializer();
};

class Dns {
public:
    static IPAddresses Resolve(const std::string& host);
};

} // ns network

#endif
