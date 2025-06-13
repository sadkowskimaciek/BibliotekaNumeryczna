cmake_minimum_required(VERSION 3.10)
project(BibliotekaNumeryczna)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BibliotekaNumeryczna STATIC
    src/RownaniaLiniowe.cpp
    src/Interpolacja.cpp
    src/RownaniaRozniczkowe.cpp
    src/Aproksymacja.cpp
    src/Calkowanie.cpp
    src/RownaniaNieLiniowe.cpp

)

target_include_directories(BibliotekaNumeryczna PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(RownaniaRozniczkowePrzyklad examples/RownaniaRozniczkowePrzyklad.cpp)
target_link_libraries(RownaniaRozniczkowePrzyklad BibliotekaNumeryczna)

add_executable(AproksymacjaPrzyklad example/AproksymacjaPrzyklad.cpp)
target_link_libraries(AproksymacjaPrzyklad PRIVATE BibliotekaNumeryczna)

enable_testing()

add_executable(RownaniaLinioweTest tests/RownaniaLinioweTest.cpp)
target_link_libraries(RownaniaLinioweTest PRIVATE BibliotekaNumeryczna)
add_test(NAME RownaniaLinioweTest COMMAND RownaniaLinioweTest)

add_executable(RownaniaNieLinioweTest tests/RownaniaNieLinioweTest.cpp)
target_link_libraries(RownaniaNieLinioweTest PRIVATE BibliotekaNumeryczna)
add_test(NAME RownaniaNieLinioweTest COMMAND RownaniaNieLinioweTest)

add_executable(InterpolacjaTest tests/InterpolacjaTest.cpp)
target_link_libraries(InterpolacjaTest PRIVATE BibliotekaNumeryczna)
add_test(NAME InterpolacjaTest COMMAND InterpolacjaTest)

add_executable(RownaniaRozniczkoweTest tests/RownaniaRozniczkoweTest.cpp)
target_link_libraries(RownaniaRozniczkoweTest PRIVATE BibliotekaNumeryczna)
add_test(NAME RownaniaRozniczkoweTest COMMAND RownaniaRozniczkoweTest)


add_executable(AproksymacjaTest tests/AproksymacjaTest.cpp)
target_link_libraries(AproksymacjaTest PRIVATE BibliotekaNumeryczna)
add_test(NAME AproksymacjaTest COMMAND AproksymacjaTest)


add_executable(CalkowanieTest tests/CalkowanieTest.cpp)
target_link_libraries(CalkowanieTest PRIVATE BibliotekaNumeryczna)
add_test(NAME CalkowanieTest COMMAND CalkowanieTest)
