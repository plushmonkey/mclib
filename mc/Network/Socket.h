#ifndef NETWORK_SOCKET_H_
#define NETWORK_SOCKET_H_

#include "../Types.h"
#include "../DataBuffer.h"
#include <string>
#include <vector>
#include <memory>

namespace Network {

class IPAddress;

typedef Minecraft::DataBuffer DataBuffer;

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
    virtual ~Socket();

    void SetBlocking(bool block);
    bool IsBlocking() const;

    Type GetType() const;
    Status GetStatus() const;
    SocketHandle GetHandle() const;

    bool Connect(const std::string& ip, u16 port);
    virtual bool Connect(const IPAddress& address, u16 port) = 0;

    void Disconnect();

    std::size_t Send(const std::string& data);
    std::size_t Send(DataBuffer& buffer);

    virtual std::size_t Send(const uint8_t* data, std::size_t size) = 0;
    virtual DataBuffer Receive(std::size_t amount) = 0;
};

typedef std::shared_ptr<Socket> SocketPtr;

} // ns network

#endif

