
include_directories(
        ../src
        ../extern/cxxopts
)

#add_executable(pico_ping pico_ping.cpp ../src/ping.h ../src/ping.cpp)
add_executable(
        pico_ping
        pico_ping.cpp
        ../src/ping_service.h ../src/ping_service.cpp
        ../src/cli.h ../src/cli.cpp
        ../extern/cxxopts/cxxopts.hpp
)
