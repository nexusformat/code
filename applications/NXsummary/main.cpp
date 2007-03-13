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

#include <iostream>
#include <napi.h>
#include <stdexcept>
#include <string>
#include <tclap/CmdLine.h>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;
using namespace TCLAP;

// useful constants
static const size_t BLOCK_SIZE = 2048;
static const size_t GROUP_STRING_LEN = 512;
static const string NXSUM_VERSION = "0.1.0";
static const string NOT_FOUND = "NOT FOUND";
static const string UNKNOWN_TYPE = "UNKNOWN TYPE";

// path to information in the summary
static vector<string> paths;
static vector<string> labels;

// define the configuration object
struct Config{
  bool verbose;
  bool multifile;
};

static void addPathLabel(const string &path, const string &label) {
  paths.push_back(path);
  labels.push_back(label);
}

static void initPathLabels() {
  addPathLabel("/entry/title", "TITLE");
  addPathLabel("/entry/notes", "NOTES");
  addPathLabel("/entry/start_time", "START TIME");
  addPathLabel("/entry/end_time", "END TIME");
  addPathLabel("/entry/duration", "DURATION");
  addPathLabel("/entry/proton_charge", "PROTON CHARGE");
  addPathLabel("/entry/sample/name", "SAMPLE NAME");
  addPathLabel("/entry/sample/nature", "SAMPLE NATURE");
  addPathLabel("/entry/sample/type", "SAMPLE TYPE");
  addPathLabel("/entry/sample/identifier", "SAMPLE IDENTIFIER");
  /*
static const char*  TOTAL_COUNTS_PATH = "";
static const string TOTAL_COUNTS_LABEL = "TOTAL COUNTS";
static const char*  TOTAL_MON_COUNTS_PATH = "";
static const string TOTAL_MON_COUNTS_LABEL = "MONITOR COUNTS";
static const char*  PI_PATH = "";
static const string PI_LABEL = "PRICINPLE INVESTIGATOR";
  */
}

template <typename NumT>
static string toString(const NumT thing) {
  std::stringstream s;
  s << thing;
  return s.str();
}

template <typename NumT>
static string toString(const NumT *data, const int dims[], const int rank, const Config &config) {
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

static string toString(const void *data, const int dims[], const int rank,
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

static void printInfo(NXhandle handle, const string &path, const string &label, const Config &config) {
  try {
    string value = readAsString(handle, path, config);
    string units = readAttrAsString(handle, "units", config);
    cout << label << ':' << value << ' ' << units << endl;
  } catch(runtime_error &e) {
    // let it drop on the floor
  }
}

static void printSummary(const string &file, Config &config) {
  if (config.multifile)
    {
      cout << "********** " << file << endl;
    }
  NXhandle handle;
  char filename[GROUP_STRING_LEN];
  strcpy(filename, file.c_str());
  if(NXopen(filename,NXACC_READ,&handle)!=NX_OK)
    {
      throw runtime_error("Could not open file");
    }

  int length = paths.size();
  for (int i = 0 ; i < length ; ++i ) {
    printInfo(handle, paths[i], labels[i], config);
  }

  if(NXclose(&handle)!=NX_OK)
    {
      throw runtime_error("Could not close file");
    }
}

int main(int argc, char *argv[]) {
  try
    {
      // set up the parser
      CmdLine cmd("Generate summary of a NeXus file", ' ', NXSUM_VERSION);

      // verbose argument
      SwitchArg verboseArg("", "verbose", "Turn on verbose printing", 
                           false, cmd);

      // the filename arguments
      UnlabeledMultiArg<string> filenameArg("filename",
                                            "Name of a file to be viewed",
                                            "filename",cmd);

      // parse the arguments
      cmd.parse(argc, argv);

      // get the list of filenames
      vector<string> files=filenameArg.getValue();
      if(files.empty())
        {
          std::cerr << "ERROR: failed to supply <filename>" << endl;
          cmd.getOutput()->usage(cmd);
          return -1;
        }

      // fill in the config object
      struct Config config;
      config.verbose = verboseArg.getValue();
      config.multifile = (files.size()>1);

      initPathLabels();
      NXMDisableErrorReporting();

      for (vector<string>::const_iterator file = files.begin() ;
           file != files.end() ; file++ )
        {
          printSummary(*file, config);
        }
    }
  catch(ArgException &e)
    {
      std::cerr << "PARSE ERROR:" << e.error() << " for arg " << e.argId()
                << endl;
      return -1;
    }
  catch(runtime_error &e)
    {
      std::cerr << "RUNTIME ERROR:" << e.what() <<endl;
      return -1;
    }

  // all is well
  return 0;
}
