cmake_minimum_required(VERSION 3.12)

project(libSocks)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pcap)
