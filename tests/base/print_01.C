#include <tbox/MemoryDatabase.h>

#include <fstream>

int main(int argc, char **argv)
{
    using namespace SAMRAI;

    tbox::MemoryDatabase database("db");

    database.putInteger("a0", 42);
    database.putInteger("a1", 84);

    database.putDatabase("subdatabase");
    auto subdatabase = database.getDatabase("subdatabase");
    subdatabase->putDouble("f0", 3.14);
    subdatabase->putDouble("f1", 2.71);

    std::ofstream out("output");
    database.printClassData(out);
}
