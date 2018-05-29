cmake_minimum_required(VERSION 3.7)
project(arm11_09)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/emulator/emulator_header.c
        src/emulator/emulator_header.h
        src/emulator/emulator_multiply.c
        src/assemble.c
        src/emulate.c)

add_executable(arm11_09 ${SOURCE_FILES})