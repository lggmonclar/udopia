//
// Created by Lug on 17/03/2021.
//
#include "Udopia.h"

int main() {
    Udopia::Startup();

    Udopia::Server server("8080");

    while(true) {
        auto stream = server.Listen();
        PacketA packet{};

        packet.Serialize(stream);
        printf("server: x: %d\n", packet.x);
        printf("server: y: %d\n", packet.y);
        printf("server: z: %d\n", packet.z);
        printf("server: w: %d\n", packet.w);
        fflush(stdout);
    }

    Udopia::Cleanup();

    return 0;
}
