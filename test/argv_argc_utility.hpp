/**
 * @file argv_argc_utility.hpp
 * @ingroup Ping_Service
 * @brief Class for simulating argv,argc for unit tests
 * continuously
 *
 * Copied from //https://github.com/jarro2783/cxxopts/blob/master/test/options.cpp
 *
 *
 */
#pragma once

#include <initializer_list>
#include <vector>
#include <string>
#include <cstring>
#include <memory>


//Helpful little argv,argc simulator class found in unit tests for cxxopts
//https://github.com/jarro2783/cxxopts/blob/master/test/options.cpp
class Argv {
public:

  Argv(std::initializer_list<const char*> args)
      : m_argv(new char*[args.size()])
      , m_argc(static_cast<int>(args.size()))
  {
    int i = 0;
    auto iter = args.begin();
    while (iter != args.end()) {
      auto len = strlen(*iter) + 1;
      auto ptr = std::unique_ptr<char[]>(new char[len]);

      strcpy(ptr.get(), *iter);
      m_args.push_back(std::move(ptr));
      m_argv.get()[i] = m_args.back().get();

      ++iter;
      ++i;
    }
  }

  char** argv() const {
    return m_argv.get();
  }

  int argc() const {
    return m_argc;
  }

private:

  std::vector<std::unique_ptr<char[]>> m_args;
  std::unique_ptr<char*[]> m_argv;
  int m_argc;
};