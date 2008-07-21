#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "napiconfig.h"
#include "NeXusFile.hpp"

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

int writeTest(const string& filename, NXaccess create_code) {
  NeXus::File file(filename, create_code);
  // create group
  file.makeGroup("entry", "NXentry", true);
  // group attributes
  file.putAttr("hugo", "namenlos");
  file.putAttr("cucumber", "passion");
  // put string
  file.writeData("ch_data", "NeXus_data");

  // 2d array
  vector<int> array_dims;
  array_dims.push_back(5);
  array_dims.push_back(4);
  char c1_array[5][4] = {{'a', 'b', 'c' ,'d'}, {'e', 'f', 'g' ,'h'}, 
                         {'i', 'j', 'k', 'l'}, {'m', 'n', 'o', 'p'},
                         {'q', 'r', 's' , 't'}};
  file.makeData("c1_data", NeXus::CHAR, array_dims, true);
  file.putData(c1_array);
  file.closeData();

  // 1d uint8 array
  vector<uint8_t> i1_array;
  for (size_t i = 0; i < 4; i++) {
    i1_array.push_back(static_cast<uint8_t>(i+1));
  }
  file.writeData("i1_data", i1_array);

  // 1d int16 array
  vector<int16_t> i2_array;
  for (size_t i = 0; i < 4; i++) {
    i2_array.push_back(1000*(i+1));
  }
  file.writeData("i2_data", i2_array);

  // 1d int32 data
  vector<int32_t> i4_array;
  for (size_t i = 0; i < 4; i++) {
    i4_array.push_back(1000000*(i+1));
  }
  file.writeData("i4_data", i4_array);

  // 2d float data
  vector<float> r4_array;
  for (size_t i = 0; i < 5*4; i++) {
    r4_array.push_back(static_cast<float>(i));
  }
  file.writeData("r4_data", r4_array, array_dims);

  // 2d double data - slab test
  vector<double> r8_array;
  for (size_t i = 0; i < 5*4; i++) {
    r8_array.push_back(static_cast<double>(i+20));
  }
  file.makeData("r8_data", NeXus::FLOAT64, array_dims, true);
  vector<int> slab_start;
  slab_start.push_back(4);
  slab_start.push_back(0);
  vector<int> slab_size;
  slab_size.push_back(1);
  slab_size.push_back(4);
  file.putSlab(&(r8_array[16]), slab_start, slab_size);
  slab_start[0] = 0;
  slab_start[1] = 0;
  slab_size[0]=4;
  slab_size[1]=4;
  file.putSlab(&(r8_array[0]), slab_start, slab_size);

  // add some attributes
  file.putAttr("ch_attribute", "NeXus");
  file.putAttr("i4_attribute", 42);
  file.putAttr("r4_attribute", 3.14159265);

  // set up for creating a link
  NXlink link = file.getDataID();
  file.closeData();

  // int64 tests
  vector<int64_t> grossezahl;
#if HAVE_LONG_LONG_INT
  grossezahl.push_back(12);
  grossezahl.push_back(555555555555LL);
  grossezahl.push_back(23);
  grossezahl.push_back(777777777777LL);
#else
  grossezahl.push_back(12);
  grossezahl.push_back(555555555555);
  grossezahl.push_back(23);
  grossezahl.push_back(777777777777);
#endif
  if (create_code != NXACC_CREATE) {
    file.writeData("grosszahl", grossezahl);
  }

  // create a new group inside this one
  file.makeGroup("data", "NXdata", true);

  // create a link
  file.makeLink(link);

  // compressed data
  array_dims[0] = 100;
  array_dims[1] = 20;
  vector<int> comp_array;
  for (int i = 0; i < array_dims[0]; i++) {
    for (int j = 0; j < array_dims[1]; j++) {
      comp_array.push_back(i);
    }
  }
  vector<int> cdims;
  cdims.push_back(20);
  cdims.push_back(20);
  file.writeCompData("comp_data", comp_array, array_dims, NeXus::LZW, cdims);

  // simple flush test
  file.flush();

  // real flush test
  file.makeData("flush_data", NeXus::getType<int>(), NX_UNLIMITED, true);
  vector<int> slab_array;
  slab_array.push_back(0);
  for (int i = 0 ; i < 7; i++) {
    slab_array[0] = i;
    file.putSlab(slab_array, i, 1);
    file.flush();
    file.openData("flush_data");
  }
  file.closeData();

  // create a sample
  file.makeGroup("sample", "NXsample", true);
  file.writeData("ch_data", "NeXus sample");

  // make more links
  NXlink glink = file.getGroupID();
  file.openPath("/");
  file.makeGroup("link", "NXentry", true);
  file.makeLink(link);
  file.makeNamedLink("renLinkGroup", glink);
  file.makeNamedLink("renLinkData", link);

  return 0;
}

