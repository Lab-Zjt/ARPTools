cmake_minimum_required(VERSION 3.8)
project(ARPtools)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp mycli.cpp myarpt.cpp mydns.cpp myconf.cpp)
add_executable(ARPtools ${SOURCE_FILES})
find_package(Threads)
target_link_libraries(ARPtools ${CMAKE_THREAD_LIBS_INIT})