cmake_minimum_required(VERSION 3.5)
project(Reader VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options(-Wall -Wextra -Wpedantic -g -std=c++2a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(ReaderLib
    src/MaPrinter.cpp
    src/Tokenizer.cpp)

add_executable(ReaderApp src/reader_main.cpp)
target_link_libraries(ReaderApp ReaderLib)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
  test_reader
  src/test_reader.cpp
)
target_link_libraries(
    test_reader
    GTest::gtest_main
    ReaderLib
)

include(GoogleTest)
gtest_discover_tests(test_reader)
