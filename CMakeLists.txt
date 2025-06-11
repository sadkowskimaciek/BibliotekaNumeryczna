cmake_minimum_required(VERSION 3.10)
project(BibliotekaNumeryczna)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🔹 Dodaj bibliotekę z pliku źródłowego
add_library(BibliotekaNumeryczna STATIC
    src/RownaniaLiniowe.cpp
)

# 🔹 Dodaj katalog nagłówków
target_include_directories(BibliotekaNumeryczna PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 🔹 Włącz testy
enable_testing()

# 🔹 Test: Równania liniowe
add_executable(RownaniaLinioweTest tests/RownaniaLinioweTest.cpp
)
target_link_libraries(RownaniaLinioweTest PRIVATE BibliotekaNumeryczna)
add_test(NAME RownaniaLinioweTest COMMAND RownaniaLinioweTest)
