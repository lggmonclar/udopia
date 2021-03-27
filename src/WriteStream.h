//
// Created by Lug on 26/03/2021.
//

#ifndef RUDP_WRITESTREAM_H
#define RUDP_WRITESTREAM_H
#include <cassert>
#include "BitWriter.h"
#include "Utils.h"

namespace Udopia {
    class WriteStream {
    public:
        enum {IsWriting = 1};
        enum {IsReading = 0};

        explicit WriteStream(uint8_t* buffer) : bitWriter(buffer) {}

        bool SerializeInteger(int32_t value, int32_t min, int32_t max) {
            assert(min < max);
            assert(value >= min);
            assert(value <= max);

            const int bits = Utils::BitsRequired(min, max);
            uint32_t unsignedValue = value - min;
            bitWriter.WriteBits(unsignedValue, bits);
            return true;
        }

        bool SerializeBits(uint32_t value, int bits) {
            assert(bits > 0);
            assert(bits <= 32);
            bitWriter.WriteBits(value, bits);
            return true;
        }

        void Flush() {
            bitWriter.Flush();
        }

    private:
        BitWriter bitWriter;
    };
}


#endif //RUDP_WRITESTREAM_H
