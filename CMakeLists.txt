cmake_minimum_required(VERSION 3.3)
project(HaffmanCoding)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Huffman_Coding.c Huffman_Coding.h)
add_executable(HaffmanCoding ${SOURCE_FILES} Huffman_Coding.c Huffman_Coding.h MyStructs.c)