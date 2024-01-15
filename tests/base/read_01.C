#include <tbox/InputManager.h>
#include <tbox/MemoryDatabase.h>

#include <fstream>

int main(int argc, char **argv)
{
   using namespace SAMRAI;

   tbox::Pointer<tbox::MemoryDatabase> database = new tbox::MemoryDatabase("db");

   tbox::InputManager::parseInputFile(argv[1], database);

   std::ofstream out("output");
   database->printClassData(out);
}
