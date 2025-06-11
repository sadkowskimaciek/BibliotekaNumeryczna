cmake_minimum_required(VERSION 3.10)
project(BibliotekaNumeryczna)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BibliotekaNumeryczna STATIC
    src/RownaniaLiniowe.cpp
)

target_include_directories(BibliotekaNumeryczna PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)


enable_testing()

add_executable(RownaniaLinioweTest tests/RownaniaLinioweTest.cpp)
target_link_libraries(RownaniaLinioweTest PRIVATE BibliotekaNumeryczna)
add_test(NAME RownaniaLinioweTest COMMAND RownaniaLinioweTest)
