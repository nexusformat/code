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

void File::putAttr(const AttrInfo &info, const void * data) {
  if (info.name == NULL_STR) {
    throw Exception("Supplied bad attribute name \"" + NULL_STR + "\"");
  }
  if (info.name.empty()) {
    throw Exception("Supplied empty name to putAttr");
  }
  NXstatus status = NXputattr(this->file_id, info.name.c_str(),
                              const_cast<void *>(data), info.length,
                              (int)(info.type));
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXputattr(" << info.name << ", data, " << info.length << ", "
        << info.type << ") failed";
    throw Exception(msg.str(), status);
  }
}

template <typename NumT>
void File::putAttr(const std::string & name, const NumT value) {
  NumT data[1];
  data[0] = value;
  AttrInfo info;
  info.name = name;
  info.length = 1;
  info.type = getType(value);
  this->putAttr(info, &value);
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
  AttrInfo info;
  info.name = name;
  info.length = my_value.size();
  info.type = CHAR;
  this->putAttr(info, &(my_value[0]));
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

void File::getData(void * data) {
  if (data == NULL) {
    throw Exception("Supplied null pointer to getData");
  }
  NXstatus status = NXgetdata(this->file_id, data);
  if (status != NX_OK) {
    throw Exception("NXgetdata failed", status);
  }
}

template <typename NumT>
void File::getData(vector<NumT> & data) {
  Info info = this->getInfo();
  int length = 1;
  int rank = info.dims.size();
  for (int i = 0; i < rank; i++ ){
    length *= info.dims[i];
  }
  data.reserve(length);
  this->getData(&(data[0]));
}

Info File::getInfo() {
  //vector<int> & dims, NXnumtype & type) {
  int dims[NX_MAXRANK];
  int type;
  int rank;
  NXstatus status = NXgetinfo(this->file_id, &rank, dims, &type);
  if (status != NX_OK) {
    throw Exception("NXgetinfo failed", status);
  }
  Info info;
  info.type = static_cast<NXnumtype>(type);
  for (int i = 0; i < rank; i++) {
    info.dims.push_back(dims[i]);
  }
  return info;
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

void File::getSlab(void * data, const vector<int> & start,
                   vector<int> & size) {
  if (data == NULL) {
    throw Exception("Supplied null pointer to getSlab");
  }
  if (start.size() <= 0) {
    stringstream msg;
    msg << "Supplied empty start offset, rank = " << start.size()
        << " in getSlab";
    throw Exception(msg.str());
  }
  if (start.size() != size.size()) {
    stringstream msg;
    msg << "In getSlab start rank=" << start.size() << " must match size rank="
        << size.size();
    throw Exception(msg.str());
  }

  int rank = start.size();
  int i_start[rank];
  for (int i = 0; i < rank; i++) {
    i_start[i] = start[i];
  }
  int i_size[rank];
  for (int i = 0; i < rank; i++) {
    i_size[i] = size[i];
  }

  NXstatus status = NXgetslab(this->file_id, data, i_start, i_size);
  if (status != NX_OK) {
    throw Exception("NXgetslab failed", status);
  }
}

AttrInfo File::getNextAttr() {
  //string & name, int & length, NXnumtype type) {
  char name[NX_MAXNAMELEN];
  int type;
  int length;
  NXstatus status = NXgetnextattr(this->file_id, name, &length, &type);
  if (status == NX_OK) {
    AttrInfo info;
    info.type = static_cast<NXnumtype>(type);
    info.length = length;
    info.name = string(name);
    return info;
  }
  else if (status == NX_EOD) {
    AttrInfo info;
    info.name = NULL_STR;
    info.length = 0;
    return info;
  }
  else {
    throw Exception("NXgetnextattr failed", status);
  }
}

void File::getAttr(AttrInfo & info, void * data) {
  char name[NX_MAXNAMELEN];
  strcpy(name, info.name.c_str());
  int type;
  int length;
  NXstatus status = NXgetattr(this->file_id, name, data, &length,
                              &type);
  if (status != NX_OK) {
    throw Exception("NXgetattr(" + info.name + ") failed", status);
  }
  info.name = string(name);
  info.type = static_cast<NXnumtype>(type);
  info.length = length;
}

template <typename NumT>
void File::getAttr(AttrInfo & info, NumT & value) {
  this->getAttr(info, &value);
}

string File::getStrAttr(AttrInfo & info) {
  if (info.type != CHAR) {
    stringstream msg;
    msg << "Data type must be CHAR (" << CHAR << ") found " << info.type;
    throw Exception(msg.str());
  }
  char value[NX_MAXNAMELEN];
  this->getAttr(info, value);
  return string(value);
}

vector<AttrInfo> File::getAttrInfos() {
  vector<AttrInfo> infos;
  this->initAttrDir();
  AttrInfo temp;
  while(true) {
    temp = this->getNextAttr();
    if (temp.name == NULL_STR) {
      break;
    }
    infos.push_back(temp);
  }
  return infos;
}

NXlink File::getGroupID() {
  NXlink link;
  NXstatus status = NXgetgroupID(this->file_id, &link);
  if (status != NX_OK) {
    throw Exception("NXgetgroupID failed", status);
  }
  return link;
}

int File::getGroupInfo(string & name, string & type) {
  int length;
  char c_name[NX_MAXNAMELEN];
  strcpy(c_name, name.c_str());
  char c_type[NX_MAXNAMELEN];
  strcpy(c_type, type.c_str());
  NXstatus status = NXgetgroupinfo(this->file_id, &length, c_name, c_type);
  if (status != NX_OK) {
    throw Exception("NXgetgroupinfo failed", status);
  }
  return length;
}

bool File::sameID(NXlink & first, NXlink & second) {
  NXstatus status = NXsameID(this->file_id, &first, &second);
  return (status == NX_OK);
}

void File::initGroupDir() {
  int status = NXinitgroupdir(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXinitgroupdir failed", status);
  }
}

void File::initAttrDir() {
  int status = NXinitattrdir(this->file_id);
  if (status != NX_OK) {
    throw Exception("NXinitattrdir failed", status);
  }
}

void File::setNumberFormat(NXnumtype & type, const string &format) {
  if (format.empty()) {
    throw Exception("Supplied empty format to setNumberFormat");
  }
  char c_format[NX_MAXNAMELEN];
  strcpy(c_format, format.c_str());
  NXstatus status = NXsetnumberformat(this->file_id, type, c_format);
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXsetnumberformat(" << format << ") failed";
    throw Exception(msg.str(), status);
  }
}

string File::inquireFile(const int buff_length) {
  char c_filename[buff_length];
  NXstatus status = NXinquirefile(this->file_id, c_filename, buff_length);
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXinquirefile(" << buff_length << ") failed";
    throw Exception(msg.str(), status);
  }
  return string(c_filename);
}

