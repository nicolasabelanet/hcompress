cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.29)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.29)
endif()

project(hcompress VERSION 1.0
                  DESCRIPTION "Simple Huffman compression."
                  LANGUAGES C)

add_executable(hcompress src/hcompress.c src/hcompress.h src/linked_list.c src/linked_list.h)

