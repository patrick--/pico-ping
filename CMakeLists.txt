cmake_minimum_required(VERSION 3.12)
project(pico_ping)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(app)

enable_testing()
add_subdirectory(test)
add_test(Tests TestAll)