string File::isExternalGroup(const string & name, const string & type,
                             const uint buff_length) {
  if (name.empty()) {
    throw Exception("Supplied empty name to isExternalGroup");
  }
  if (type.empty()) {
    throw Exception("Supplied empty type to isExternalGroup");
  }
  char url[buff_length];
  NXstatus status = NXisexternalgroup(this->file_id, name.c_str(),
                                      type.c_str(), url, buff_length);
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXisexternalgroup(" <<  type << ", " << buff_length << ")";
    throw Exception(msg.str(), buff_length);
  }
  return string(url);
}

void File::linkExternal(const string & name, const string & type,
                        const string & url) {
  if (name.empty()) {
    throw Exception("Supplied empty name to linkExternal");
  }
  if (type.empty()) {
    throw Exception("Supplied empty type to linkExternal");
  }
  if (url.empty()) {
    throw Exception("Supplied empty url to linkExternal");
  }
  NXstatus status = NXlinkexternal(this->file_id, name.c_str(), type.c_str(),
                                   url.c_str());
  if (status != NX_OK) {
    stringstream msg;
    msg << "NXlinkexternal(" << name << ", " << type << ", " << url
        << ") failed";
    throw Exception(msg.str(), status);
  }
}

void malloc(void ** data, std::vector<int> & dims, NXnumtype type) {
  int rank = dims.size();
  int c_dims[rank];
  NXstatus status = NXmalloc(data, rank, c_dims, type);
  if (status != NX_OK) {
    throw Exception("NXmalloc failed", status);
  }
}


void free(void **data) {
  NXstatus status = NXfree(data);
  if (status != NX_OK) {
    throw Exception("NXfree failed", status);
  }
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
void File::getAttr(AttrInfo & info, float & value);
template
void File::getAttr(AttrInfo & info, double & value);
template
void File::getAttr(AttrInfo & info, int8_t & value);
template
void File::getAttr(AttrInfo & info, uint8_t & value);
template
void File::getAttr(AttrInfo & info, int16_t & value);
template
void File::getAttr(AttrInfo & info, uint16_t & value);
template
void File::getAttr(AttrInfo & info, int32_t & value);
template
void File::getAttr(AttrInfo & info, uint32_t & value);
template
void File::getAttr(AttrInfo & info, int64_t & value);
template
void File::getAttr(AttrInfo & info, uint64_t & value);

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
void File::getData(std::vector<float> & data);
template
void File::getData(std::vector<double> & data);
template
void File::getData(std::vector<int8_t> & data);
template
void File::getData(std::vector<uint8_t> & data);
template
void File::getData(std::vector<int16_t> & data);
template
void File::getData(std::vector<uint16_t> & data);
template
void File::getData(std::vector<int32_t> & data);
template
void File::getData(std::vector<uint32_t> & data);
template
void File::getData(std::vector<int64_t> & data);
template
void File::getData(std::vector<uint64_t> & data);
