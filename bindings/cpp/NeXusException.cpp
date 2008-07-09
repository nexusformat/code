#include "NeXusException.hpp"

using namespace NeXus;

Exception::Exception(const std::string& msg) :
  std::runtime_error(msg) {
 this->__what = msg;
}

const char* Exception::what() const throw() {
  return __what.c_str();
}

Exception::~Exception() throw() {
}
