/**
 * @file cli.h
 * @ingroup Ping_Service
 * @brief Basic command line parsing functionality
 *
 * This file acts as a thin wrapper around cxxopts to allow for easier testability
 * and to limit functionality that we aren't using at the moment.
 *
 * Copyright (c) 2020 Patrick Servello (patrick.servello@gmail.com)
 *
 * Distributed under the Apache license and can be found in LICENSE.txt
 */
#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "cxxopts.hpp"

using namespace std::chrono;

namespace pico_ping {
namespace cli {

/**
 * @brief Reusable structure to hold all current and future command line params
 *
*/
struct command_parameters {
  std::string host;
  seconds timeout;
};

/**
 * @brief Parse commandline input and generate converted parameter values
 * to pass to ping service
 *
 * This is what acts as a thin wrapper around cxxopts. If an invalid option flag
 * is provided, or if the positional host parameter is ommited, an exception will
 * be thrown.
 *
 * @param[in] argc Argument count from the commandline
 * @param[in] argv Argument array from the commandline
 *
 * @throw std::invalid_argument if IP or hostname is invalid
 * @throw std::runtime_error if socket operations fail
 */
command_parameters get_input(int argc, char **argv);

/**
 * @brief Show usage statement for commmand line parameters
 *
 */
void show_usage();
} // namespace cli
} // namespace pico_ping
