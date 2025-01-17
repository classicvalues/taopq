// Copyright (c) 2021-2022 Daniel Frey and Dr. Colin Hirsch
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PQ_ACCESS_MODE_HPP
#define TAO_PQ_ACCESS_MODE_HPP

namespace tao::pq
{
   enum class access_mode
   {
      default_access_mode,
      read_write,
      read_only
   };

}  // namespace tao::pq

#endif
