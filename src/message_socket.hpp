// Copyright (c) 2013-2014, David Keller
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of California, Berkeley nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY DAVID KELLER AND CONTRIBUTORS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef KADEMLIA_MESSAGE_SOCKET_HPP
#define KADEMLIA_MESSAGE_SOCKET_HPP

#ifdef _MSC_VER
#   pragma once
#endif

#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include <kademlia/endpoint.hpp>

namespace kademlia {
namespace detail {

///
using message_socket = boost::asio::ip::udp::socket;
///
using message_sockets = std::vector<message_socket>;
///
using resolved_endpoints = std::vector<message_socket::endpoint_type>;

/**
 *
 */
message_sockets
create_sockets
    ( boost::asio::io_service & io_service
    , std::vector<endpoint> const& es );

/**
 *
 */
void
graceful_close_socket
    ( message_socket & s );

/**
 *
 */
resolved_endpoints
resolve_endpoint
    ( boost::asio::io_service & io_service
    , endpoint const& e );

} // namespace detail
} // namespace kademlia

#endif

