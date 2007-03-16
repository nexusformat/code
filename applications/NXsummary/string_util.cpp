/*
 * Copyright (c) 2007, P.F.Peterson <petersonpf@ornl.gov>
 *               Spallation Neutron Source at Oak Ridge National Laboratory
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "nxsummary.hpp"
#include "string_util.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <napi.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "nxconfig.h"
#include "data_util.hpp"

// use STDINT if possible, otherwise define the types here
#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
#endif

using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

namespace nxsum {
  template <typename NumT>
  string toString(const NumT thing) {
    stringstream s;
    s << thing;
    return s.str();
  }

  // explicit instantiations so they get compiled in
  template string toString<uint32_t>(const uint32_t thing);

  template <typename NumT>
  string toString(const NumT *data, const int dims[], const int rank) {
    int num_ele = 1;
    for (size_t i = 0; i < rank; ++i ) {
      num_ele *= dims[i];
    }

    if (num_ele == 1)
      {
        return toString(data[0]);
      }
    else
      {
        throw runtime_error("Do not know how to work with arrays");
      }
  }

  string toString(const void *data, const int dims[], const int rank,
                  const int type) {
    if (type == NX_CHAR)
      {
        return (char *) data;
      }
    else if (type == NX_FLOAT32)
      {
        return toString((float *)data, dims, rank);
      }
    else if (type == NX_FLOAT64)
      {
        return toString((double *)data, dims, rank);
      }
    else
      {
        std::ostringstream s;
        s << "Do not know how to work with type=" << nxtypeAsString(type);
        throw runtime_error(s.str());
      }
  }

  string toString(const void *data, const int length, const int type) {
    int dims[1]  = {length};
    return toString(data, dims, 1, type);
  }

  string toUpperCase(const string &orig) {
    string result = orig;
    std::transform(orig.begin(), orig.end(), result.begin(), (int(*)(int))std::toupper);
    return result;
  }
}
