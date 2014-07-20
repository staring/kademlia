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

#include "message.hpp"

#include "helpers/common.hpp"

#include <random>

namespace k = kademlia;
namespace kd = k::detail;

/**
 */
BOOST_AUTO_TEST_SUITE( test_message )

BOOST_AUTO_TEST_CASE( can_serialize_header )
{
    std::default_random_engine random_engine;

    kd::header const header_out =
        { kd::header::V1
        , kd::header::FIND_VALUE_RESPONSE
        , kd::id{ random_engine }
        , kd::id{ random_engine } };

    kd::buffer buffer;
    kd::serialize( header_out, buffer );

    kd::header header_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, header_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL( header_out.version_, header_in.version_ );
    BOOST_REQUIRE_EQUAL( header_out.type_, header_in.type_);

    BOOST_REQUIRE_EQUAL_COLLECTIONS( header_out.source_id_.begin()
                                   , header_out.source_id_.end()
                                   , header_in.source_id_.begin()
                                   , header_in.source_id_.end() );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( header_out.random_token_.begin()
                                   , header_out.random_token_.end()
                                   , header_in.random_token_.begin()
                                   , header_in.random_token_.end() );
}
 
BOOST_AUTO_TEST_CASE( can_serialize_find_peer_request_body )
{
    std::default_random_engine random_engine;

    kd::find_peer_request_body const body_out =
            { kd::id{ random_engine } };

    kd::buffer buffer;
    kd::serialize( body_out, buffer );

    kd::find_peer_request_body body_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, body_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( body_out.peer_to_find_id_.begin()
                                   , body_out.peer_to_find_id_.end()
                                   , body_in.peer_to_find_id_.begin()
                                   , body_in.peer_to_find_id_.end() );
}

BOOST_AUTO_TEST_CASE( can_serialize_find_peer_response_body )
{
    std::default_random_engine random_engine;

    kd::find_peer_response_body body_out;
   
    for ( std::size_t i = 0; i < 10; ++ i)
    {
        static std::string const IPS[2] =
            { "::1"
            , "127.0.0.1" };

        kd::peer new_peer = 
            { kd::id{ random_engine }
            , { boost::asio::ip::address::from_string( IPS[ i % 2 ] )
              , std::uint16_t( 1024 + i ) } };

        body_out.peers_.push_back( std::move( new_peer ) );
    } 

    kd::buffer buffer;
    kd::serialize( body_out, buffer );

    kd::find_peer_response_body body_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, body_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL( body_out.peers_.size(), body_in.peers_.size() );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( body_out.peers_.begin()
                                   , body_out.peers_.end()
                                   , body_in.peers_.begin()
                                   , body_in.peers_.end() );
}
 
BOOST_AUTO_TEST_CASE( can_serialize_find_value_request_body )
{
    std::default_random_engine random_engine;

    kd::find_value_request_body const body_out{ kd::id{ random_engine } };
   
    kd::buffer buffer;
    kd::serialize( body_out, buffer );

    kd::find_value_request_body body_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, body_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL( body_out.value_to_find_
                       , body_in.value_to_find_ );
}
 
BOOST_AUTO_TEST_CASE( can_serialize_find_value_response_body )
{
    kd::find_value_response_body body_out
    { std::vector< std::uint8_t >( 4096 ) };
   
    std::generate( body_out.data_.begin()
                 , body_out.data_.end()
                 , std::rand );

    kd::buffer buffer;
    kd::serialize( body_out, buffer );

    kd::find_value_response_body body_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, body_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( body_out.data_.begin()
                                   , body_out.data_.end()
                                   , body_in.data_.begin()
                                   , body_in.data_.end() );
}

BOOST_AUTO_TEST_CASE( can_serialize_store_value_request_body )
{
    std::default_random_engine random_engine;

    kd::store_value_request_body body_out
            { kd::id{ random_engine }
            , std::vector< std::uint8_t >( 4096 ) };
   
    std::generate( body_out.data_value_.begin()
                 , body_out.data_value_.end()
                 , std::rand );

    kd::buffer buffer;
    kd::serialize( body_out, buffer );

    kd::store_value_request_body body_in;
    auto i = buffer.cbegin(), e = buffer.cend();
    BOOST_REQUIRE( ! kd::deserialize( i, e, body_in ) );
    BOOST_REQUIRE( i == e );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( body_out.data_key_hash_.begin()
                                   , body_out.data_key_hash_.end()
                                   , body_in.data_key_hash_.begin()
                                   , body_in.data_key_hash_.end() );

    BOOST_REQUIRE_EQUAL_COLLECTIONS( body_out.data_value_.begin()
                                   , body_out.data_value_.end()
                                   , body_in.data_value_.begin()
                                   , body_in.data_value_.end() );
}
        
BOOST_AUTO_TEST_SUITE_END()

