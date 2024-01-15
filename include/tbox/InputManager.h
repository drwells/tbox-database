//
// File:	$URL: file:///usr/casc/samrai/repository/SAMRAI/tags/v-2-4-4/source/toolbox/inputdb/InputManager.h $
// Package:	SAMRAI toolbox
// Copyright:	(c) 1997-2008 Lawrence Livermore National Security, LLC
// Revision:	$LastChangedRevision: 2132 $
// Modified:	$LastChangedDate: 2008-04-14 14:51:47 -0700 (Mon, 14 Apr 2008) $
// Description:	An input manager singleton class that parses input files
//

#ifndef included_tbox_InputManager
#define included_tbox_InputManager

#include "tbox/InputDatabase.h"
#include "tbox/Pointer.h"
#ifndef included_String
#include <string>
#define included_String
#endif


namespace SAMRAI {
   namespace tbox {


/**
 * Class InputManager parses an input file and returns the associated
 * database.  This manager class hides the complexity of opening the
 * input file, creating the parser, and populating the database with
 * values.  The input manager is simple enough that it did not need
 * to be implemented as a singleton class; however, it was implemented
 * as a singleton to be consistent with the restart manager class.
 *
 * All processors must call the parsing routines.  Any errors are reported
 * to pout and will result in termination of the program.
 *
 * The input file will generally have the following format.  For each
 * object that requires user specified input, an entry of the following 
 * format should be included in the input file.
 *
 * \verbatim
 * Object_entry {
 *    keyword1 = <value1>     // maybe some end line comments
 *    keyword2 = <value2>
 *    nested_input_keyword {
 *       nested_data_keyword1 = <value1> 
 *    }
 *     \ldots
 * }
 * \endverbatim
 * 
 * For convenience, the input parser also supports C/C++ style comments,
 * "include" files, and some expression evaluation.
 *
 * @warning Unlike SAMRAI, this fork does not use singletons so all member
 * functions of this object are static and most are not implemented.
 */
class InputManager
{
public:
   InputManager() = delete;

   /**
    * Parse data from the specified file into the existing database.
    */
   static void parseInputFile(
      const std::string& filename, Pointer<InputDatabase> db);
};


}
}

#endif
