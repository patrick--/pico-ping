
include_directories(
        ../extern/catch2
        ../extern/cxxopts
        ../src
)

add_executable(
        TestAll
        test.cpp
        ../extern/catch2/catch.hpp
        ../extern/cxxopts/cxxopts.hpp
        ../src/cli.h ../src/cli.cpp
        ../src/ping_service.h ../src/ping_service.cpp
)

target_link_libraries(TestAll)