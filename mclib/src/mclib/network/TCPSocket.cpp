#include <mclib/network/TCPSocket.h>

#include <mclib/common/DataBuffer.h>

#include <iostream>

#ifdef _WIN32
#define WOULDBLOCK WSAEWOULDBLOCK
#define MSG_DONTWAIT 0
#else
#define WOULDBLOCK EWOULDBLOCK
#endif

namespace mc {
namespace network {

TCPSocket::TCPSocket() : Socket(Socket::TCP), m_Port(0) {
    m_Handle = INVALID_SOCKET;
}

bool TCPSocket::Connect(const IPAddress& address, unsigned short port) {
    if (this->GetStatus() == Connected) return true;

    struct addrinfo hints = {0}, *result = nullptr;

    if ((m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return false;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address.ToString().c_str(), std::to_string(port).c_str(),
                    &hints, &result) != 0)
        return false;

    struct addrinfo* ptr = nullptr;
    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        struct sockaddr_in* sockaddr = (struct sockaddr_in*)ptr->ai_addr;
        if (::connect(m_Handle, (struct sockaddr*)sockaddr,
                      sizeof(struct sockaddr_in)) != 0)
            continue;
        m_RemoteAddr = *sockaddr;
        break;
    }

    freeaddrinfo(result);

    if (!ptr) return false;

    this->SetStatus(Connected);
    m_RemoteIP = address;
    m_Port = port;
    return true;
}

size_t TCPSocket::Send(const unsigned char* data, size_t size) {
    if (this->GetStatus() != Connected) return 0;

    size_t sent = 0;

    while (sent < size) {
        int cur = ::send(m_Handle, reinterpret_cast<const char*>(data + sent),
                         size - sent, 0);
        if (cur <= 0) {
            Disconnect();
            return 0;
        }
        sent += cur;
    }

    return sent;
}

std::size_t TCPSocket::Receive(DataBuffer& buffer, std::size_t amount) {
    buffer.Resize(amount);
    buffer.SetReadOffset(0);

    int recvAmount = recv(m_Handle, (char*)&buffer[0], amount, MSG_DONTWAIT);
    if (recvAmount <= 0) {
#if defined(_WIN32) || defined(WIN32)
        int err = WSAGetLastError();
#else
        int err = errno;
#endif
        if (err == WOULDBLOCK) {
            buffer.Clear();
            return 0;
        }

        Disconnect();
        buffer.Clear();
        return 0;
    }
    buffer.Resize(recvAmount);
    return recvAmount;
}

DataBuffer TCPSocket::Receive(std::size_t amount) {
    std::unique_ptr<char[]> buf(new char[amount]);

    int received = ::recv(m_Handle, buf.get(), amount, MSG_DONTWAIT);

    if (received <= 0) {
#if defined(_WIN32) || defined(WIN32)
        int err = WSAGetLastError();
#else
        int err = errno;
#endif
        if (err == WOULDBLOCK) return DataBuffer();

        Disconnect();
        return DataBuffer();
    }

    return DataBuffer(std::string(buf.get(), received));
}

}  // namespace network
}  // namespace mc
