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
string toString(const vector<NumT> & data) {
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

/**
 * This function returns the NXnumtype given a concrete number. The
 * template is present to cover cases that are not otherwised
 * explicitly implemented.
 */
template <typename NumT>
NXnumtype getType(NumT number) {
  stringstream msg;
  msg << "Do not know type of " << number;
  throw Exception(msg.str());
}

NXnumtype getType(float number) {
  return FLOAT32;
}

NXnumtype getType(double number) {
  return FLOAT64;
}

NXnumtype getType(int8_t number) {
  return INT8;
}

NXnumtype getType(uint8_t number) {
  return UINT8;
}

NXnumtype getType(int16_t number) {
  return INT16;
}

NXnumtype getType(uint16_t number) {
  return UINT16;
}

NXnumtype getType(int32_t number) {
  return INT32;
}

NXnumtype getType(uint32_t number) {
  return UINT32;
}

NXnumtype getType(int64_t number) {
  return INT64;
}

NXnumtype getType(uint64_t number) {
  return UINT64;
}

NXnumtype getType(char number) {
  return CHAR;
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

void File::makeData(const string & name, NXnumtype type,
                    const vector<int> & dims) {
  // error check the parameters
  if (name.empty()) {
    throw Exception("Supplied empty label to makeData");
  }
  if (dims.empty()) {
    throw Exception("Supplied empty dimensions to makeData");
  }

  // do the work
  NXstatus status = NXmakedata(this->file_id, name.c_str(), (int)type,
                               dims.size(), const_cast<int *>(&(dims[0])));
  // report errors
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXmakedata(" << name << ", " << type << ", " << dims.size()
        << ", " << toString(dims) << ") failed";
    throw Exception(msg.str(), status);
  }
  this->openData(name);
}

void File::writeData(const string & name, const string & value) {
  if (value.empty()) {
    throw Exception("Supplied empty value to makeData");
  }
  vector<int> dims;
  dims.push_back(value.size());
  this->makeData(name, CHAR, dims);

  string my_value(value);
  this->putData(&(my_value[0]));

  this->closeData();
}

template <typename NumT>
void File::writeData(const string & name, const vector<NumT> & value) {
  vector<int> dims;
  dims.push_back(value.size());
  this->writeData(name, value, dims);
}

template <typename NumT>
void File::writeData(const string & name, const vector<NumT> & value,
                     const vector<int> & dims) {
  this->makeData(name, getType(value[0]), dims);
  this->putData(value);
  this->closeData();
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

void File::putData(const void * data) {
  if (data == NULL) {
    throw Exception("Data specified as null in putData");
  }
  NXstatus status = NXputdata(this->file_id, const_cast<void *>(data));
  if (status != NX_OK) {
    throw Exception("NXputdata(void *) failed", status);
  }
}

template <typename NumT>
void File::putData(const vector<NumT> & data) {
  if (data.empty()) {
    throw Exception("Supplied empty data to putData");
  }
  this->putData(&(data[0]));
}

void File::putAttr(const string & name, const void * data, int length,
                   NXnumtype type) {
  if (name.empty()) {
    throw Exception("Supplied empty name to putAttr");
  }
  if (length <= 0) {
    stringstream msg;
    msg << "Length must be greater than zero. Found " << length;
    throw Exception(msg.str());
  }
  NXstatus status = NXputattr(this->file_id, name.c_str(),
                              const_cast<void *>(data), length, (int)type);
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXputattr(" << name << ", data, " << length << ", " << type 
        << ") failed";
    throw Exception(msg.str(), status);
  }
}

template <typename NumT>
void File::putAttr(const std::string & name, const NumT value) {
  NumT data[1];
  data[0] = value;
  this->putAttr(name, &value, 1, getType(value));
}

void File::putAttr(const char * name, const char * value) {
  if (name == NULL) {
    throw Exception("Specified name as null to putAttr");
  }
  if (value == NULL) {
    throw Exception("Specified value as null to putAttr");
  }
  string s_name(name);
  string s_value(value);
  this->putAttr(s_name, s_value);
}

void File::putAttr(const std::string & name, const std::string value) {
  if (value.empty()) {
    throw Exception("Supplied empty value to putAttr");
  }
  string my_value(value);
  this->putAttr(name, &(my_value[0]), my_value.size(), CHAR);
}

void File::putSlab(void * data, vector<int> & start,vector<int> & size) {
  if (data == NULL) {
    throw Exception("Data specified as null in putSlab");
  }
  if (start.empty()) {
    throw Exception("Supplied empty start to putSlab");
  }
  if (size.empty()) {
    throw Exception("Supplied empty size to putSlab");
  }
  if (start.size() != size.size()) {
    stringstream msg;
    msg << "Supplied start rank=" << start.size()
        << " must match supplied size rank=" << size.size()
        << "in putSlab";
    throw Exception(msg.str());
  }
  NXstatus status = NXputslab(this->file_id, data, &(*(start.begin())),
                              &(*(size.begin())));
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXputslab(data, " << toString(start) << ", " << toString(size)
        << ") failed";
    throw Exception(msg.str(), status);
  }
}

template <typename NumT>
void File::putSlab(vector<NumT> & data, vector<int> & start,
                   vector<int> & size) {
  if (data.empty()) {
    throw Exception("Supplied empty data");
  }
  this->putSlab(&(*(data.begin())), start, size);
}

template <typename NumT>
void File::putSlab(vector<NumT> & data, int start, int size) {
  vector<int> start_v;
  start_v.push_back(start);
  vector<int> size_v;
  size_v.push_back(size);
  this->putSlab(data, start_v, size_v);
}

NXlink File::getDataID() {
  NXlink link;
  NXstatus status = NXgetdataID(this->file_id, &link);
  if (status != NX_OK) {
    throw Exception("NXgetdataID failed", status);
  }
  return link;
}

void File::makeLink(NXlink & link) {
  NXstatus status = NXmakelink(this->file_id, &link);
  if (status != NX_OK) {
    throw Exception("NXmakelink failed", status);
  }
}

void File::makeNamedLink(const string & name, NXlink & link) {
  if (name.empty()) {
    throw Exception("Supplied empty name to makeNamedLink");
  }
  NXstatus status = NXmakenamedlink(this->file_id, name.c_str(), &link);
  if (status != NX_OK) {
    throw Exception("NXmakenamedlink(" + name + ", link)", status);
  }
}

void File::openSourceGroup() {
  NXstatus status = NXopensourcegroup(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXopensourcegroup failed");
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

/* ---------------------------------------------------------------- */
/* Concrete instantiations of template definitions.                 */
/* ---------------------------------------------------------------- */
template
void File::putAttr(const string & name, const float value);
template
void File::putAttr(const string & name, const double value);
template
void File::putAttr(const string & name, const int8_t value);
template
void File::putAttr(const string & name, const uint8_t value);
template
void File::putAttr(const string & name, const int16_t value);
template
void File::putAttr(const string & name, const uint16_t value);
template
void File::putAttr(const string & name, const int32_t value);
template
void File::putAttr(const string & name, const uint32_t value);
template
void File::putAttr(const string & name, const int64_t value);
template
void File::putAttr(const string & name, const uint64_t value);

template
void File::writeData(const string & name, const vector<float> & value);
template
void File::writeData(const string & name, const vector<double> & value);
template
void File::writeData(const string & name, const vector<int8_t> & value);
template
void File::writeData(const string & name, const vector<uint8_t> & value);
template
void File::writeData(const string & name, const vector<int16_t> & value);
template
void File::writeData(const string & name, const vector<uint16_t> & value);
template
void File::writeData(const string & name, const vector<int32_t> & value);
template
void File::writeData(const string & name, const vector<uint32_t> & value);
template
void File::writeData(const string & name, const vector<int64_t> & value);
template
void File::writeData(const string & name, const vector<uint64_t> & value);

template
void File::writeData(const string & name, const vector<float> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<double> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<int8_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<uint8_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<int16_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<uint16_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<int32_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<uint32_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<int64_t> & value, const vector<int> & dims);
template
void File::writeData(const string & name, const vector<uint64_t> & value, const vector<int> & dims);
