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
  /**
   * @brief Construct a Ping_Service object that is ready to send echo requests
   *
   * Responsible for validating hostname or ip address and then initializes all
   * local and remote network structures.
   *
   * If IP address or hostname is found to be invalid an exception is thrown.
   *
   * @param[in] host String containing either the hostname or ip address
   * @param[in] timeout Chrono seconds object for storing timeout value
   *
   * @throw std::invalid_argument if IP or hostname is invalid
   * @throw std::runtime_error if socket operations fail
   */
  Ping_Service(const std::string &host, seconds timeout);

  /**
   * @brief Infinite loop that sounds out ICMP echo packets and processes
   * replies
   *
   * Constructs echo packets and processes replies as they are recieved. If a
   * response takes longer than the timeout value, a packet loss error is
   * reported
   *
   * If IP address or hostname is found to be invalid an exception is thrown.
   */
  void start();

private:
  /**
   * @brief Converts a string to in_addr struct
   *
   * Takes a string representation of a hostname or ip address and converts it
   * into in_addr struct which is used during socket operations.
   *
   * @param[in] host String representation of a hostname of IP address
   *
   * @return in_addr struct containing populated address fields containing
   * address
   *
   * @throw std::invalid_argument if string representation cannot be converted
   * to address
   */
  struct in_addr str_to_in_addr(const std::string &host);
  /**
   * @brief Initializes socket for sending and receiving ICMP packets
   *
   */
  void socket_init();
  /**
   * @brief Calculates packet RTT based on stored sent time and received time
   *
   * Because there is no assurance that ICMP replies will be recieved in the
   * order they were dispatched, responses are stored in a hash map with the
   * packet sequence acting as the key and a chrono timepoint as the value. Upon
   * the socket receiving a response, an elapsed duration is calculated by
   * comparing the timepoint value with that key and the timepoint representing
   * now.
   *
   * @param[in] Packet sequence identifier
   *
   * @return Floating point duration value representing milliseconds.
   */
  duration<double, std::milli> get_packet_rtt(int pkt_sequence);
  /**
   * @brief Adds a packet sequence and chonro time point to hashmap
   *
   * @param[in] Packet sequence identifier
   *
   */
  void log_echo_sent_time(int pkt_sequence);
  /**
   * @brief Clears all packets out of hashmap
   *
   * The hash map is reset every 100 packets
   *
   */
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