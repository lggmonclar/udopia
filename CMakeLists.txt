cmake_minimum_required(VERSION 3.17)
project(udopia)

set(CMAKE_CXX_STANDARD 14)

add_library(udopia src/library.cpp src/library.h src/Packet.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h)

add_executable(test_server src/test_server.cpp src/library.cpp src/library.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h)
add_executable(test_client src/test_client.cpp src/library.cpp src/library.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h)