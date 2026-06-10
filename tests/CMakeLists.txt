include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(engine_tests test_main.cpp test_order_book.cpp)
target_link_libraries(engine_tests PRIVATE exchange_lib gtest)
add_test(NAME EngineUnitTests COMMAND engine_tests)
apply_sanitizers(engine_tests)