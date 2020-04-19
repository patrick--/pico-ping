/**
 * @file ping_service.cpp
 * @ingroup Ping_Service
 * @brief Bare bones ICMP ping implementation that sends echo requests
 * continuously
 *
 * Copyright (c) 2020 Patrick Servello (patrick.servello@gmail.com)
 *
 *
 * Distributed under the Apache license and can be found in LICENSE.txt
 */

#include <iomanip>
#include <iostream>
#include <thread>

#include "ping_service.h"

namespace pico_ping {

//Ensure that class is usable after construction
Ping_Service::Ping_Service(const std::string &host, seconds timeout)
    : timeout_(timeout) {
  remote_dest_ = str_to_in_addr(host);
  socket_init();
}

// Packet sending and receiving loop
void Ping_Service::start() {
  int sequence = 0;
  while (true) {

    unsigned char data[64];

    struct timeval timeout_settings = {timeout_.count(), 0};

    icmp_header_.un.echo.sequence = ++sequence;
    std::memcpy(data, &icmp_header_, sizeof(icmp_header_));
    std::memcpy(data + sizeof(icmp_header_), "PingPong", 8);

    // Cover general send failure incase interface goes down - no reason to exit
    int rc = sendto(sock_, data, sizeof(icmp_header_) + 8, 0,
                    (struct sockaddr *)&addr_, sizeof(addr_));
    log_echo_sent_time(sequence);
    if (rc <= 0) {
      std::cout << "Ping failed. \n";
    }

    fd_set read_set;
    memset(&read_set, 0, sizeof(read_set));
    FD_SET(sock_, &read_set);

    rc = select(sock_ + 1, &read_set, NULL, NULL, &timeout_settings);
    if (rc == 0) {
      std::cout << "Request timed out \n";
      continue;
    }

    //Get data from response
    socklen_t slen = 0;
    rc = recvfrom(sock_, data, sizeof(data), 0, NULL, &slen);
    std::memcpy(&icmp_response_header_, data, sizeof(icmp_response_header_));

    //We only care about ECHO_REPLY ICMP packets, ignore all other types
    if (icmp_response_header_.type == ICMP_ECHOREPLY) {
      int recvd_seq = icmp_response_header_.un.echo.sequence;
      std::cout << std::fixed << std::setprecision(2);
      std::cout << sizeof(data) << " bytes from " << inet_ntoa(remote_dest_)
                << ": icmp_seq=" << recvd_seq
                << " time=" << get_packet_rtt(recvd_seq).count() << "\n";
    }

    // Clear out hash map containing last 100 packet sent timepoints
    if (sequence % 100 == 0) {
      sequence = 0;
      clear_echo_sent_times();
    }

    std::this_thread::sleep_for(milliseconds(500));
  }
}

struct in_addr Ping_Service::str_to_in_addr(const std::string &host) {
  struct in_addr dst;

  // Use number of "."'s to determine if ip address or hostname
  auto num_periods = count(host.cbegin(), host.cend(), '.');

  // Attempt to parse as an IP address
  if (num_periods == 3) {
    if (inet_aton(host.c_str(), &dst) == 0)
      throw std::invalid_argument("Invalid IP address.");
  }

  // Attempt to parse as a hostname
  else {
    auto hp = gethostbyname(host.c_str());
    if (hp == nullptr)
      throw std::invalid_argument("Invalid hostname.");

    // https://stackoverflow.com/questions/5444197/converting-host-to-ip-by-sockaddr-in-gethostname-etc
    std::memcpy(&dst, hp->h_addr_list[0], hp->h_length);
  }

  return dst;
}

// Init all local socket resources to allow them to send and receive
void Ping_Service::socket_init() {
  sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

  // If socket is already used, or permissions are not correct throw
  if (sock_ < 0) {
    throw std::runtime_error("Unable to create socket");
  }

  //Setup remote address fields
  std::memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr = remote_dest_;

  //Setup packet to send fields
  std::memset(&icmp_header_, 0, sizeof(icmp_header_));
  icmp_header_.type = ICMP_ECHO;
  icmp_header_.un.echo.id = 1337;
}

//Add an entry to hashmap with a timepoint and a given packet sequence
void Ping_Service::log_echo_sent_time(int sequence) {
  echo_sent_times_.insert({sequence, steady_clock::now()});
}

//Delete all entries in the hashmap
void Ping_Service::clear_echo_sent_times() { echo_sent_times_.clear(); }

//Calculate a duration based on a now time point and the logged time point
duration<double, std::milli> Ping_Service::get_packet_rtt(int sequence) {
  return steady_clock::now() - echo_sent_times_[sequence];
}
} // namespace pico_ping
