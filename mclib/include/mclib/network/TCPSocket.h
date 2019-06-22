#ifndef NETWORK_TCP_SOCKET_H_
#define NETWORK_TCP_SOCKET_H_

#include <mclib/network/IPAddress.h>
#include <mclib/network/Socket.h>

#include <cstdint>

namespace mc {
namespace network {

class TCPSocket : public Socket {
private:
    IPAddress m_RemoteIP;
    uint16_t m_Port;
    sockaddr_in m_RemoteAddr;

public:
    MCLIB_API TCPSocket();

    bool MCLIB_API Connect(const IPAddress& address, uint16_t port);
    std::size_t MCLIB_API Send(const u8* data, std::size_t size);
    DataBuffer MCLIB_API Receive(std::size_t amount);
    std::size_t MCLIB_API Receive(DataBuffer& buffer, std::size_t amount);
};

}  // namespace network
}  // namespace mc

#endif
