cmake_minimum_required(VERSION 2.8)
project(libwave)

file(GLOB LIBWAVC libwav/*.cpp)
file(GLOB LIBWAVH libwav/*.h)

add_library(libwav ${LIBWAVC} ${LIBWAVH})

add_executable(wavtest test/main.cpp)
target_link_libraries(wavtest libwav)
