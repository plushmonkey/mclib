#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_

#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "Socket.h"
#include "IPAddress.h"
#include "UDPSocket.h"
#include "TCPSocket.h"

namespace Network {

class NetworkInitializer {
private:
public:
    MCLIB_API NetworkInitializer();
    MCLIB_API ~NetworkInitializer();
};

class Dns {
public:
    static MCLIB_API IPAddresses Resolve(const std::string& host);
};

} // ns network

#endif
