#ifndef __STRING_UTIL_HPP_GUARD__
#define __STRING_UTIL_HPP_GUARD__

#include <string>

namespace nxsum {
  std::string toString(const void *data, const int dims[], const int rank,
                       const int type, const Config &config);
  std::string toString(const void *data, const int length, const int type,
                       const Config &config);
}
#endif
