#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "cxxopts.hpp"

using namespace std::chrono;

namespace pico_ping {
namespace cli {

struct command_parameters {
  std::string host;
  seconds timeout;
};

command_parameters get_input(int argc, char **argv);
void show_usage();
} // namespace cli
} // namespace pico_ping
