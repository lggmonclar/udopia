#ifndef RUDP_LIBRARY_H
#define RUDP_LIBRARY_H

#include <string>
#include "Serialization.h"
#include "Client.h"
#include "Server.h"


namespace Udopia {
    void Startup();
    void Cleanup();
}

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

#endif //RUDP_LIBRARY_H
