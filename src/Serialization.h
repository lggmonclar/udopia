//
// Created by Lug on 26/03/2021.
//

#ifndef RUDP_SERIALIZATION_H
#define RUDP_SERIALIZATION_H

#include <cstdint>

#define SerializeInt(stream, value, min, max)                       \
    do                                                              \
    {                                                               \
        int32_t signedValue;                                        \
        if (Stream::IsWriting) {                                    \
            signedValue = (int32_t) value;                          \
        }                                                           \
        if (!stream.SerializeInteger(signedValue, min, max)) {      \
            return false;                                           \
        }                                                           \
        if (Stream::IsReading) {                                    \
            value = signedValue;                                    \
        }                                                           \
     } while (0)

template <typename Stream>
bool SerializeFloatInternal(Stream& stream, float& value ) {
    union FloatInt {
        float floatVal;
        uint32_t uintVal;
    };
    FloatInt tmp;
    if (Stream::IsWriting) {
        tmp.floatVal = value;
    }
    bool result = stream.SerializeBits(tmp.uintVal, 32);
    if (Stream::IsReading) {
        value = tmp.floatVal;
    }
    return result;
}

#define SerializeFloat(stream, value)                             \
  do                                                              \
  {                                                               \
      if (!SerializeFloatInternal(stream, value)) {               \
          return false;                                           \
      }                                                           \
} while (0)
#endif //RUDP_SERIALIZATION_H
