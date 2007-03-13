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
#include <iostream>
#include <sstream>
#include <napi.h>
#include <stdexcept>
#include <string>
#include <vector>

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

  template <typename NumT>
  string toString(const NumT *data, const int dims[], const int rank,
                  const Config &config) {
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
                  const int type, const Config &config) {
    if (type == NX_CHAR)
      {
        return (char *) data;
      }
    else if (type == NX_FLOAT32)
      {
        return toString((float *)data, dims, rank, config);
      }
    else
      {
        return UNKNOWN_TYPE;
      }
  }

  string toString(const void *data, const int length, const int type,
                  const Config &config) {
    int dims[1]  = {length};
    return toString(data, dims, 1, type, config);
  }

/*
static string readAsString(NXhandle handle, const string &path, 
                           const Config &config) {
  // convert the path to something c-friendly
  char c_path[GROUP_STRING_LEN];
  strcpy(c_path, path.c_str());

  // open the path
  if(NXopenpath(handle, c_path)!=NX_OK)
    {
      throw runtime_error("COULD NOT OPEN PATH");
      return "";
    }

  // determine rank and dimension
  int rank = 0;
  int type = 0;
  int dims[NX_MAXRANK];
  if (NXgetinfo(handle, &rank, dims, &type)!=NX_OK)
    {
      throw runtime_error("COULD NOT GET NODE INFORMATION");
    }

  // confirm dimension isn't too high
  if (rank > NX_MAXRANK)
    {
      throw runtime_error("DIMENSIONALITY IS TOO HIGH");
    }

  // allocate space for data
  void *data;
  if(NXmalloc(&data,rank,dims,type)!=NX_OK)
    {
      throw runtime_error("NXmalloc falied");
    }

  // retrieve data from the file
  if(NXgetdata(handle,data)!=NX_OK)
    {
      throw runtime_error("NXgetdata failed");
    }

  // convert result to string
  string result = toString(data, dims, rank, type, config);

  //free up the pointer
  if(NXfree(&data)!=NX_OK)
    {
      throw runtime_error("NXfree failed");
    }

  return result;
}
*/

/*
static string readAttrAsString(NXhandle handle, const string label, const Config &config) {
  if (NXinitattrdir(handle)!=NX_OK)
    {
      throw runtime_error("NXinitattrdir failed");
    }
  int num_attr;
  if (NXgetattrinfo(handle, &num_attr)!=NX_OK)
    {
      throw runtime_error("NXgetattrinfo failed");
    }
  char name[GROUP_STRING_LEN];
  int length;
  int type;
  for (int i = 0 ; i < num_attr ; ++i) {
    if (NXgetnextattr(handle, name, &length, &type)!=NX_OK)
      {
        throw runtime_error("NXgetnextattr failed");
      }
    if (label == name)
      {
        void *data;
        int dims[1]  = {length};
        if (NXmalloc(&data, 1, dims, type)!=NX_OK)
          {
            throw runtime_error("NXmalloc failed");
          }
        if (NXgetattr(handle, name, data, dims, &type)!=NX_OK)
          {
            throw runtime_error("NXgetattr failed");
          }
        string result = toString(data, dims, 1, type, config);
        if (NXfree(&data)!=NX_OK)
          {
            throw runtime_error("NXfree failed");
          }
        return result;
      }
  }

  return "";
}
*/
}
