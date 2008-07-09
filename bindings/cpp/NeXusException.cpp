#include "NeXusException.hpp"

using namespace NeXus;

Exception::Exception(const std::string& msg, const int status) :
  std::runtime_error(msg) {
 this->__what = msg;
 this->__status = status;
}

const char* Exception::what() const throw() {
  return this->__what.c_str();
}

int Exception::status() throw() {
  return this->__status;
}

Exception::~Exception() throw() {
}
