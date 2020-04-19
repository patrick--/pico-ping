/**
 * @file linux_socket_incl.h
 * @ingroup Ping_Service
 * @brief Utility header file to group all necessary linux socket headers
 *
 * Copyright (c) 2020 Patrick Servello (patrick.servello@gmail.com)
 *
 * Distributed under the Apache license and can be found in LICENSE.txt
 */
#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/select.h>
#include <sys/socket.h>