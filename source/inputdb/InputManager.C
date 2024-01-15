//
// File:	$URL: file:///usr/casc/samrai/repository/SAMRAI/tags/v-2-4-4/source/toolbox/inputdb/InputManager.C $
// Package:	SAMRAI toolbox
// Copyright:	(c) 1997-2008 Lawrence Livermore National Security, LLC
// Revision:	$LastChangedRevision: 1917 $
// Modified:	$LastChangedDate: 2008-01-25 13:28:01 -0800 (Fri, 25 Jan 2008) $
// Description:	An input manager singleton class that parses input files
//

#include "tbox/InputManager.h"
#include "tbox/Parser.h"
#include "tbox/Utilities.h"

#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#ifndef NULL
#define NULL (0)
#endif

namespace SAMRAI {
   namespace tbox {

/*
*************************************************************************
*									*
* Parse the specified input file into the given database.		*
*									*
*************************************************************************
*/

void InputManager::parseInputFile(
   const std::string& filename, Pointer<InputDatabase> db)
{
   int mpi_has_been_started = 0;
   int ierr                 = MPI_Initialized(&mpi_has_been_started);
   TBOX_ASSERT(ierr == MPI_SUCCESS);

   FILE* fstream = NULL;
   int rank = 0;

   if (mpi_has_been_started)
   {
      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      TBOX_ASSERT(ierr == MPI_SUCCESS);
   }
   if (rank == 0) {
      fstream = fopen(filename.c_str(), "r");
   }

   int worked = (fstream ? 1 : 0);

   if (mpi_has_been_started)
   {
      ierr = MPI_Bcast(&worked, 1, MPI_INT, 0, MPI_COMM_WORLD);
      TBOX_ASSERT(ierr == MPI_SUCCESS);
   }
   if (!worked) {
      TBOX_ERROR("InputManager:: Could not open input file``" <<
                  filename.c_str() << "''\n");
   }

   /*
    * Parse input file.
    */
   Parser parser;
   const int errors = parser.parse(filename, fstream, db);
   const int warnings = parser.getNumberWarnings();

   if (errors > 0) {
      std::cerr << "InputManager: Errors = " << errors
                << ", Warnings = " << warnings
                << "\n when parsing input file = " << filename << std::endl;
      db->printClassData(std::cerr);
      TBOX_ERROR("InputManager exiting..." << std::endl);
   }
   if (warnings > 0) {
      std::cerr << "InputManager: Warnings  = " << warnings
                << "\n when parsing input file = " << filename << std::endl;
   }

   if (fstream) fclose(fstream);
}

}
}
