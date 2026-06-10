cmake_minimum_required(VERSION 3.10)
project(ExchangeEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sanitizers
option(ENABLE_SANITIZER_ADDRESS "Enable AddressSanitizer" OFF)
option(ENABLE_SANITIZER_UNDEFINED "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_SANITIZER_THREAD "Enable ThreadSanitizer" OFF)

function(apply_sanitizers TARGET_NAME)
    if(ENABLE_SANITIZER_ADDRESS)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()

    if(ENABLE_SANITIZER_UNDEFINED)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
    endif()

    if(ENABLE_SANITIZER_THREAD)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=thread)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread)
    endif()
endfunction()

include_directories(include)

add_library(exchange_lib STATIC
    src/OrderBook.cpp
)

add_executable(engine
	src/main.cpp
)

target_link_libraries(engine PRIVATE exchange_lib)

enable_testing()
add_subdirectory(tests)

apply_sanitizers(exchange_lib)
apply_sanitizers(engine)