// Copyright (c) 2021 Daniel Frey and Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/taopq/

#ifndef TAO_PQ_PARAMETER_TRAITS_PAIR_HPP
#define TAO_PQ_PARAMETER_TRAITS_PAIR_HPP

#include <type_traits>
#include <utility>

#include <tao/pq/internal/gen.hpp>
#include <tao/pq/parameter_traits.hpp>

namespace tao::pq
{
   template< typename T, typename U >
   struct parameter_traits< std::pair< T, U > >
   {
   private:
      using first_t = parameter_traits< std::decay_t< T > >;
      using second_t = parameter_traits< std::decay_t< U > >;

      using pair_t = std::pair< first_t, second_t >;
      pair_t m_pair;

      using gen = internal::gen< first_t::columns, second_t::columns >;

   public:
      explicit parameter_traits( const std::pair< T, U >& pair ) noexcept( noexcept( pair_t( pair ) ) )
         : m_pair( pair )
      {}

      explicit parameter_traits( std::pair< T, U >&& pair ) noexcept( noexcept( pair_t( std::move( pair ) ) ) )
         : m_pair( std::move( pair ) )
      {}

      static constexpr std::size_t columns = first_t::columns + second_t::columns;

      template< std::size_t I >
      [[nodiscard]] constexpr auto type() const noexcept( noexcept( std::get< gen::template outer< I > >( m_pair ).template type< gen::template inner< I > >() ) ) -> oid
      {
         return std::get< gen::template outer< I > >( m_pair ).template type< gen::template inner< I > >();
      }

      template< std::size_t I >
      [[nodiscard]] constexpr auto value() const noexcept( noexcept( std::get< gen::template outer< I > >( m_pair ).template value< gen::template inner< I > >() ) ) -> const char*
      {
         return std::get< gen::template outer< I > >( m_pair ).template value< gen::template inner< I > >();
      }

      template< std::size_t I >
      [[nodiscard]] constexpr auto length() const noexcept( noexcept( std::get< gen::template outer< I > >( m_pair ).template length< gen::template inner< I > >() ) ) -> int
      {
         return std::get< gen::template outer< I > >( m_pair ).template length< gen::template inner< I > >();
      }

      template< std::size_t I >
      [[nodiscard]] constexpr auto format() const noexcept( noexcept( std::get< gen::template outer< I > >( m_pair ).template format< gen::template inner< I > >() ) ) -> int
      {
         return std::get< gen::template outer< I > >( m_pair ).template format< gen::template inner< I > >();
      }

      template< std::size_t I >
      void copy_to( std::string& data ) const
      {
         std::get< gen::template outer< I > >( m_pair ).template copy_to< gen::template inner< I > >( data );
      }
   };

}  // namespace tao::pq

#endif
