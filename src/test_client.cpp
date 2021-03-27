//
// Created by Lug on 17/03/2021.
//
#include <iostream>
#include "Client.h"
#include "Serialization.h"
#include "WriteStream.h"
#include "Udopia.h"

#define BUF_SIZE 2048

int main() {
    Udopia::Client client("localhost", "8080");


    while(true) {
        std::string strToSend;
        std::getline(std::cin, strToSend);

        int x, y, z, w;
        x = 255 * rand() / (RAND_MAX + 1.0);
        y = 255 * rand() / (RAND_MAX + 1.0);
        z = 255 * rand() / (RAND_MAX + 1.0);
        w = 255 * rand() / (RAND_MAX + 1.0);


        uint8_t buffer[2048];
        Udopia::WriteStream stream(buffer);
        PacketA packet{x, y, z, w};
        printf("client: x: %d\n", packet.x);
        printf("client: y: %d\n", packet.y);
        printf("client: z: %d\n", packet.z);
        printf("client: w: %d\n", packet.w);

        packet.Serialize(stream);

        client.SendPacket(buffer);
    }

    return 0;
}



