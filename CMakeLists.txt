cmake_minimum_required(VERSION 3.10)
project(ExchangeEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(engine src/main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)