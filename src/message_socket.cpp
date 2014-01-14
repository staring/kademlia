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

#include "message_socket.hpp"

namespace kademlia {
namespace detail {

namespace {

/**
 *
 */
std::vector<message_socket::endpoint_type>
convert_endpoints
    ( boost::asio::io_service & io_service
    , std::vector<endpoint> const& es )
{
    std::vector<message_socket::endpoint_type> resolved_endpoints;

    for ( endpoint const& e : es ) {
        auto const new_endpoints = resolve_endpoint( io_service, e );

        resolved_endpoints.insert( resolved_endpoints.end()
                                 , new_endpoints.begin()
                                 , new_endpoints.end() );
    }

    return std::move( resolved_endpoints );
}

/**
 *
 */
message_socket
create_socket
    ( boost::asio::io_service & io_service
    , message_socket::endpoint_type const& e )
{ return message_socket{ io_service, e }; }

} // anonymous namespace 

/**
 *
 */
message_sockets
create_sockets
    ( boost::asio::io_service & io_service
    , std::vector<endpoint> const& es )
{
    // Get resolved endpoints from raw endpoints.
    auto const endpoints = convert_endpoints( io_service, es );

    std::vector<message_socket> sockets;
    sockets.reserve( endpoints.size() );
    
    // Create one socket per resolved_endpoint.
    for( auto const& e : endpoints ) 
        sockets.push_back( create_socket( io_service, e ) );

    return std::move( sockets );
}

/**
 *
 */
void
graceful_close_socket
    ( message_socket & s )
{
    boost::system::error_code error_discared;
    s.close( error_discared );
}

/**
 *
 */
resolved_endpoints
resolve_endpoint
    ( boost::asio::io_service & io_service
    , endpoint const& e )
{
    message_socket::protocol_type::resolver r{ io_service };
    message_socket::protocol_type::resolver::query q{ e.address(), e.service() };
    // One raw endpoint (e.g. localhost) can be resolved to
    // multiple endpoints (e.g. IPv4 / IPv6 address).
    message_socket::protocol_type::resolver::iterator i = r.resolve(q), end;

    return resolved_endpoints{ i, end };
}

} // namespace detail
} // namespace kademlia

