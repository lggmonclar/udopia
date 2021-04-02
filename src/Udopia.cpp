//
// Created by Lug on 02/04/2021.
//

#include "Udopia.h"
#include "Client.h"
#include "Server.h"

namespace Udopia {
    void Startup() {
        WSADATA wsaData{};
        printf("Initializing Udopia...\n");
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("Failed to initialize Udopia. Error Code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("Initialized.\n");
    }

    void Cleanup() {
        WSACleanup();
    }
}