#include <mclib/network/Socket.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/network/IPAddress.h>
#include <mclib/network/Network.h>

namespace mc {
namespace network {

Socket::Socket(Type type)
    : m_Handle(INVALID_SOCKET),
      m_Type(type),
      m_Blocking(false),
      m_Status(Disconnected) {}

Socket::~Socket() { Disconnect(); }

void Socket::SetBlocking(bool block) {
    unsigned long mode = block ? 0 : 1;

#ifdef _WIN32
    ioctlsocket(m_Handle, FIONBIO, &mode);
#else
    int opts = fcntl(m_Handle, F_GETFL);
    if (opts < 0) return;
    if (block)
        opts |= O_NONBLOCK;
    else
        opts &= ~O_NONBLOCK;
    fcntl(m_Handle, F_SETFL, opts);
#endif

    m_Blocking = block;
}

bool Socket::IsBlocking() const noexcept { return m_Blocking; }

Socket::Type Socket::GetType() const noexcept { return m_Type; }

SocketHandle Socket::GetHandle() const noexcept { return m_Handle; }

void Socket::SetStatus(Socket::Status status) { m_Status = status; }

Socket::Status Socket::GetStatus() const noexcept { return m_Status; }

bool Socket::Connect(const std::string& ip, uint16_t port) {
    IPAddress addr(ip);
    return Connect(addr, port);
}

std::size_t Socket::Send(const std::string& data) {
    return this->Send(reinterpret_cast<const unsigned char*>(data.c_str()),
                      data.length());
}

std::size_t Socket::Send(DataBuffer& buffer) {
    std::string data = buffer.ToString();
    return this->Send(reinterpret_cast<const unsigned char*>(data.c_str()),
                      data.length());
}

void Socket::Disconnect() {
    if (m_Handle != INVALID_SOCKET) closesocket(m_Handle);
    m_Status = Disconnected;
}

}  // namespace network
}  // namespace mc
