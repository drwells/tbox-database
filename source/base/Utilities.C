//
// File:        $URL:
// file:///usr/casc/samrai/repository/SAMRAI/tags/v-2-4-4/source/toolbox/base/Utilities.C
// $ Package:     SAMRAI toolbox Copyright:   (c) 1997-2008 Lawrence Livermore
// National Security, LLC Revision:    $LastChangedRevision: 2039 $ Modified:
// $LastChangedDate: 2008-03-11 13:23:52 -0700 (Tue, 11 Mar 2008) $ Description:
// Utility functions for error reporting, file manipulation, etc.
//

#include "tbox/Utilities.h"

#include <iostream>

namespace SAMRAI {
namespace tbox {
void Utilities::abort(const std::string &message, const std::string &filename,
                      const int line) {
  std::cerr << "Aborting from " << filename << " at line " << line << ":\n"
            << message << '\n';

  std::abort();
}

} // namespace tbox
} // namespace SAMRAI
