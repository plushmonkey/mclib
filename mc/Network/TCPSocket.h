#ifndef NETWORK_TCP_SOCKET_H_
#define NETWORK_TCP_SOCKET_H_

#include "Socket.h"

#include "IPAddress.h"

#include <cstdint>
#include <WinSock2.h>

namespace Network {

class TCPSocket : public Socket {
private:
    IPAddress m_RemoteIP;
    uint16_t m_Port;
    sockaddr_in m_RemoteAddr;

public:
    TCPSocket();

    bool Connect(const IPAddress& address, uint16_t port);
    std::size_t Send(const u8* data, std::size_t size);
    DataBuffer Receive(std::size_t amount);
};

} // ns network

#endif
