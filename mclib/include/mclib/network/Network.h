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

#include <mclib/network/Socket.h>
#include <mclib/network/IPAddress.h>
#include <mclib/network/UDPSocket.h>
#include <mclib/network/TCPSocket.h>

namespace mc {
namespace network {

class Dns {
public:
    static MCLIB_API IPAddresses Resolve(const std::string& host);
};

} // ns network
} // ns mc

#endif
