/*
*  This file is part of the program: Architecture Independent Profiler (AIP).
*
*  AIP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  AIP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Authors: Wim Heirman, Ghent University
*           Trevor Carlson, Ghent University
*           Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef __MEMORY_ACCESS_H
#define __MEMORY_ACCESS_H

#include "fixed_types.h"

namespace Memory
{
   struct Access
   {
      union {
         UInt64 address, virt, phys;
      };
      //static const UInt64 length = 8;
      void set(UInt64 address)
      {
         this->address = address;
      }
   };

   // Original interface, with virt/phys/length as separate fields:
   //inline Access make_access(UInt64 virt, UInt64 phys, UInt64 length = 8);

   inline Access make_access(UInt64 address)
   {
      Access access;
      access.set(address);
      return access;
   }
}

#endif // __MEMORY_ACCESS_H
