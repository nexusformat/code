#include <sstream>
#include "NeXusFile.hpp"
#include "NeXusException.hpp"

using namespace NeXus;
using std::map;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

const string NULL_STR = "NULL";

template <typename NumT>
string toString(vector<NumT> & data) {
  stringstream result;
  result << "[";
  size_t size = data.size();
  for (size_t i = 0; i < size; i++) {
    result << data[i];
    if (i+1 < size) {
      result << ",";
    }
  }
  result << "]";
  return result.str();
}

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
  this->openGroup(name, class_name);
}

void File::openGroup(const string & name, const string & class_name) {
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

void File::openPath(const string & path) {
  if (path.empty()) {
    throw Exception("Supplied empty path to openPath");
  }
  NXstatus status = NXopenpath(this->file_id, path.c_str());
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXopenpath(" << path << ") failed";
    throw Exception(msg.str(), status);
  }
}

void File::openGroupPath(const string & path) {
  if (path.empty()) {
    throw Exception("Supplied empty path to openGroupPath");
  }
  NXstatus status = NXopengrouppath(this->file_id, path.c_str());
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXopengrouppath(" << path << ") failed";
    throw Exception(msg.str(), status);
  }
}

void File::closeGroup() {
  NXstatus status = NXclosegroup(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXclosegroup failed", status);
  }
}

void File::makeData(const string & name, NXnumtype type, vector<int> & dims) {
  // error check the parameters
  if (name.empty()) {
    throw Exception("Supplied empty label to makeData");
  }
  if (dims.empty()) {
    throw Exception("Supplied empty dimensions to makeData");
  }

  // do the work
  NXstatus status = NXmakedata(this->file_id, name.c_str(), (int)type,
                               dims.size(), &(*(dims.begin())));
  // report errors
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXmakedata(" << name << ", " << type << ", " << dims.size()
        << ", " << toString(dims) << ") failed";
    throw Exception(msg.str(), status);
  }
  this->openData(name);
}

void File::makeCompData(const string & name, NXnumtype type,
                        vector<int> & dims, NXcompression comp,
                        vector<int> & bufsize) {
  // error check the parameters
  if (name.empty()) {
    throw Exception("Supplied empty name to makeCompData");
  }
  if (dims.empty()) {
    throw Exception("Supplied empty dimensions to makeCompData");
  }
  if (bufsize.empty()) {
    throw Exception("Supplied empty bufsize to makeCompData");
  }
  if (dims.size() != bufsize.empty()) {
    stringstream msg;
    msg << "Supplied dims rank=" << dims.size()
        << " must match supplied bufsize rank=" << bufsize.size()
        << "in makeCompData";
    throw Exception(msg.str());
  }

  // do the work
  NXstatus status = NXcompmakedata(this->file_id, name.c_str(), (int)type,
                                   dims.size(), &(*(dims.begin())), (int)comp,
                                   &(*(bufsize.begin())));

  // report errors
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXcompmakedata(" << name << ", " << type << ", " << dims.size()
        << ", " << toString(dims) << ", " << comp << ", " << toString(bufsize)
        << ") failed";
    throw Exception(msg.str(), status);
  }
  this->openData(name);
}

void File::compress(NXcompression comp) {
  stringstream msg;
  msg << "compress(" << comp << ") is depricated - use makeCompData()";
  throw Exception(msg.str());
}

void File::openData(const string & name) {
  if (name.empty()) {
    throw Exception("Supplied empty name to openData");
  }
  NXstatus status = NXopendata(this->file_id, name.c_str());
  if (status != NX_OK) {
    throw Exception("NXopendata(" + name + ") failed", status);
  }
}

void File::closeData() {
  NXstatus status = NXclosedata(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXclosedata() failed", status);
  }
}

void File::putData(void * data) {
  if (data == NULL) {
    throw Exception("Data specified as null in putData");
  }
  NXstatus status = NXputdata(this->file_id, data);
  if (status != NX_OK) {
    throw Exception("NXputdata(void *) failed", status);
  }
}

void File::initGroupDir() {
  int status = NXinitgroupdir(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXinitgroupdir failed", status);
  }
}

pair<string, string> File::getNextEntry() {
  // set up temporary variables to get the information
  char name[NX_MAXNAMELEN];
  char class_name[NX_MAXNAMELEN];
  int datatype;

  NXstatus status = NXgetnextentry(this->file_id, name, class_name,
                                   &datatype);
  if (status == NX_OK) {
    string str_name(name);
    string str_class(class_name);
    return pair<string,string>(str_name, str_class);
  }
  else if (status == NX_EOD) {
    return pair<string,string>(NULL_STR, NULL_STR); // TODO return the correct thing
  }
  else {
    throw Exception("NXgetnextentry failed", status);
  }
}

map<string, string> File::getEntries() {
  this->initGroupDir();

  map<string, string> result;

  pair<string,string> temp;
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
