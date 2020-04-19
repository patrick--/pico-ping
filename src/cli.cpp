#include "cli.h"
#include <iomanip>

namespace pico_ping {
namespace cli {
command_parameters get_input(int argc, char **argv) {

  cxxopts::Options options("pico_ping", "IMCP echo sender");
  options.add_options()("host", "", cxxopts::value<std::string>())(
      "W,timeout", "Response packet timeout [sec]",
      cxxopts::value<int>()->default_value("5"));

  try {

    options.parse_positional("host");
    auto result = options.parse(argc, argv);

    if (result["host"].count() != 1) {
      throw(std::invalid_argument("Invalid command line parameters"));
    }

    seconds timeout = seconds(result["timeout"].as<int>());

    command_parameters params = {result["host"].as<std::string>(), timeout};
    return params;
  }

  catch (const cxxopts::OptionParseException &ex) {
    throw(std::invalid_argument("Invalid command line parameters"));
  }
}

void show_usage() {
  std::cout << "\nUsage:\n";
  std::cout << std::setw(8) << "pico_ping [OPTION...] destination\n";
  std::cout << std::setw(64)
            << "-W, --timeout arg Response packet timeout [sec] (default: 5)\n";
}
} // namespace cli
} // namespace pico_ping