//
// Created by Lug on 17/03/2021.
//
#include "library.h"
#include "Serialization.h"
#include "WriteStream.h"
#include "ReadStream.h"

#define BUF_SIZE 2048

int main() {
    RUDP rudp;
    rudp.CreateClient("localhost", "8080", 55);

    return 0;
}



