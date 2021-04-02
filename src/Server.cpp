//
// Created by Lug on 26/03/2021.
//
#include <cstdio>
#include <cstdint>
#include "Server.h"
#include "Packet.h"

namespace Udopia {
    Server::Server(const char *port) {
        // Used to specify which type of socket we want to use.
        struct addrinfo hints{};
        hints.ai_flags = AI_PASSIVE; // Fill in my IP for me
        hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM; // UDP socket

        // A linked list of returned address infos, which will be filled below by getaddrinfo()
        struct addrinfo *serverInfo{};
        if(getaddrinfo(nullptr, port, &hints, &serverInfo) != 0) {
            fprintf(stderr, "server: getaddrinfo error code: %d\n", WSAGetLastError());
            return;
        }

        // loop through all the results and bind to the first we can
        auto p = serverInfo;
        for(; p != nullptr; p = p->ai_next) {
            // Get the file descriptor from the kernel
            if ((serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                fprintf(stderr, "server: socket FD creation error code: %d\n", WSAGetLastError());
                continue;
            }

            // Bind the address to it
            if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == -1) {
                closesocket(serverSocket);
                fprintf(stderr, "server: bind error code: %d\n", WSAGetLastError());
                continue;
            }

            void *addr;
            char *ipver;
            if (p->ai_family == AF_INET) { // IPv4
                auto *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            } else { // IPv6
                auto *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }

            // convert the IP to a string and print it:
            char ipstr[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            printf("Bound socket with %s: %s\n", ipver, ipstr);

            break;
        }

        if (p == nullptr) {
            fprintf(stderr, "server: failed to bind socket\n");
            return;
        }
        freeaddrinfo(serverInfo);
    }

    void Server::SendPacket() {

    }

    ReadStream Server::Listen() const {
        uint8_t buffer[MAX_PACKET_SIZE]{};
        ReadStream readStream(buffer, MAX_PACKET_SIZE);

        struct sockaddr_storage remoteAddr{};
        socklen_t addrSize = sizeof(remoteAddr);
        int numBytes;
        if((numBytes = recvfrom(serverSocket, reinterpret_cast<char *>(buffer), MAX_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr*>(&remoteAddr), &addrSize)) == -1) {
            fprintf(stderr, "recvfrom error code: %d\n", WSAGetLastError());
            return readStream;
        }

        char s[INET6_ADDRSTRLEN];
        printf("server: got packet from %s\n",
               inet_ntop(remoteAddr.ss_family, Utils::GetInAddr(reinterpret_cast<struct sockaddr *>(&remoteAddr)), s, sizeof(s)));
        printf("server: bytes received: \"%d\"\n", numBytes);
        return readStream;
    }

    Server::~Server() {
        printf("Shutting down Server...\n");
        closesocket(serverSocket);
    }

}