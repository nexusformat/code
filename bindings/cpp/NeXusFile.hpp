#ifndef NEXUSFILE_HPP
#define NEXUSFILE_HPP 1

#include <string>
#include "napi.h"

namespace NeXus {
  //  typedef enum NXnumtype {BLAH};

  class File
  {
  private:
    NXhandle file_id;

  public:
    File(const std::string filename, const NXaccess access = NXACC_READ);
    
    ~File();
  };
};

#endif
