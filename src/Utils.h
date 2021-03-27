#ifndef RUDP_UTILS_H
#define RUDP_UTILS_H

//
// Copied from https://github.com/networkprotocol/yojimbo/blob/master/yojimbo.h#L871
//
#include <cmath>

#if !defined (LITTLE_ENDIAN ) && !defined( BIG_ENDIAN )
    #ifdef __BYTE_ORDER__
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
          #define LITTLE_ENDIAN 1
        #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
          #define BIG_ENDIAN 1
        #else
          #error Unknown machine endianess detected. User needs to define LITTLE_ENDIAN or BIG_ENDIAN.
        #endif // __BYTE_ORDER__

      // Detect with GLIBC's endian.h
    #elif defined(__GLIBC__)
        #include <endian.h>
        #if (__BYTE_ORDER == __LITTLE_ENDIAN)
          #define LITTLE_ENDIAN 1
        #elif (__BYTE_ORDER == __BIG_ENDIAN)
          #define BIG_ENDIAN 1
        #else
          #error Unknown machine endianess detected. User needs to define LITTLE_ENDIAN or BIG_ENDIAN.
        #endif // __BYTE_ORDER

      // Detect with _LITTLE_ENDIAN and _BIG_ENDIAN macro
    #elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
        #define LITTLE_ENDIAN 1
    #elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
        #define BIG_ENDIAN 1

    // Detect with architecture macros
    #elif    defined(__sparc)     || defined(__sparc__)                           \
            || defined(_POWER)      || defined(__powerpc__)                         \
            || defined(__ppc__)     || defined(__hpux)      || defined(__hppa)      \
            || defined(_MIPSEB)     || defined(_POWER)      || defined(__s390__)
        #define BIG_ENDIAN 1
    #elif    defined(__i386__)    || defined(__alpha__)   || defined(__ia64)      \
            || defined(__ia64__)    || defined(_M_IX86)     || defined(_M_IA64)     \
            || defined(_M_ALPHA)    || defined(__amd64)     || defined(__amd64__)   \
            || defined(_M_AMD64)    || defined(__x86_64)    || defined(__x86_64__)  \
            || defined(_M_X64)      || defined(__bfin__)
        #define LITTLE_ENDIAN 1
    #elif defined(_MSC_VER) && defined(_M_ARM)
        #define LITTLE_ENDIAN 1
        #else
            #error Unknown machine endianess detected. User needs to define LITTLE_ENDIAN or BIG_ENDIAN.
        #endif
#endif


namespace Udopia {
    namespace Utils {

        /**
         *  Get sockaddr, IPv4 or IPv6
         */
        inline void* GetInAddr(struct sockaddr *sa) {
            if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
            }

            return &(((struct sockaddr_in6*)sa)->sin6_addr);
        }

        /**
          Calculates the number of bits required to serialize an integer in range [min,max].
          @param min The minimum value.
          @param max The maximum value.
          @returns The number of bits required to serialize the integer.
       */
        inline int BitsRequired(uint32_t min, uint32_t max) {
            return (min == max) ? 0 : log2(max - min) + 1;
        }

        /**
            Reverse the order of bytes in a 64 bit integer.
            @param value The input value.
            @returns The input value with the byte order reversed.
         */
        inline uint64_t bswap(uint64_t value) {
            value = (value & 0x00000000FFFFFFFF) << 32 | (value & 0xFFFFFFFF00000000) >> 32;
            value = (value & 0x0000FFFF0000FFFF) << 16 | (value & 0xFFFF0000FFFF0000) >> 16;
            value = (value & 0x00FF00FF00FF00FF) << 8  | (value & 0xFF00FF00FF00FF00) >> 8;
            return value;
        }

        /**
            Reverse the order of bytes in a 32 bit integer.
            @param value The input value.
            @returns The input value with the byte order reversed.
         */
        inline uint32_t bswap(uint32_t value) {
            return (value & 0x000000ff) << 24 | (value & 0x0000ff00) << 8 | (value & 0x00ff0000) >> 8 | (value & 0xff000000) >> 24;
        }

        /**
            Reverse the order of bytes in a 16 bit integer.
            @param value The input value.
            @returns The input value with the byte order reversed.
         */
        inline uint16_t bswap(uint16_t value) {
            return (value & 0x00ff) << 8 | (value & 0xff00) >> 8;
        }

        /**
            Template to convert an integer value from local byte order to network byte order.
            @param value The input value in local byte order. Supported integer types: uint64_t, uint32_t, uint16_t.
            @returns The input value converted to network byte order. If this processor is little endian the output is the same as the input. If the processor is big endian, the output is the input byte swapped.
            @see bswap
         */
        template <typename T>
        T HostToNetwork(T value) {
            #if BIG_ENDIAN
                return bswap(value);
            #else // #if BIG_ENDIAN
                return value;
            #endif // #if BIG_ENDIAN
        }

        /**
            Template to convert an integer value from network byte order to local byte order.
            @param value The input value in network byte order. Supported integer types: uint64_t, uint32_t, uint16_t.
            @returns The input value converted to local byte order. If this processor is little endian the output is the same as the input. If the processor is big endian, the output is the input byte swapped.
            @see bswap
         */
        template <typename T>
        T NetworkToHost(T value) {
            #if BIG_ENDIAN
                return bswap(value);
            #else // #if BIG_ENDIAN
                return value;
            #endif // #if BIG_ENDIAN
        }
    }
}

#endif //RUDP_UTILS_H
