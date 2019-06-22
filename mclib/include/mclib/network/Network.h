#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_

#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <mclib/network/IPAddress.h>
#include <mclib/network/Socket.h>
#include <mclib/network/TCPSocket.h>
#include <mclib/network/UDPSocket.h>

namespace mc {
namespace network {

class Dns {
public:
    static MCLIB_API IPAddresses Resolve(const std::string& host);
};

}  // namespace network
}  // namespace mc

#endif
