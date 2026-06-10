cmake_minimum_required(VERSION 3.10)
project(ExchangeEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(exchange_lib STATIC
    src/OrderBook.cpp
)

add_executable(engine
	src/main.cpp
	src/OrderBook.cpp
)

target_link_libraries(engine PRIVATE exchange_lib)

enable_testing()
add_subdirectory(tests)