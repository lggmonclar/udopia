//
// Created by Lug on 26/03/2021.
//

#ifndef UDOPIA_SERVER_H
#define UDOPIA_SERVER_H
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "ReadStream.h"

#pragma comment(lib, "ws2_32.lib")

namespace Udopia {
    class Server {
    public:
        explicit Server(const char *port): serverSocket(InitSocket(port)) {}
        ReadStream Listen();
        void SendPacket();
        ~Server();
    private:
        const SOCKET serverSocket{};
        static SOCKET InitSocket(const char *port);
    };
}
#endif //UDOPIA_SERVER_H
