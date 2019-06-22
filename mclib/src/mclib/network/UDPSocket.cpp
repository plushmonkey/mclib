#include <mclib/network/UDPSocket.h>

#include <mclib/common/DataBuffer.h>

#include <iostream>

#ifdef _WIN32
#define WOULDBLOCK WSAEWOULDBLOCK
#else
#define WOULDBLOCK EWOULDBLOCK
#endif

namespace mc {
namespace network {

UDPSocket::UDPSocket() : Socket(Socket::UDP), m_Port(0) {
    m_Handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

bool UDPSocket::Connect(const IPAddress& address, unsigned short port) {
    if (this->GetStatus() == Connected) return true;

    m_RemoteAddr.sin_port = htons(port);
    m_RemoteAddr.sin_family = AF_INET;
    m_RemoteAddr.sin_addr.s_addr = inet_addr(address.ToString().c_str());

    this->SetStatus(Connected);
    m_RemoteIP = IPAddress(address);
    m_Port = port;
    return true;
}

size_t UDPSocket::Send(const unsigned char* data, size_t size) {
    size_t sent = 0;

    if (this->GetStatus() != Connected) return 0;

    while (sent < size) {
        int cur = sendto(
            m_Handle, reinterpret_cast<const char*>(data + sent), size - sent,
            0, reinterpret_cast<sockaddr*>(&m_RemoteAddr), sizeof(sockaddr_in));
        if (cur <= 0) {
            Disconnect();
            return 0;
        }
        sent += cur;
    }

    return sent;
}

DataBuffer UDPSocket::Receive(std::size_t amount) {
    std::unique_ptr<char[]> buf(new char[amount]);
    socklen_t slen = sizeof(sockaddr_in);

    int received = recvfrom(m_Handle, buf.get(), amount, 0,
                            reinterpret_cast<sockaddr*>(&m_RemoteAddr), &slen);

    if (received <= 0) {
#if defined(_WIN32) || defined(WIN32)
        int err = WSAGetLastError();
#else
        int err = errno;
#endif
        if (err == WOULDBLOCK)
            return DataBuffer(std::string(buf.get(), received));

        Disconnect();
        return DataBuffer();
    }

    return DataBuffer(std::string(buf.get(), received));
}

}  // namespace network
}  // namespace mc
