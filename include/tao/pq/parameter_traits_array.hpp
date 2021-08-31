// Copyright (c) 2021 Daniel Frey and Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/taopq/

#ifndef TAO_PQ_PARAMETER_TRAITS_ARRAY_HPP
#define TAO_PQ_PARAMETER_TRAITS_ARRAY_HPP

#include <array>
#include <cstddef>
#include <list>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include <tao/pq/internal/to_traits.hpp>
#include <tao/pq/parameter_traits.hpp>

namespace tao::pq
{
   template< typename >
   inline constexpr bool is_array_parameter = false;

   template< typename T, std::size_t N >
   inline constexpr bool is_array_parameter< std::array< T, N > > = true;

   template< typename... Ts >
   inline constexpr bool is_array_parameter< std::list< Ts... > > = true;

   template< typename... Ts >
   inline constexpr bool is_array_parameter< std::set< Ts... > > = true;

   template< typename... Ts >
   inline constexpr bool is_array_parameter< std::unordered_set< Ts... > > = true;

   template< typename... Ts >
   inline constexpr bool is_array_parameter< std::vector< Ts... > > = true;

   namespace internal
   {
      template< typename T >
      auto to_array( std::string& data, const T& v )
         -> std::enable_if_t< !is_array_parameter< T > >
      {
         const auto t = internal::to_traits( v );
         static_assert( t.columns == 1 );
         t.template element< 0 >( data );
      }

      template< typename T >
      auto to_array( std::string& data, const T& v )
         -> std::enable_if_t< is_array_parameter< T > >
      {
         data += '{';
         if( v.empty() ) {
            data += '}';
         }
         else {
            for( const auto& e : v ) {
               internal::to_array( data, e );
               data += ',';
            }
            *data.rbegin() = '}';
         }
      }

   }  // namespace internal

   template< typename T >
   struct parameter_traits< T, std::enable_if_t< is_array_parameter< T > > >
   {
   private:
      std::string m_data;

   public:
      explicit parameter_traits( const T& v )
      {
         internal::to_array( m_data, v );
      }

      static constexpr std::size_t columns = 1;

      template< std::size_t I >
      [[nodiscard]] static constexpr auto type() noexcept -> oid
      {
         return oid::invalid;
      }

      template< std::size_t I >
      [[nodiscard]] auto value() const noexcept -> const char*
      {
         return m_data.c_str();
      }

      template< std::size_t I >
      [[nodiscard]] static constexpr auto length() noexcept -> int
      {
         return 0;
      }

      template< std::size_t I >
      [[nodiscard]] static constexpr auto format() noexcept -> int
      {
         return 0;
      }

      template< std::size_t I >
      void copy_to( std::string& data ) const
      {
         internal::table_writer_append( data, m_data );
      }
   };

}  // namespace tao::pq

#endif