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

#ifndef KADEMLIA_SESSION_HPP
#define KADEMLIA_SESSION_HPP

#ifdef _MSC_VER
#   pragma once
#endif

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <system_error>

#include <kademlia/detail/symbol_visibility.hpp>
#include <kademlia/detail/cxx11_macros.hpp>
#include <kademlia/endpoint.hpp>

namespace kademlia {

/**
 *  @brief This object is used to save and load data from the network.
 */
class session final
{
public:
    /// The key type used to find data.
    using key_type = std::vector< std::uint8_t >;

    /// The stored data type.
    using data_type = std::vector< std::uint8_t >;

    /// The callback type called to signal an async save status.
    using save_handler_type = std::function
            < void
                ( std::error_code const& error )
            >;
    /// The callback type called to signal an async load status.
    using load_handler_type = std::function
            < void
                ( std::error_code const& error
                , data_type const& data )
            >;

    /// This kademlia implementation default port.
    enum { DEFAULT_PORT = 27980U };

public:
    /**
     *  @brief Construct a passive session.
     *  @details This session acts like an active session expect it
     *           does'nt try to discover neighbors. It can be used
     *           by the first node of a network as no peer is known
     *           uppon its creation.
     *
     *  @param initial_peer In order to discover network peers, the session
     *         contacts this peer and retrieve it's neighbors.
     *  @param listen_on_ipv4 IPv4 listening endpoint.
     *  @param listen_on_ipv6 IPv6 listening endpoint.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    session
        ( endpoint const& initial_peer
        , endpoint const& listen_on_ipv4 = endpoint{ "0.0.0.0", DEFAULT_PORT }
        , endpoint const& listen_on_ipv6 = endpoint{ "::", DEFAULT_PORT } );

    /**
     *  @brief Destruct the session.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    ~session
        ( void );

    /**
     *  @brief Disabled copy constructor.
     */
    session
        ( session const& )
        = delete;

    /**
     *  @brief Disabled assignment operator.
     */
    session&
    operator=
        ( session const& )
        = delete;

    /**
     *  @brief Async save a data into the network.
     *
     *  @param key The data to save key.
     *  @param data The data to save.
     *  @param handler Callback called to report call status.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    void
    async_save
        ( key_type const& key
        , data_type const& data
        , save_handler_type handler );

    /**
     *  @brief Async load a data from the network.
     *
     *  @param key The data to save key.
     *  @param handler Callback called to report call status.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    void
    async_load
        ( key_type const& key
        , load_handler_type handler );

    /**
     *  @brief This <b>blocking call</b> execute the session main loop.
     *  @details Callbacks are executed inside this call.
     *
     *  @return The exit reason of the call.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    std::error_code
    run
        ( void );

    /**
     *  @brief Abort the session main loop.
     */
    KADEMLIA_SYMBOL_VISIBILITY
    void
    abort
        ( void );

private:
    /// Hidden implementation.
    struct impl;

private:
    /// The hidden implementation instance.
    std::unique_ptr< impl > impl_;
};

} // namespace kademlia

#endif

