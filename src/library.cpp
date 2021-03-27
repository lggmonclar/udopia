#include "library.h"
#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Packet.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "Serialization.h"
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

struct PacketA {
    int x, y, z, w;

    template <typename Stream>
    bool Serialize(Stream& stream) {
        SerializeInt(stream, x, 0, 32768);
        SerializeInt(stream, y, 0, 32768);
        SerializeInt(stream, z, 0, 32768);
        SerializeInt(stream, w, 0, 32768);
        return true;
    }
};

// get sockaddr, IPv4 or IPv6:
void* GetInAddr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


RUDP::RUDP() {
    WSADATA wsaData{};
    printf("Initializing RUDP...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialized.\n");
}

PacketA ReadPacket(SOCKET socket) {
    uint8_t buffer[MAX_PACKET_SIZE]{};
    struct sockaddr_storage remoteAddr{};
    socklen_t addrSize = sizeof(remoteAddr);
    PacketA packet;
    int numBytes;
    if((numBytes = recvfrom(socket, reinterpret_cast<char *>(buffer), MAX_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr*>(&remoteAddr), &addrSize)) == -1) {
        fprintf(stderr, "recvfrom error code: %d\n", WSAGetLastError());
        return packet;
    }

    char s[INET6_ADDRSTRLEN];
    printf("server: got packet from %s\n",
           inet_ntop(remoteAddr.ss_family, GetInAddr(reinterpret_cast<struct sockaddr *>(&remoteAddr)), s, sizeof(s)));
    printf("server: bytes received: \"%d\"\n", numBytes);

    ReadStream readStream(buffer, MAX_PACKET_SIZE);
    packet.Serialize(readStream);

    return packet;
}

void RUDP::CreateServer(const char *port) {
    // Used to specify which type of socket we want to use.
    struct addrinfo hints{};
    hints.ai_flags = AI_PASSIVE; // Fill in my IP for me
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    // A linked list of returned address infos, which will be filled below by getaddrinfo()
    struct addrinfo *serverInfo{};

    if(getaddrinfo(nullptr, port, &hints, &serverInfo) != 0) {
        fprintf(stderr, "getaddrinfo error code: %d\n", WSAGetLastError());
        return;
    }

    SOCKET serverSocket;
    // loop through all the results and bind to the first we can
    auto p = serverInfo;
    for(; p != nullptr; p = p->ai_next) {
        // Get the file descriptor from the kernel
        if ((serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            fprintf(stderr, "socket FD creation error code: %d\n", WSAGetLastError());
            continue;
        }

        // Bind the address to it
        if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == -1) {
            closesocket(serverSocket);
            fprintf(stderr, "bind error code: %d\n", WSAGetLastError());
            continue;
        }

        void*addr;
        char *ipver;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);

        break;
    }

    if (p == nullptr) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return;
    }


    freeaddrinfo(serverInfo);


    while(true) {
        PacketA packet = ReadPacket(serverSocket);
        printf("server: x: \"%d\"\n", packet.x);
        printf("server: y: \"%d\"\n", packet.y);
        printf("server: z: \"%d\"\n", packet.z);
        printf("server: w: \"%d\"\n", packet.w);
        fflush(stdout);
    }

    closesocket(serverSocket);
}

void RUDP::CreateClient(const char *host, const char *port, const unsigned int protocolId) {
    // Used to specify which type of socket we want to use.
    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    // A linked list of returned address infos, which will be filled below by getaddrinfo()
    struct addrinfo *serverInfo{};
    if(getaddrinfo(host, port, &hints, &serverInfo) != 0) {
        fprintf(stderr, "getaddrinfo: %d\n", WSAGetLastError());
        return;
    }

    SOCKET remoteSocket;

    // loop through all the results and make a socket
    auto p = serverInfo;
    for(; p != nullptr; p = p->ai_next) {
        if ((remoteSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            fprintf(stderr, "talker: socket error: %d\n", WSAGetLastError());
            continue;
        }
        break;
    }


    if (p == nullptr) {
        fprintf(stderr, "talker: failed to create socket\n");
        return;
    }


    uint8_t buffer[2048]{};
    while(true) {
        std::string strToSend;
        std::getline(std::cin, strToSend);

        WriteStream writeStream(buffer);
        PacketA packet{23, 21, 35, 4};
        packet.Serialize(writeStream);
        writeStream.Flush();


        int numBytes;
        if((numBytes = sendto(remoteSocket, reinterpret_cast<const char *>(buffer), 8, 0, p->ai_addr, p->ai_addrlen)) == -1) {
            fprintf(stderr, "talker: sendto error %d\n", WSAGetLastError());
            return;
        }
        printf("talker: sent %d bytes to %s\n", numBytes, host);
    }

    freeaddrinfo(serverInfo);
    closesocket(remoteSocket);
}

RUDP::~RUDP() {
    printf("Shutting down RUDP...\n");
    WSACleanup();
}
