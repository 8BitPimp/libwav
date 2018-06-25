cmake_minimum_required(VERSION 2.8)
project(libwave)

file(GLOB LIBWAVEC libwave/*.cpp)
file(GLOB LIBWAVEH libwave/*.h)

add_library(libwave ${LIBWAVEC} ${LIBWAVEH})

add_executable(testwave testwave/main.cpp)
target_link_libraries(testwave libwave)

configure_file(testwave.py.in testwave.py)
