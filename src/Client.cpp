//
// Created by Lug on 26/03/2021.
//

#include <cstdio>
#include "Client.h"

namespace Udopia {
    SOCKET Client::InitSocket(const char *host, const char *port) {
        WSADATA wsaData{};
        printf("Initializing Client...\n");
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("Failed. Error Code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("Initialized.\n");

        SOCKET remoteSocket{};

        // Used to specify which type of socket we want to use.
        struct addrinfo hints{};
        hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM; // UDP socket

        // A linked list of returned address infos, which will be filled below by getaddrinfo()
        struct addrinfo *availableAddresses{};
        if(getaddrinfo(host, port, &hints, &availableAddresses) != 0) {
            fprintf(stderr, "getaddrinfo: %d\n", WSAGetLastError());
            return remoteSocket;
        }

        // loop through all the results and make a socket
        auto p = availableAddresses;
        for(; p != nullptr; p = p->ai_next) {
            if ((remoteSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                fprintf(stderr, "client: socket error: %d\n", WSAGetLastError());
                continue;
            }
            break;
        }

        if (p == nullptr) {
            fprintf(stderr, "client: failed to create socket\n");
            return remoteSocket;
        }
        remoteSocketAddrInfo = p;

        return remoteSocket;
    }

    void Client::SendPacket(uint8_t *buffer) {
        int numBytes;
        if((numBytes = sendto(remoteSocket, reinterpret_cast<const char *>(buffer), 8, 0, remoteSocketAddrInfo->ai_addr, remoteSocketAddrInfo->ai_addrlen)) == -1) {
            fprintf(stderr, "client: sendto error %d\n", WSAGetLastError());
            return;
        }
        printf("client: sent %d bytes\n", numBytes);
    }

    Client::~Client() {
        freeaddrinfo(remoteSocketAddrInfo);
        closesocket(remoteSocket);
        WSACleanup();
    }
}
