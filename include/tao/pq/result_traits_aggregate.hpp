// Copyright (c) 2021 Daniel Frey and Dr. Colin Hirsch
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PQ_RESULT_TRAITS_AGGREGATE_HPP
#define TAO_PQ_RESULT_TRAITS_AGGREGATE_HPP

#include <tao/pq/internal/aggregate.hpp>
#include <tao/pq/is_aggregate.hpp>
#include <tao/pq/result_traits.hpp>

namespace tao::pq
{
   namespace internal
   {
      template< typename... Ts >
      struct decay_tuple;

      template< typename... Ts >
      struct decay_tuple< std::tuple< Ts... > >
      {
         using type = std::tuple< std::decay_t< Ts >... >;
      };

      template< typename T, typename = typename decay_tuple< decltype( aggregate::tie( std::declval< T >() ) ) >::type >
      struct result_traits_aggregate;

      template< typename T, typename... Ts >
      struct result_traits_aggregate< T, std::tuple< Ts... > >
      {
         static constexpr std::size_t size{ (0 + ... + result_traits_size< Ts >)};

         template< typename Row, std::size_t... Ns >
         [[nodiscard]] static auto from( const Row& row, std::index_sequence< Ns... > /*unused*/ )
         {
            return T{ row.template get< Ts >( Ns )... };
         }

         template< typename Row >
         [[nodiscard]] static auto from( const Row& row )
         {
            return result_traits_aggregate::from( row, exclusive_scan_t< std::index_sequence< result_traits_size< Ts >... > >() );
         }
      };

   }  // namespace internal

   template< typename T >
   struct result_traits< T, std::enable_if_t< is_aggregate_result< T > > >
      : internal::result_traits_aggregate< T >
   {};

}  // namespace tao::pq

#endif
