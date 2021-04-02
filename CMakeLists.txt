cmake_minimum_required(VERSION 3.17)
project(udopia)

set(CMAKE_CXX_STANDARD 14)

add_library(udopia src/Udopia.h src/Packet.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h src/Server.h src/Client.cpp src/Client.h src/Server.cpp src/Udopia.cpp)

add_executable(test_server src/test_server.cpp src/Udopia.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h src/Server.h src/Server.cpp src/Udopia.cpp)
add_executable(test_client src/test_client.cpp src/Udopia.h src/Packet.h src/WriteStream.h src/ReadStream.h src/BitWriter.h src/BitReader.h src/Utils.h src/Serialization.h src/Client.cpp src/Client.h src/Udopia.cpp)