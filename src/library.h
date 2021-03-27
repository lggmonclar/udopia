#ifndef RUDP_LIBRARY_H
#define RUDP_LIBRARY_H

#include <string>

struct RUDP {
    RUDP();
    void CreateServer(const char* port);
    void CreateClient(const char* host, const char* port, unsigned int protocolId);
    ~RUDP();
};

#endif //RUDP_LIBRARY_H
