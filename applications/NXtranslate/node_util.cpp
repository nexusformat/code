#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include "node.h"
#include "node_util.h"
//#include "nexus_util.h"
//#include "retriever.h"
#include "string_util.h"
//#include "xml_parser.h"
//#include "Ptr.h"
//#include "tree.hh"
//#include "nxtranslate_debug.h"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
//using std::map;
using std::runtime_error;
using std::string;
using std::vector;

/**
 * The void pointer passed to this function must have enough space
 * allocated to be filled with values generated from the string.
 */
extern void void_ptr_from_string(void *&value, string &char_data, int rank,
                                       int *dims, Node::NXtype type){
  /*
   *    type    | napi4_test    | napi5_test    | nxbrowse
   * -----------|---------------|---------------|-----------
   * NX_CHAR    | char          | char          | char
   * NX_FLOAT32 | float         | float         | float
   * NX_FLOAT64 | double        | double        | double
   * NX_INT8    | unsigned char | unsigned char | char
   * NX_INT16   | short int     | short int     | short
   * NX_INT32   | int           | int           | int
   * NX_UINT8   |               |               | unsigned char
   * NX_UINT16  |               |               | unsigned short
   * NX_UINT32  |               |               | unsigned int
   */

  // REMEMBER: int a[10][20] can be accessed using a[20*row+col]
  int len=1;
  for( int i=0 ; i<rank ; i++ )
    len*=dims[i];
  if(type==NX_CHAR){
    strcpy((char*)value,char_data.c_str());
  }else if(type==NX_FLOAT32){
    string_util::str_to_floatArray(char_data,(float *)value,len);
  }else if(type==NX_FLOAT64){
    string_util::str_to_doubleArray(char_data,(double *)value,len);
  }else if(type==NX_INT8){
    string_util::str_to_ucharArray(char_data,(unsigned char *)value,len);
  }else if(type==NX_INT16){ // something got gobbled
    string_util::str_to_shortArray(char_data,(short int *)value,len);
  }else if(type==NX_INT32){
    string_util::str_to_intArray(char_data,(int *)value,len);
  }else if(type==NX_UINT8){
    string_util::str_to_ucharArray(char_data,(unsigned char *)value,len);
  }else if(type==NX_UINT16){ // something got gobbled
    string_util::str_to_ushortArray(char_data,(unsigned short *)value,len);
  }else if(type==NX_UINT32){
    string_util::str_to_uintArray(char_data,(unsigned int *)value,len);
  }else{
    throw runtime_error("unknown type in end_add_char()");
  }
}

/*
 * This routine changes a node by giving it a value (from a string),
 * dimensions and a type.
 */
extern void update_node_from_string(Node &node, string &char_data,
                                      vector<int> &v_dims, Node::NXtype type){
  // local variables to do the work
  int rank=v_dims.size();
  if(type==NX_CHAR)
    rank=1;
  int dims[rank];
  if(type==NX_CHAR){
    dims[0]=char_data.size();
  }else{
    for( int i=0 ; i<rank ; i++ )
      dims[i]=v_dims[i];
  }
  void *value;

  // allocate a space for the value
  NXmalloc(&value,rank,dims,type);

  // fill the void pointer with data generated from the string
  void_ptr_from_string(value,char_data,rank,dims,type);

  // set the value of the data
  node.set_data(value,rank,dims,type);

  // free up created value
  NXfree(&value);
}
