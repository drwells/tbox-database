//
// File:        $URL: file:///usr/casc/samrai/repository/SAMRAI/tags/v-2-4-4/source/toolbox/base/Utilities.h $
// Package:     SAMRAI toolbox
// Copyright:   (c) 1997-2008 Lawrence Livermore National Security, LLC
// Revision:    $LastChangedRevision: 2249 $
// Modified:    $LastChangedDate: 2008-07-03 08:17:20 -0700 (Thu, 03 Jul 2008) $
// Description: Utility functions for error reporting, file manipulation, etc.
//

#ifndef included_tbox_Utilities
#define included_tbox_Utilities

#include <string>
#include <sstream>

namespace SAMRAI {
   namespace tbox {
/*!
 * Utilities is a Singleton class containing basic routines for error 
 * reporting, file manipulations, etc.
 */

struct Utilities
{
   /*!
    * Aborts the run after printing an error message with file and
    * linenumber information.
    */
   static void abort(const std::string &message, 
		     const std::string &filename,
		     const int line);

};


/*!
 * A statement that does nothing, for insure++ make it something 
 * more complex than a simple C null statement to avoid a warning.
 */
#ifdef __INSURE__
#define NULL_STATEMENT {}
#else
#define NULL_STATEMENT
#endif

/*!
 * A null use of a variable, use to avoid GNU compiler 
 * warnings about unused variables.
 */
#define NULL_USE(variable) (void)(variable)

/*!
 * Throw an error exception from within any C++ source code.  The 
 * macro argument may be any standard ostream expression.  The file and
 * line number of the abort are also printed.
 */
#ifndef LACKS_SSTREAM
#define TBOX_ERROR(X) do {					\
      std::ostringstream tboxos;					\
      tboxos << X << std::ends;					\
      SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
} while (0)
#else
#define TBOX_ERROR(X) do {					\
      std::ostrstream tboxos;					\
      tboxos << X << std::ends;					\
      SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
} while (0)
#endif

   /*!
    * Throw an error exception from within any C++ source code if the
    * given expression is not true.  This is a parallel-friendly version
    * of assert.
    * The file and line number of the abort are also printed.
    */
#ifdef HAVE_STRINGIZE
#ifndef LACKS_SSTREAM
#define TBOX_ASSERT(EXP) do {                                   \
      if ( !(EXP) ) {                                           \
         std::ostringstream tboxos;                             \
         tboxos << "Failed assertion: " << #EXP << std::ends;        \
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
      }                                                         \
} while (0)
#else
#define TBOX_ASSERT(EXP) do {                                   \
      if ( !(EXP) ) {                                           \
         std::ostrstream tboxos;                                \
         tboxos << "Failed assertion: " << #EXP << std::ends;        \
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
      }                                                         \
} while (0)
#endif
#else
#ifndef LACKS_SSTREAM
#define TBOX_ASSERT(EXP) do {                                   \
      if ( !(EXP) ) {                                           \
         std::ostringstream tboxos;                             \
         tboxos << "Failed assertion: " << std::ends;                \
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
      }                                                         \
} while (0)
#else
#define TBOX_ASSERT(EXP) do {                                   \
      if ( !(EXP) ) {                                           \
         std::ostrstream tboxos;                                \
         tboxos << "Failed assertion: " << std::ends;                \
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);\
      }                                                         \
} while (0)
#endif
#endif

/**
 * Macro for use when assertions are to be included
 * only when debugging.
 */
#ifdef DEBUG_CHECK_ASSERTIONS
#define TBOX_CHECK_ASSERT(EXP) TBOX_ASSERT(EXP)
#else
#define TBOX_CHECK_ASSERT(EXP) 
#endif


/**
 * Throw an error exception from within any C++ source code.  This is
 * is similar to TBOX_ERROR(), but is designed to be invoked after a
 * call to a PETSc library function.  In other words, it acts similarly
 * to the PETSc CHKERRQ(ierr) macro.
 */
#ifdef HAVE_PETSC

/*
 * In the following, "CHKERRCONTINUE(ierr);" will cause PETSc to print out
 * a stack trace that led to the error; this may be useful for debugging.
 */
 
#ifndef LACKS_SSTREAM
#define PETSC_SAMRAI_ERROR(ierr) do {						\
      if (ierr) {                                   				\
         std::ostringstream tboxos;							\
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);	\
      } 									\
} while (0)
#else
#define PETSC_SAMRAI_ERROR(ierr) do {						\
      if (ierr) {                                   				\
         std::ostrstream tboxos;							\
         CHKERRCONTINUE(ierr); 							\
         SAMRAI::tbox::Utilities::abort(tboxos.str(), __FILE__, __LINE__);	        \
      } 									\
} while (0)
#endif
#endif

}
}

#endif
