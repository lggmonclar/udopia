//
// Created by Lug on 26/03/2021.
//

#ifndef UDOPIA_CLIENT_H
#define UDOPIA_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdint>

#pragma comment(lib, "ws2_32.lib")

namespace Udopia {
    class Client {
    public:
        Client(const char* host, const char* port);
        void SendPacket(uint8_t* buffer);
        ~Client();
    private:
        struct addrinfo* remoteSocketAddrInfo;
        SOCKET remoteSocket{};
    };
}

#endif //UDOPIA_CLIENT_H