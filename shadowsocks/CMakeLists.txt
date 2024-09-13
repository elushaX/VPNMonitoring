cmake_minimum_required(VERSION 3.12)

project(libSocks)

find_package(prometheus-cpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Monitor.cpp PGDataBase.cpp PrometheusDataBase.cpp)

target_link_libraries(${PROJECT_NAME} pcap pqxx pq prometheus-cpp::pull)
