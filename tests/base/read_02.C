#include <tbox/InputManager.h>
#include <tbox/MemoryDatabase.h>

#include <fstream>

#include <mpi.h>

int main(int argc, char **argv)
{
   using namespace SAMRAI;

   int ierr = MPI_Init(&argc, &argv);
   TBOX_ASSERT(ierr == MPI_SUCCESS);

   {
      tbox::Pointer<tbox::MemoryDatabase> database = new tbox::MemoryDatabase("db");

      tbox::InputManager::parseInputFile(argv[1], database);

      int rank = 0;
      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      TBOX_ASSERT(ierr == MPI_SUCCESS);
      if (rank == 0)
      {
         std::ofstream out("output");
         database->printClassData(out);
      }
   }

   MPI_Finalize();
}
