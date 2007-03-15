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
#include <vector>
#include "nxsummary.hpp"
#include "preferences.hpp"
#include "string_util.hpp"
#include "tclap/CmdLine.h"

using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;
using namespace TCLAP;
using namespace nxsum;

static const string NXSUM_VERSION = "0.1.0";

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
        if (NXmalloc(&data, 1, &length, type)!=NX_OK)
          {
            throw runtime_error("NXmalloc failed");
          }
        int dims[1]  = {length};
        if (NXgetattr(handle, name, data, dims, &type)!=NX_OK)
          {
            throw runtime_error("NXgetattr failed");
          }
        string result = toString(data, length, type, config);
        if (NXfree(&data)!=NX_OK)
          {
            throw runtime_error("NXfree failed");
          }
        return result;
      }
  }

  return "";
}

static void printInfo(NXhandle handle, const Item &item, const Config &config) {
  try {
    string value = readAsString(handle, item.path, config);
    string units = readAttrAsString(handle, "units", config);
    if (config.show_label)
      {
        cout << item.label << ':';
      }
    cout << value << ' ' << units << endl;
  } catch(runtime_error &e) {
    // let it drop on the floor
  }
}

static void printSummary(const string &file, const Config &config) {
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

  int length = config.preferences.size();
  for (int i = 0 ; i < length ; ++i ) {
    printInfo(handle, config.preferences[i], config);
  }

  if(NXclose(&handle)!=NX_OK)
    {
      throw runtime_error("Could not close file");
    }
}

static void printValue(const string &file, const Item &item,
                       const Config &config) {
  NXhandle handle;
  char filename[GROUP_STRING_LEN];
  strcpy(filename, file.c_str());
  if(NXopen(filename,NXACC_READ,&handle)!=NX_OK)
    {
      throw runtime_error("Could not open file");
    }

  if (config.multifile)
    {
      cout << file << ";";
    }
  printInfo(handle, item, config);

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

      // configure the arguments
      SwitchArg verboseArg("", "verbose", "Turn on verbose printing", 
                           false, cmd);
      UnlabeledMultiArg<string> filenameArg("filename",
                                            "Name of a file to be viewed",
                                            "filename",cmd);
      ValueArg<string> configArg("", "config", "Specify configuration file",
                                 false, "", "config", cmd);
      ValueArg<string> writeConfigArg("", "writeconfig",
                              "Write the default configuration out to a file",
                                      false, "", "config", cmd);
      ValueArg<string> valueArg("", "value",
                               "Get value of the item pointed to by the label",
                                false, "", "label", cmd);

      // parse the arguments
      cmd.parse(argc, argv);

      // fill in the config object
      struct Config config;
      config.verbose = verboseArg.getValue();
      config.show_label = true;

      // load in the preferences
      loadPreferences(configArg.getValue(), config.preferences);

      // write out the preferences and exit
      string configOut = writeConfigArg.getValue();
      if (configOut != "")
        {
          writePreferences(configOut, config.preferences);
          return 0;
        }

      // turn of NeXus debug printing
      NXMDisableErrorReporting();

      // get the list of filenames
      vector<string> files=filenameArg.getValue();
      if(files.empty())
        {
          std::cerr << "ERROR: failed to supply <filename>" << endl;
          cmd.getOutput()->usage(cmd);
          return -1;
        }
      config.multifile = (files.size()>1);

      // are we looking for a particular value
      Item item;
      bool getValue = (valueArg.getValue().size()>0);
      if (getValue)
        {
          item = getPreference(valueArg.getValue(), config.preferences);
          config.show_label = false;
        }

      // go through the list of files
      for (vector<string>::const_iterator file = files.begin() ;
           file != files.end() ; file++ )
        {
          try
            {
              if (getValue)
                {
                  printValue(*file, item, config);
                }
              else
                {
                  printSummary(*file, config);
                }
            }
          catch(runtime_error &e)
            {
              std::cerr << "RUNTIME ERROR:" << e.what() <<endl;
            }
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
