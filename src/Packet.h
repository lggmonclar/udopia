//
// Created by Lug on 20/03/2021.
//

#ifndef RUDP_PACKET_H
#define RUDP_PACKET_H

#include <cstdint>

#define MAX_PACKET_SIZE 2048

class Packet {
public:
    char buffer[MAX_PACKET_SIZE];

    Packet() {
        writePtr = readPtr = buffer;
    }

    int Size() {
        return writePtr - buffer;
    }

    /**
     * Serializes and writes an unsigned 2-byte integer to the packet's contents
     * @param value
     */
    void WriteUInt16(unsigned int value) {
        *writePtr++ = value >> 8;
        *writePtr++ = value;
    }

    unsigned int ReadUInt16() {
        unsigned int result = ((unsigned int)readPtr[0]<<8) | readPtr[1];
        readPtr += 2;
        return result;
    }

    /**
     * Serializes and writes an unsigned 4-byte integer to the packet's contents
     * @param value
     */
    void WriteUInt32(unsigned long value) {
        *writePtr++ = value >> 24;
        *writePtr++ = value >> 16;
        *writePtr++ = value >> 8;
        *writePtr++ = value;
    }

    unsigned long ReadUInt32() {
        unsigned long result =
                ((unsigned long)readPtr[0]<<24) |
                ((unsigned long)readPtr[1]<<16) |
                ((unsigned long)readPtr[2]<<8) |
                readPtr[3];
        readPtr += 4;
        return result;
    }

    /**
     * Serializes and writes an unsigned 8-byte integer to the packet's contents
     * @param value
     */
    void WriteUInt64(unsigned long long value) {
        *writePtr++ = value >> 56;
        *writePtr++ = value >> 48;
        *writePtr++ = value >> 40;
        *writePtr++ = value >> 32;
        *writePtr++ = value >> 24;
        *writePtr++ = value >> 16;
        *writePtr++ = value >> 8;
        *writePtr++ = value;
    }

    unsigned long long ReadUInt64() {
        unsigned long long result =
                ((unsigned long long)readPtr[0]<<56) |
                ((unsigned long long)readPtr[1]<<48) |
                ((unsigned long long)readPtr[2]<<40) |
                ((unsigned long long)readPtr[3]<<32) |
                ((unsigned long long)readPtr[4]<<24) |
                ((unsigned long long)readPtr[5]<<16) |
                ((unsigned long long)readPtr[6]<<8) |
                readPtr[7];
        readPtr += 4;
        return result;
    }

    /**
     * Serializes and writes a 4-byte float to the packet's contents
     * @param value
     */
    void WriteFloat(const float value) {

    }

    /**
     * Serializes and writes an 8-byte float to the packet's contents
     * @param value
     */
    void WriteDouble(const double value) {

    }

    /**
     * Writes a char value to the packet's contents
     * @param value
     */
    void WriteChar(const char value) {
        *writePtr++ = value;
    }

    char ReadChar() {
        char ch = *readPtr;
        readPtr++;
        return ch;
    }

    /**
     * Writes a string to the packet's contents
     * @param value
     */
    void WriteString(const char* const value) {
        const char* p = value;
        while(*p != '\0') {
            *writePtr++ = *p++;
        }
        *writePtr++ = '\0';
    }

    char* ReadString() {
        char* strPtr = readPtr;
        while(*readPtr != '\0') {
            readPtr++;
        }

        return strPtr;
    }

    ~Packet() {
//        free(buffer);
    }
private:
    char* writePtr;
    char* readPtr;
};

#endif //RUDP_PACKET_H
