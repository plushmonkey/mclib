#ifndef NETWORK_SOCKET_H_
#define NETWORK_SOCKET_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/common/Types.h>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define closesocket close
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

namespace mc {
namespace network {

class IPAddress;

using mc::DataBuffer;

typedef int SocketHandle;

class Socket {
public:
    enum Status { Connected, Disconnected, Error };
    enum Type { TCP, UDP };

private:
    bool m_Blocking;
    Type m_Type;
    Status m_Status;

protected:
    SocketHandle m_Handle;

    Socket(Type type);
    void SetStatus(Status status);

public:
    virtual MCLIB_API ~Socket();

    Socket(Socket&& rhs) = default;
    Socket& operator=(Socket&& rhs) = default;

    void MCLIB_API SetBlocking(bool block);
    bool MCLIB_API IsBlocking() const noexcept;

    Type MCLIB_API GetType() const noexcept;
    Status MCLIB_API GetStatus() const noexcept;
    SocketHandle MCLIB_API GetHandle() const noexcept;

    bool MCLIB_API Connect(const std::string& ip, u16 port);
    virtual bool Connect(const IPAddress& address, u16 port) = 0;

    void MCLIB_API Disconnect();

    std::size_t MCLIB_API Send(const std::string& data);
    std::size_t MCLIB_API Send(DataBuffer& buffer);

    virtual std::size_t Send(const uint8_t* data, std::size_t size) = 0;
    virtual DataBuffer Receive(std::size_t amount) = 0;

    virtual std::size_t Receive(DataBuffer& buffer, std::size_t amount) = 0;
};

typedef std::shared_ptr<Socket> SocketPtr;

}  // namespace network
}  // namespace mc

#endif
