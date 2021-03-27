//
// Created by Lug on 26/03/2021.
//

#ifndef RUDP_READSTREAM_H
#define RUDP_READSTREAM_H
#include <cassert>
#include "BitReader.h"
#include "Utils.h"

namespace Udopia {
    class ReadStream {
    public:
        enum {IsWriting = 0};
        enum {IsReading = 1};

        ReadStream(uint8_t * buffer, int size) : bitReader(buffer, size) {}

        bool SerializeInteger(int32_t &value, int32_t min, int32_t max) {
            assert(min < max);
            const int bits = Utils::BitsRequired(min, max);
            if (bitReader.WouldReadPastEnd(bits)) {
                return false;
            }
            uint32_t unsignedValue = bitReader.ReadBits(bits);
            value = static_cast<int32_t>(unsignedValue) + min;

            return value >= min && value <= max;
        }

        bool SerializeBits(uint32_t& value, int bits) {
            assert(bits > 0);
            assert(bits <= 32);
            if (bitReader.WouldReadPastEnd(bits)) {
                return false;
            }

            uint32_t read_value = bitReader.ReadBits(bits);
            value = read_value;
            return true;
        }

    private:
        BitReader bitReader;
    };
}

#endif //RUDP_READSTREAM_H
