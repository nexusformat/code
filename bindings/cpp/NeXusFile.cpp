#include <iostream>
#include "NeXusFile.hpp"
#include "NeXusException.hpp"

using namespace NeXus;

File::File(const std::string filename, const NXaccess access) {
  if (filename.empty()) {
    throw Exception("Filename specified is empty constructor");
  }

  NXstatus status = NXopen(filename.c_str(), access, &(this->file_id));
  if (status != NX_OK) {
    throw Exception("Failed to open file " + filename);
  }
}

File::~File() {
  if (this->file_id != NULL) {
    NXstatus status = NXclose(&(this->file_id));
    this->file_id = NULL;
    if (status != NX_OK) {
      throw Exception("Failed to close file");
    }
  }
}
