#include "Network.h"

namespace {

Network::NetworkInitializer initializer;

} // ns

namespace Network {

#ifdef _WIN32
    NetworkInitializer::NetworkInitializer() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    NetworkInitializer::~NetworkInitializer() {
        WSACleanup();
    }
#else
    NetworkInitializer::NetworkInitializer() {

    }
    NetworkInitializer::~NetworkInitializer() {

    }
#endif

IPAddresses Dns::Resolve(const std::string& host) {
    IPAddresses list;
    addrinfo hints = { 0 }, *addresses;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo(host.c_str(), NULL, &hints, &addresses);

    for (addrinfo *p = addresses; p != NULL; p = p->ai_next) {

#ifdef _WIN32
        //wchar_t straddr[35];
        char straddr[35];
        DWORD len;
        
        WSAAddressToStringA(p->ai_addr, p->ai_addrlen, NULL, straddr, &len);
#else
        char straddr[35];

        inet_ntop(p->ai_family, &((sockaddr_in*)p->ai_addr)->sin_addr, straddr, sizeof(straddr));
#endif

        list.push_back(IPAddress(straddr));
    }

    return list;
}

} // ns network
