CMAKE_MINIMUM_REQUIRED(VERSION 3.15.0)

PROJECT(tdatabase
  DESCRIPTION "Just SAMRAI::tbox::Database"
  VERSION "2.4.4"
  LANGUAGES C CXX)

INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CMakePackageConfigHelpers)
INCLUDE(GNUInstallDirs)

OPTION(BUILD_SHARED_LIBS "Whether or not to build shared libraries." ON)

MESSAGE(STATUS "")
MESSAGE(STATUS "Setting up MPI")
# CMake's MPI detection fails when a user specifies CMAKE_C_COMPILER=mpicc et
# al pointing to a custom installation of MPI when there is a system copy
# available too. Try to detect this and set MPI_ROOT correctly.
#
# Only continue if we are potentially using an explicitly provided MPI compiler
# wrapper
IF(NOT "${CMAKE_CXX_COMPILER}" STREQUAL "")
  # If it cannot compile a basic MPI application then it isn't an MPI compiler
  # wrapper
  CHECK_CXX_SOURCE_COMPILES(
  "
  #include <mpi.h>
  int main(int argc, char **argv)
  {
    MPI_Init(&argc, &argv);
    MPI_Finalize();
  }
  "
  CXX_COMPILER_SUPPORTS_MPI)

  IF(${CXX_COMPILER_SUPPORTS_MPI})
    MESSAGE(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} is an MPI wrapper")
    # It looks like we have a compiler wrapper: overide MPI_ROOT
    IF(NOT "${MPI_ROOT}" STREQUAL "")
      MESSAGE(FATAL_ERROR "If MPI compiler wrappers are used then MPI_ROOT must not be set.")
    ENDIF()
    GET_FILENAME_COMPONENT(_compiler_directory ${CMAKE_CXX_COMPILER} DIRECTORY)
    SET(MPI_ROOT "${_compiler_directory}/../")
    MESSAGE(STATUS "Using MPI compiler wrapper to set MPI_ROOT=${MPI_ROOT}")
  ELSE()
    MESSAGE(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} is not an MPI compiler wrapper - CMake will use the normal MPI detection sequence")
  ENDIF()
ELSEIF((NOT $ENV{MPI_HOME} STREQUAL "") AND ("${MPI_ROOT}" STREQUAL ""))
  # if MPI_HOME is set then we should use that instead (environment modules
  # usually set up MPI this way)
  SET(MPI_ROOT $ENV{MPI_HOME})
  MESSAGE(STATUS "Using environment variable MPI_HOME=$ENV{MPI_HOME} to set MPI_ROOT=${MPI_ROOT}")
ENDIF()

# MPI_ROOT is now either set to a valid value (we want a specific MPI) or empty
# (use default search paths) so we can proceed with setting up MPI
IF(NOT ${MPI_ROOT} STREQUAL "")
  # CMake wants to detect MPI with MPI_HOME, not MPI_ROOT
  SET(MPI_HOME ${MPI_ROOT})
  MESSAGE(STATUS "Setting up MPI at location ${MPI_ROOT}")
ENDIF()
# We never use Fortran with MPI so try to skip it. We also never use the
# deprecated C++ MPI bindings, but we want things to work with mpic++, so keep
# it around.
FIND_PACKAGE(MPI REQUIRED COMPONENTS C CXX)

SET(TDATABASE_SOURCE
source/database/DatabaseBox.C
source/database/Database.C
source/database/MemoryDatabase.C
source/memory/Arena.C
source/memory/ReferenceCounter.C
source/memory/DescribedClass.C
source/base/Utilities.C
source/inputdb/Grammar.C
source/inputdb/InputManager.C
source/inputdb/Parser.C
source/inputdb/Scanner.C
)

ADD_LIBRARY(tdatabase)
TARGET_SOURCES(tdatabase PRIVATE ${TDATABASE_SOURCE})
TARGET_LINK_LIBRARIES(tdatabase PUBLIC MPI::MPI_C)

# If openMPI is built with the old C++ bindings enabled then we need to link
# against them, since we don't consistently define OMPI_SKIP_MPICXX. We always
# need this with libMesh since libMesh is underlinked.
IF(${MPI_MPICXX_FOUND})
  TARGET_LINK_LIBRARIES(tdatabase PUBLIC MPI::MPI_CXX)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(tdatabase PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/;${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>)

ADD_SUBDIRECTORY(tests)
