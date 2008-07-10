#include <sstream>
#include "NeXusFile.hpp"
#include "NeXusException.hpp"

using namespace NeXus;
using std::string;
using std::stringstream;

const string NULL_STR = "NULL";

File::File(const string filename, const NXaccess access) {
  if (filename.empty()) {
    throw Exception("Filename specified is empty constructor");
  }

  NXstatus status = NXopen(filename.c_str(), access, &(this->file_id));
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXopen(" << filename << ", "  << access << ") failed";
    throw Exception(msg.str(), status);
  }
}

File::~File() {
  if (this->file_id != NULL) {
    NXstatus status = NXclose(&(this->file_id));
    this->file_id = NULL;
    if (status != NX_OK) {
      throw Exception("NXclose failed", status);
    }
  }
}

void File::flush() {
  NXstatus status = NXflush(&(this->file_id));
  if (status != NX_OK) {
    throw Exception("NXflush failed", status);
  }
}

void File::makeGroup(const string & name, const string & class_name) {
  if (name.empty()) {
    throw Exception("Supplied empty name to makeGroup");
  }
  if (class_name.empty()) {
    throw Exception("Supplied empty class name to makeGroup");
  }
  NXstatus status = NXmakegroup(this->file_id, name.c_str(),
                                class_name.c_str());
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXmakegroup(" << name << ", " << class_name << ") failed";
    throw Exception(msg.str(), status);
  }
}

void File::openGroup(const string name, const string class_name) {
  if (name.empty()) {
    throw Exception("Supplied empty name to openGroup");
  }
  if (class_name.empty()) {
    throw Exception("Supplied empty class name to openGroup");
  }
  NXstatus status = NXopengroup(this->file_id, name.c_str(),
                                class_name.c_str());
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXopengroup(" << name << ", " << class_name << ") failed";
    throw Exception(msg.str(), status);
  }
}

void File::initGroupDir() {
  int status = NXinitgroupdir(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXinitgroupdir failed", status);
  }
}

std::pair<string, string> File::getNextEntry() {
  // set up temporary variables to get the information
  char name[NX_MAXNAMELEN];
  char class_name[NX_MAXNAMELEN];
  int datatype;

  NXstatus status = NXgetnextentry(this->file_id, name, class_name,
                                   &datatype);
  if (status == NX_OK) {
    string str_name(name);
    string str_class(class_name);
    return std::pair<string,string>(str_name, str_class);
  }
  else if (status == NX_EOD) {
    return std::pair<string,string>(NULL_STR, NULL_STR); // TODO return the correct thing
  }
  else {
    throw Exception("NXgetnextentry failed", status);
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
