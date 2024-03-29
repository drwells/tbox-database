//
// File:	$URL: file:///usr/casc/samrai/repository/SAMRAI/tags/v-2-4-4/source/toolbox/memory/ReferenceCounter.C $
// Package:	SAMRAI toolbox for memory management
// Copyright:	(c) 1997-2008 Lawrence Livermore National Security, LLC
// Revision:	$LastChangedRevision: 1917 $
// Modified:	$LastChangedDate: 2008-01-25 13:28:01 -0800 (Fri, 25 Jan 2008) $
// Description:	Reference counting class for Array and Pointer
//

#include "tbox/ReferenceCounter.h"
#include "tbox/Arena.h"

#include <stdlib.h>

namespace SAMRAI {
   namespace tbox {

ReferenceCounter::ReferenceCounter(Arena *arena,
                                             ReferenceCounter *counter)
{
   if (!arena) {
      d_references = 1;
      d_arena      = (Arena *) NULL;
      d_counter    = (ReferenceCounter *) NULL;
   } else {
      d_references = 1;
      d_arena      = arena;
      d_counter    = counter;
      if (d_counter) d_counter->d_references++;
   }
}

ReferenceCounter::~ReferenceCounter()
{
   if ((d_counter) && (--d_counter->d_references == 0)) {
      if (d_arena) {
         if (d_counter->d_arena) {
            d_arena->~Arena();
            d_counter->d_arena->free(d_arena);
         } else {
            delete d_arena;
         }
      }
      delete d_counter;
   }
}

}
}
