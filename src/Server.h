//
// Created by Lug on 26/03/2021.
//

#ifndef UDOPIA_SERVER_H
#define UDOPIA_SERVER_H
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "ReadStream.h"
#include "Udopia.h"


#pragma comment(lib, "ws2_32.lib")

namespace Udopia {
    class Server {
    public:
        explicit Server(const char *port);
        ReadStream Listen() const;
        void SendPacket();
        ~Server();
    private:
        SOCKET serverSocket{};
    };
}
#endif //UDOPIA_SERVER_H