template <typename NumT>
string toString(const vector<NumT> & data) {
  std::stringstream result;
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

int readTest(const string & filename) {
  const string SDS("SDS");
  // top level file information
  NeXus::File file(filename);
  std::string str;
  cout << "NXinquirefile found: " << file.inquireFile() << endl;
  vector<NeXus::AttrInfo> attr_infos = file.getAttrInfos();
  cout << "Number of global attributes: " << attr_infos.size() << endl;
  for (vector<NeXus::AttrInfo>::iterator it = attr_infos.begin();
       it != attr_infos.end(); it++) {
    if (it->name != "file_time" && it->name != "HDF_version" && it->name !=  "HDF5_Version" && it->name != "XML_version") {
        cout << "   " << it->name << " = ";
        if (it->type == NeXus::CHAR) {
          file.getAttr(*it, str);
          cout << str;
        }
        cout << endl;
    }
  }

  // check group attributes
  file.openGroup("entry", "NXentry");
  attr_infos = file.getAttrInfos();
  cout << "Number of group attributes: " << attr_infos.size() << endl;
  for (vector<NeXus::AttrInfo>::iterator it = attr_infos.begin();
       it != attr_infos.end(); it++) {
    cout << "   " << it->name << " = ";
    if (it->type == NeXus::CHAR) {
      cout << file.getStrAttr(*it);
    }
    cout << endl;
  }

  // print out the entry level fields
  map<string, string> entries = file.getEntries();
  cout << "Group contains " << entries.size() << " items" << endl;
  NeXus::Info info;
  for (map<string,string>::const_iterator it = entries.begin();
       it != entries.end(); it++) {
    cout << "   " << it->first;
    if (it->second == SDS) {
      file.openData(it->first);
      info = file.getInfo();
      cout << toString(info.dims) << " = ";
      if (info.type == NeXus::CHAR) {
        if (info.dims.size()==1) {
          cout << file.getStrData();
        }
        else {
          cout << "2d character array";
        }
      }
      else if (info.type == NeXus::FLOAT32) {
        vector<float> * result = file.getData<float>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::FLOAT64) {
        vector<double> * result = file.getData<double>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::INT8) {
        vector<int8_t> * result = file.getData<int8_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::UINT8) {
        vector<uint8_t> * result = file.getData<uint8_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::INT16) {
        vector<int16_t> * result = file.getData<int16_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::UINT16) {
        vector<uint16_t> * result = file.getData<uint16_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::INT32) {
        vector<int32_t> * result = file.getData<int32_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::UINT32) {
        vector<uint32_t> * result = file.getData<uint32_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::INT64) {
        vector<int64_t> * result = file.getData<int64_t>();
        cout << toString(*result);
      }
      else if (info.type == NeXus::UINT64) {
        vector<uint64_t> * result = file.getData<uint64_t>();
        cout << toString(*result);
      }
    }
    else {
      cout << ":" << it->second;
    }
    cout <<  endl;
  }
  file.closeGroup();

  // check links
  file.openGroup("entry", "NXentry");
  file.openGroup("sample", "NXsample");
  NXlink glink = file.getGroupID();
  file.closeGroup();
  file.openGroup("data", "NXdata");
  file.openData("r8_data");
  NXlink dlink = file.getDataID();
  file.closeData();
  file.closeGroup();
  file.openData("r8_data");
  NXlink d2link = file.getDataID();
  file.closeData();
  if (!file.sameID(dlink, d2link)) {
    cout << "Link check FAILED (r8_data)" << endl;
    cout << "     original data = ";
    file.printLink(dlink);
    cout << "     linked data = ";
    file.printLink(d2link);
    return 1;
  }
  file.closeGroup();

  file.openGroup("link", "NXentry");
  file.openGroup("sample", "NXsample");
  NXlink g2link = file.getGroupID();
  if (!file.sameID(glink, g2link)) {
    cout << "Link check FAILED (sample)" << endl;
    cout << "     original group = ";
    file.printLink(glink);
    cout << "     linked group = ";
    file.printLink(g2link);
    return 1;
  }
  file.closeGroup();

  file.openGroup("renLinkGroup", "NXsample");
  g2link = file.getGroupID();
  file.closeGroup();
  if (!file.sameID(glink, g2link)) {
    cout << "Link check FAILED (renLinkGroup)" << endl;
    cout << "     original group = ";
    file.printLink(glink);
    cout << "     linked group = ";
    file.printLink(g2link);
    return 1;
  }

  file.openData("renLinkData");
  d2link = file.getDataID();
  file.closeData();
  if (!file.sameID(dlink, d2link)) {
    cout << "Link check FAILED (renLinkData)" << endl;
    cout << "     original data = ";
    file.printLink(dlink);
    cout << "     linked data = ";
    file.printLink(d2link);
    return 1;
  }

  file.closeGroup();
  cout << "Link check OK" << endl;

  // openpath checks
  file.openPath("/entry/data/comp_data");
  file.openPath("/entry/data/comp_data");
  file.openPath("../r8_data");
  printf("NXopenpath checks OK\n");

  // everything went fine
  return 0;
}

int testLoadPath(const string & filename) {
  if(getenv("NX_LOAD_PATH") != NULL){
    NeXus::File file(filename);
    cout << "Success loading NeXus file from path" << endl;
    // cout << file.inquireFile() << endl; // DEBUG print
  }
  return 0;
}

int testExternal(const string & filename, NXaccess create_code){
  NeXus::File file(filename, create_code);
  file.makeGroup("entry1", "NXentry");

/*
  char nxfile[255], ext[5], testFile[80], time[132], filename[256];
  int create;
  NXhandle hfil;

  if(strstr(progName,"hdf4") != NULL){
    strcpy(ext,"hdf");
    create = NXACC_CREATE;
  } else if(strstr(progName,"hdf5") != NULL){
    strcpy(ext,"h5");
    create = NXACC_CREATE5;
  } else if(strstr(progName,"xml") != NULL){
    strcpy(ext,"xml");
    create = NXACC_CREATEXML;
  } else {
    printf("Failed to recognise napi_test program in testExternal\n");
    return 1;
  }

  sprintf(testFile,"nxext.%s", ext);

  // create the test file
  if(NXopen(testFile,create,&hfil) != NX_OK){
    return 1;
  }
  if(NXmakegroup(hfil,"entry1","NXentry") != NX_OK){
    return 1;
  }
  sprintf(nxfile,"nxfile://data/dmc01.%s#/entry1",ext);
  if(NXlinkexternal(hfil,"entry1","NXentry",nxfile) != NX_OK){
    return 1;
  }
  if(NXmakegroup(hfil,"entry2","NXentry") != NX_OK){
    return 1;
  }
  sprintf(nxfile,"nxfile://data/dmc02.%s#/entry1",ext);
  if(NXlinkexternal(hfil,"entry2","NXentry",nxfile) != NX_OK){
    return 1;
  }
  if(NXclose(&hfil) != NX_OK){
    return 1;
  }

  //
  if(NXopen(testFile,NXACC_RDWR,&hfil) != NX_OK){
    return 1;
  }

  if(NXopenpath(hfil,"/entry1/start_time") != NX_OK){
    return 1;
  }
  memset(time,0,132);
  if(NXgetdata(hfil,time) != NX_OK){
    return 1;
  }
  printf("First file time: %s\n", time);

  if(NXinquirefile(hfil,filename,256) != NX_OK){
    return 1;
  }
  printf("NXinquirefile found: %s\n", filename);

  if(NXopenpath(hfil,"/entry2/sample/sample_name") != NX_OK){
    return 1;
  }
  memset(time,0,132);
  if(NXgetdata(hfil,time) != NX_OK){
    return 1;
  }
  printf("Second file sample: %s\n", time);
  if(NXinquirefile(hfil,filename,256) != NX_OK){
    return 1;
  }
  printf("NXinquirefile found: %s\n", filename);

  if(NXopenpath(hfil,"/entry2/start_time") != NX_OK){
    return 1;
  }
  memset(time,0,132);
  if(NXgetdata(hfil,time) != NX_OK){
    return 1;
  }
  printf("Second file time: %s\n", time);
  NXopenpath(hfil,"/");
  if(NXisexternalgroup(hfil,"entry1","NXentry",filename,255) != NX_OK){
    return 1;
  } else {
    printf("entry1 external URL = %s\n", filename);
  }

  NXclose(&hfil);
  printf("External File Linking tested OK\n");
*/
  return 0;
}

int main(int argc, char** argv)
{
  NXaccess nx_creation_code;
  string filename;
  if(strstr(argv[0],"napi_test_cpp-hdf5") != NULL){
    nx_creation_code = NXACC_CREATE5;
    filename = "napi_test_cpp.h5";
  }else if(strstr(argv[0],"napi_test_cpp-xml-table") != NULL){
    nx_creation_code = NXACC_CREATEXML | NXACC_TABLE;
    filename = "napi_test_cpp-table.xml";
  }else if(strstr(argv[0],"napi_test_cpp-xml") != NULL){
    nx_creation_code = NXACC_CREATEXML;
    filename = "napi_test_cpp.xml";
  } else {
    nx_creation_code = NXACC_CREATE;
    filename = "napi_test_cpp.hdf";
  }

  int result;
  result = writeTest(filename, nx_creation_code);
  if (result) {
    cout << "writeTest failed" << endl;
    return result;
  }
  if ( (argc >= 2) && !strcmp(argv[1], "-q") )
  {
    return 0;	//
  }

  // try reading a file
  result = readTest(filename);
  if (result) {
    cout << "readTest failed" << endl;
    return result;
  }

  // try using the load path
  result = testLoadPath("dmc01.hdf");
  if (result) {
    cout << "testLoadPath failed" << endl;
    return result;
  }

  // try external linking
  result = testExternal("napi_cpp_ext.nxs", nx_creation_code);
  if (result) {
    cout << "testExternal failed" << endl;
    return result;
  }

  // everything went ok
  return 0;
}
