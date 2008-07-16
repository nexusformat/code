#include <iostream>
#include <string>
#include <vector>
#include "napiconfig.h"
#include "NeXusFile.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void writeTest(const string & filename) {
  NXaccess create_code = NXACC_CREATE;
#ifdef HDF5
  create_code = NXACC_CREATE5;
#elif HDF4
  create_code = NXACC_CREATE4;
#elif NXXML
  create_code = NXACC_CREATEXML;
#endif
  create_code = NXACC_CREATEXML; // TODO remove this line
  NeXus::File file(filename, create_code);
  // create group
  file.makeGroup("entry", "NXentry");
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
  file.makeData("c1_data", NeXus::CHAR, array_dims);
  file.putData(c1_array);
  file.closeData();

  // 1d uint8 array
  vector<uint8_t> i1_array;
  for (size_t i = 0; i < 4; i++) {
    i1_array.push_back(i+1);
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
    r8_array.push_back(static_cast<double>(i));
  }
  file.makeData("r8_data", NeXus::FLOAT64, array_dims);
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

  // int64 tests - TODO this does not put anything into the file
#ifdef HAVE_LONG_LONG_INT
  vector<int64_t> grossezahl;
  grossezahl.push_back(12);
  grossezahl.push_back(555555555555LL);
  grossezahl.push_back(23);
  grossezahl.push_back(777777777777LL);
  file.writeData("grosszahl", grossezahl);
#endif

  // create a new group inside this one
  file.makeGroup("data", "NXdata");

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
  file.makeData("flush_data", NeXus::getType(static_cast<int>(0)),
                NX_UNLIMITED);
  vector<int> slab_array;
  slab_array.push_back(0);
  for (int i = 0 ; i < 7; i++) {
    slab_array[0] = i;
    file.putSlab(slab_array, i, 1);
    file.flush();
  }
  file.closeData();

  // create a sample
  file.makeGroup("sample", "NXsample");
  file.writeData("ch_data", "NeXus sample");

  // make more links
  NXlink glink = file.getGroupID();
  file.openPath("/");
  file.makeGroup("link", "NXentry");
  file.makeLink(link);
  file.makeNamedLink("renLinkGroup", glink);
  file.makeNamedLink("renLinkData", link);
}

void readTest(const string & filename) {
  // top level file information
  NeXus::File file(filename);
  cout << "NXinquirefile found: " << file.inquireFile() << endl;
  vector<NeXus::AttrInfo> attr_infos = file.getAttrInfos();
  for (vector<NeXus::AttrInfo>::iterator it = attr_infos.begin();
       it != attr_infos.end(); it++) {
    cout << "   " << it->name << " = ";
    if (it->type == NeXus::CHAR) {
      cout << file.getStrAttr(*it);
    }
    cout << endl;
  }
}

int main(int argc, char** argv)
{
  string filename("napi_cpp.nxs");
  writeTest(filename);
  if ( (argc >= 2) && !strcmp(argv[1], "-q") )
  {
     return 0;	/* create only */
  }
  readTest(filename);
  return 0;
}
