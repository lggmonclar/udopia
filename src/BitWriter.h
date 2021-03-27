//
// Created by Lug on 26/03/2021.
//

#ifndef RUDP_BITWRITER_H
#define RUDP_BITWRITER_H
#include <cstdint>
#include <WS2tcpip.h>
#include "Utils.h"

class BitWriter {
private:
    uint64_t scratch{};
    int scratchBits{};
    int wordIndex{};
    uint32_t* buffer;

public:
    BitWriter(uint8_t *buffer) : buffer(reinterpret_cast<uint32_t *>(buffer)) {}
    BitWriter(uint32_t *buffer) : buffer(buffer) {}

    void WriteBits(uint32_t value, int bits) {
        // Add the number of bits to the scratch memory
        scratch |= static_cast<uint64_t>(value) << scratchBits;

        // Advance the counter of how many bits we wrote, so we know when we reached the desired word size.
        scratchBits += bits;

        // We reached the word size that we're working with
        if(scratchBits >= 32) {
            Flush();
        }
    }

    /**
     * Flushes the scratch memory to the buffer
     */
    void Flush() {
        if(scratchBits > 0) {
            // Write one word to the buffer and increment the counter of how many words we've written
            buffer[wordIndex++] = Utils::HostToNetwork(scratch);

            // Reset the scratch by one word
            scratch >>= 32;
            scratchBits -= 32;
        }
    }
};

#endif //RUDP_BITWRITER_H
