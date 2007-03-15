#ifndef __STRING_UTIL_HPP_GUARD__
#define __STRING_UTIL_HPP_GUARD__

#include <string>

namespace nxsum {
  template <typename NumT> std::string toString(const NumT thing);
  std::string toString(const void *data, const int dims[], const int rank,
                       const int type);
  std::string toString(const void *data, const int length, const int type);

  std::string toUpperCase(const std::string &orig);
}
#endif
