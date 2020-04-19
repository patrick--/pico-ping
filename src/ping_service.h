/**
 * @file ping_service.h
 * @ingroup Ping_Service
 * @brief Bare bones ICMP ping implementation that sends echo requests
 * continuously
 *
 * Copyright (c) 2020 Patrick Servello (patrick.servello@gmail.com)
 *
 *
 * Distributed under the Apache license and can be found in LICENSE.txt
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

// Utility include that has all relevant linux network header files
#include "linux_socket_incl.h"

using namespace std::chrono;

/**
 * @brief Main class that is responsible that continually sends ECHO packets.
 *
 * Implements all low level detail such as constructing system socket, building
 * the remote host structure, packet structure and then finally sending packet
 * and parsing response.
 */
namespace pico_ping {

class Ping_Service {
public:
  Ping_Service(const std::string &host, seconds timeout);
  void start();

private:
  struct in_addr str_to_in_addr(const std::string &);
  void socket_init();
  duration<double, std::milli> get_packet_rtt(int);
  void log_echo_sent_time(int);
  void clear_echo_sent_times();

  std::map<int, time_point<steady_clock>> echo_sent_times_;
  int sock_;
  seconds timeout_ = seconds(5);
  struct in_addr remote_dest_;
  struct icmphdr icmp_header_;
  struct sockaddr_in addr_;
  struct icmphdr icmp_response_header_;
};
} // namespace pico_ping