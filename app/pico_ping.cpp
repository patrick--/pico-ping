//#include "ping.h"
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
  }
}