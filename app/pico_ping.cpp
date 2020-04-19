/**
 * @file pico_ping.cpp
 * @ingroup Ping_Service
 * @brief Main executable for running application
 *
 * Copyright (c) 2020 Patrick Servello (patrick.servello@gmail.com)
 *
 * Distributed under the Apache license and can be found in LICENSE.txt
 */

#include "ping_service.h"
#include "cli.h"

using namespace pico_ping;

int main(int argc, char** argv){

  try {
    auto params = cli::get_input(argc, argv);
    auto p = Ping_Service(params.host, params.timeout);
    p.start();

  } catch (const std::invalid_argument& e) {
    cli::show_usage();
    exit(0);
  } catch (const std::runtime_error& e) {
    std::cout << "Encountered runtime network error - check permissions\n";
    exit(0);
  } catch(...) {
    std::cout << "Encountered unknown error\n";
    exit(0);
  }
}