#ifndef NETWORK_UDP_SOCKET_H_
#define NETWORK_UDP_SOCKET_H_

#include "Socket.h"

#include "IPAddress.h"

#include <cstdint>

namespace Network {

class UDPSocket : public Socket {
private:
    IPAddress m_RemoteIP;
    uint16_t m_Port;
    sockaddr_in m_RemoteAddr;

public:
    UDPSocket();

    bool MCLIB_API Connect(const IPAddress& address, uint16_t port);
    std::size_t MCLIB_API Send(const u8* data, std::size_t size);
    DataBuffer MCLIB_API Receive(std::size_t amount);
};

} // ns network

#endif
