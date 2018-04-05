cmake_minimum_required(VERSION 3.10)
project(toylet)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        toylet
        src/main.cpp
        src/toylet.cpp
        src/toylet.h
        src/scanner.cpp
        src/scanner.h
        src/token.cpp
        src/token.h
        src/error_reporter.cpp
        src/error_reporter.h
        src/error.cpp
        src/error.h
)