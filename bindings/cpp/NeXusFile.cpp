#include <iostream>
#include "NeXusFile.hpp"
#include "NeXusException.hpp"

using namespace NeXus;
using std::string;

const string NULL_STR = "NULL";

File::File(const string filename, const NXaccess access) {
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

void File::initGroupDir() {
  int status = NXinitgroupdir(&(this->file_id));
  if (status != NX_OK) {
    throw Exception("NXinitgroupdir failed", status);
  }
}

std::pair<string, string> File::getNextEntry() {
  // set up temporary variables to get the information
  char name[NX_MAXNAMELEN];
  char class_name[NX_MAXNAMELEN];
  int datatype;

  int status = NXgetnextentry(&(this->file_id), name, class_name, &datatype);
  if (status == NX_OK) {
    string str_name(name);
    string str_class(class_name);
    return std::pair<string,string>(str_name, str_class);
  }
  else if (status == NX_EOD) {
    return std::pair<string,string>(NULL_STR, NULL_STR); // TODO return the correct thing
  }
  else if (status == NX_ERROR) {
    throw Exception("Failed NXgetnextentry", status);
  }
}

std::map<string, string> File::getEntries() {
  this->initGroupDir();

  std::map<string, string> result;

  std::pair<string,string> temp;
  while (true) {
    temp = this->getNextEntry();
    if (temp.first == NULL_STR && temp.second == NULL_STR) { // TODO this needs to be changed when getNextEntry is fixed
      break;
    }
    else {
      result.insert(temp);
    }
  }

  return result;
}
