#include "TCPSocket.h"

#include "../DataBuffer.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#ifdef _WIN32
#define WOULDBLOCK WSAEWOULDBLOCK
#else
#define WOULDBLOCK EWOULDBLOCK
#endif

namespace Network {

TCPSocket::TCPSocket()
    : Socket(Socket::TCP)
{
    m_Handle = INVALID_SOCKET;
}

bool TCPSocket::Connect(const IPAddress& address, unsigned short port) {
    if (this->GetStatus() == Connected)
        return true;

    struct addrinfo hints = { 0 }, *result = nullptr;

    if ((m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return false;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address.ToString().c_str(), std::to_string(port).c_str(), &hints, &result) != 0)
        return false;

    struct addrinfo* ptr = nullptr;
    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        struct sockaddr_in* sockaddr = (struct sockaddr_in*)ptr->ai_addr;
        if (::connect(m_Handle, (struct sockaddr*)sockaddr, sizeof(struct sockaddr_in)) != 0)
            continue;
        m_RemoteAddr = *sockaddr;
        break;
    }

    if (!ptr) return false;

    this->SetStatus(Connected);
    m_RemoteIP = address;
    m_Port = port;
    return true;
}

size_t TCPSocket::Send(const unsigned char* data, size_t size) {
    if (this->GetStatus() != Connected)
        return 0;

    size_t sent = 0;

    while (sent < size) {
        int cur = ::send(m_Handle, reinterpret_cast<const char*>(data + sent), size - sent, 0);
        if (cur <= 0) {
            Disconnect();
            return 0;
        }
        sent += cur;
    }

    return sent;
}

DataBuffer TCPSocket::Receive(std::size_t amount) {
    std::unique_ptr<char[]> buf(new char[amount]);

    int received = ::recv(m_Handle, buf.get(), amount, 0);

    if (received <= 0) {
#if defined(_WIN32) || defined(WIN32)
        int err = WSAGetLastError();
#else
        int err = errno;
#endif
        if (err == WOULDBLOCK || err == WSAEMSGSIZE)
            return DataBuffer();

        Disconnect();
        return DataBuffer();
    }

    return DataBuffer(std::string(buf.get(), received));
}

} // ns network
