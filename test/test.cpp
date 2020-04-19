#define CATCH_CONFIG_MAIN
#include "argv_argc_utility.hpp"
#include "catch.hpp"
#include "cli.h"
#include "ping_service.h"

using namespace pico_ping;

TEST_CASE("Testing Command line parsing") {

  SECTION("Valid positional destination parameter without optional") {
    Argv argv({"test", "8.8.8.8"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_NOTHROW(cli::get_input(argc, actual_argv));

    cli::command_parameters res = cli::get_input(argc, actual_argv);
    REQUIRE(res.host == "8.8.8.8");
  }

  SECTION("Valid positional destination parameter with valid short optional") {
    Argv argv({"test", "8.8.8.8", "-W", "5"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_NOTHROW(cli::get_input(argc, actual_argv));

    cli::command_parameters res = cli::get_input(argc, actual_argv);
    REQUIRE(res.host == "8.8.8.8");
    REQUIRE(res.timeout.count() == 5);
  }

  SECTION("Valid positional destination parameter with valid long optional") {
    Argv argv({"test", "8.8.8.8", "--timeout", "5"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_NOTHROW(cli::get_input(argc, actual_argv));

    cli::command_parameters res = cli::get_input(argc, actual_argv);
    REQUIRE(res.host == "8.8.8.8");
    REQUIRE(res.timeout.count() == 5);
  }

  SECTION(
      "Missing positional destination parameter with valid short optional") {
    Argv argv({"test", "-W", "5"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_THROWS_AS(cli::get_input(argc, actual_argv), std::invalid_argument);
  }

  SECTION("Invalid short optional") {
    Argv argv({"test", "-L", "5"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_THROWS_AS(cli::get_input(argc, actual_argv), std::invalid_argument);
  }

  SECTION("Invalid long optional") {
    Argv argv({"test", "-noexist", "5"});

    char **actual_argv = argv.argv();
    auto argc = argv.argc();

    REQUIRE_THROWS_AS(cli::get_input(argc, actual_argv), std::invalid_argument);
  }
}

TEST_CASE("Testing Ping_Service constructor") {
  seconds timeout(5);

  SECTION("Testing that a valid IP does not throw an exception") {
    REQUIRE_NOTHROW(Ping_Service("8.8.8.8", timeout));
  }

  SECTION("Testing that a valid host does not throw an exception") {
    REQUIRE_NOTHROW(Ping_Service("google.com", timeout));
  }

  SECTION("Testing that invalid hostname throws proper exception") {
    REQUIRE_THROWS_AS(Ping_Service("google..com", timeout),
                      std::invalid_argument);
  }

  SECTION("Testing that invalid hostname throws proper exception") {
    REQUIRE_THROWS_AS(Ping_Service(".com", timeout), std::invalid_argument);
  }

  SECTION("Testing that invalid ip throws proper exception") {
    REQUIRE_THROWS_AS(Ping_Service("8.8.8.257", timeout),
                      std::invalid_argument);
  }

  SECTION("Testing that invalid ip throws proper exception") {
    REQUIRE_THROWS_AS(Ping_Service("8.8.8.", timeout), std::invalid_argument);
  }

  SECTION("Testing that blank parameter throws proper exception") {
    REQUIRE_THROWS_AS(Ping_Service("", timeout), std::invalid_argument);
  }
}