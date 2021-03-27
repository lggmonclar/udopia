//
// Created by Lug on 26/03/2021.
//
#ifndef RUDP_BITREADER_H
#define RUDP_BITREADER_H
#include <cstdint>
#include <WS2tcpip.h>
#include "Utils.h"

namespace Udopia {
    class BitReader {
    private:
        uint64_t scratch{};
        int scratchBits{};
        int totalBits{};
        int bitsRead{};
        int wordIndex{};
        uint32_t * buffer{};
    public:
        BitReader(uint8_t *buffer, int size) : buffer(reinterpret_cast<uint32_t *>(buffer)), totalBits(size * 8) {}
        BitReader(uint32_t *buffer, int size) : buffer(buffer), totalBits(size * 8) {}

        bool WouldReadPastEnd(int bits) const {
            return (bitsRead + bits) > totalBits;
        }

        uint32_t ReadBits(int bits) {
            if(scratchBits < bits) {
                //If there are less bits available in the scratch memory than what was requested, we must load another word
                scratch |= Utils::NetworkToHost(buffer[wordIndex++]) << scratchBits;
                scratchBits += 32;
            }

            // Get only the bits that we care about
            uint32_t val = scratch << (64 - bits) >> (64 - bits);

            // Discard the bits that were read from the scratch memory
            scratch >>= bits;
            scratchBits -= bits;

            //Increment the number of bits read so we can know if a read would attempt to go past the maximum size of the buffer
            bitsRead += bits;

            return val;
        }
    };
}

#endif //RUDP_BITREADER_H
