//#include <algorithm>
//#include <iostream>
//#include <fstream>
//#include <sstream>
#include <stdexcept>
#include <stdint.h>
//#include <string>
//#include <vector>
//#include <iterator>
//#include <stdlib.h>
#include "retriever.h"
//#include "../node_util.h"
#include "../string_util.h"

using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::cout;
using std::endl;
//using std::vector;

static const string name("loopy");

/*********************************
/SnsHistogramRetriever constructor
`/*********************************/
LoopyRetriever::LoopyRetriever(const string &str)
{
  // there is nothing to allocate
}

/*********************************
/SnsHistogramRetriever destructor
/*********************************/
LoopyRetriever::~LoopyRetriever()
{
  // there is nothing to deallocate
}

static string::size_type count_occur(const string &str, const string &ch){
  string::size_type count=0;
  string::size_type index=0;

  while(true){
    index=str.find(ch,index+1);
    if(index==string::npos)
      break;
    count++;
  }
  return count;
}

static int str_to_type(const string &type){
  if(type.find("INT8")==0){
    return NX_INT8;
  }else if(type.find("INT16")==0){
    return NX_INT16;
  }else if(type.find("INT32")==0){
    return NX_INT32;
  }else if(type.find("UINT8")==0){
    return NX_UINT8;
  }else if(type.find("UINT16")==0){
    return NX_UINT16;
  }else if(type.find("UINT32")==0){
    return NX_UINT32;
  }else if(type.find("UINT32")==0){
    return NX_UINT32;
  }else if(type.find("FLOAT32")==0){
    return NX_FLOAT32;
  }else if(type.find("FLOAT64")==0){
    return NX_FLOAT64;
  }
  throw invalid_argument("Do not understand type:"+type);
}

template <typename NumT>
void
fill_array(void *&data, const NumT offset, const NumT &delta, const int num){
  for( int i=0 ; i<num ; i++ ){
    (static_cast<NumT *>(data))[i]=(delta*static_cast<NumT>(i)+offset);
  }
}

/********************************
/* This is the method for retrieving data from a file. 
/* Interpreting the string is left up to the implementing code.
/********************************/
void LoopyRetriever::getData(const string &location, tree<Node> &tr)
{
  // check that the argument is not an empty string
  if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");

  // check that it has the correct number of colons
  if(count_occur(location,":")!=1)
    throw invalid_argument("location must contain only one colon");

  // check that it has 
  if(count_occur(location,",")!=2)
    throw invalid_argument("location must contain only two commas");

  string::size_type index;

  // split the string into type and loop information
  index=location.find(":");
  string type=location.substr(0,index);
  string loopinfo=location.substr(index+1);

  // split the loop parameters into separate strings
  index=loopinfo.find(",");
  string offset_str=loopinfo.substr(0,index);
  loopinfo=loopinfo.substr(index+1);
  index=loopinfo.find(",");
  string delta_str=loopinfo.substr(0,index);
  string num_str=loopinfo.substr(index+1);

  // create the dimensions array - this can throw an exception
  int dims[]={static_cast<int>(string_util::str_to_int(num_str))};

  // convert the type to an integer
  int int_type=str_to_type(type);

  // get memory for the data
  void *data;
  if(NXmalloc(&data,1,dims,int_type)!=NX_OK)
    throw runtime_error("NXmalloc failed");

  // fill the data array
  if(int_type==NX_INT8){
    fill_array(data,
               static_cast<int8_t>(string_util::str_to_int(offset_str)),
               static_cast<int8_t>(string_util::str_to_int(delta_str)),
               dims[0]);
  }else if(int_type==NX_INT16){
    fill_array(data,
               static_cast<int16_t>(string_util::str_to_int(offset_str)),
               static_cast<int16_t>(string_util::str_to_int(delta_str)),
               dims[0]);
  }else if(int_type==NX_INT32){
    fill_array(data,
               static_cast<int32_t>(string_util::str_to_int(offset_str)),
               static_cast<int32_t>(string_util::str_to_int(delta_str)),
               dims[0]);
  }else if(int_type==NX_UINT8){
    fill_array(data,
               static_cast<uint8_t>(string_util::str_to_uint(offset_str)),
               static_cast<uint8_t>(string_util::str_to_uint(delta_str)),
               dims[0]);
  }else if(int_type==NX_UINT16){
    fill_array(data,
               static_cast<uint16_t>(string_util::str_to_uint(offset_str)),
               static_cast<uint16_t>(string_util::str_to_uint(delta_str)),
               dims[0]);
  }else if(int_type==NX_UINT32){
    fill_array(data,
               static_cast<uint32_t>(string_util::str_to_uint(offset_str)),
               static_cast<uint32_t>(string_util::str_to_uint(delta_str)),
               dims[0]);
  }else if(int_type==NX_FLOAT32){
    fill_array(data,
               static_cast<float>(string_util::str_to_float(offset_str)),
               static_cast<float>(string_util::str_to_float(delta_str)),
               dims[0]);
  }else if(int_type==NX_FLOAT64){
    fill_array(data,
               static_cast<double>(string_util::str_to_float(offset_str)),
               static_cast<double>(string_util::str_to_float(delta_str)),
               dims[0]);
  }

  // create the node
  Node node=Node(name,data,1,dims,int_type);
  tr.insert(tr.begin(),node);

  // delete the data
  if(NXfree(&data)!=NX_OK)
    throw runtime_error("NXfree failed");

}

// **********************************************************************
/*
        if(((UserData *)user_data)->expand){
          using string_util::intListStr_to_intVec;
          int rank=1;
          int dims[rank];
          void *value;
          Node::NXtype type=node.int_type();
          if(type==NX_INT8){
            vector<unsigned char> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((unsigned char *)value)[i]=vec[i];
          }else if(type==NX_INT16){
            vector<short int> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((short int *)value)[i]=vec[i];
          }else if(type==NX_INT32){
            vector<int> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((int *)value)[i]=vec[i];
          }else if(type==NX_UINT8){
            vector<unsigned char> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((unsigned char *)value)[i]=vec[i];
          }else if(type==NX_UINT16){
            vector<unsigned short> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((unsigned short *)value)[i]=vec[i];
          }else if(type==NX_UINT32){
            vector<unsigned int> vec;
            intListStr_to_intVec(((UserData *)user_data)->char_data,vec);
            dims[0]=vec.size();
            NXmalloc(&value,rank,dims,type);
            for( int i=0 ; i<dims[0] ; i++ )
              ((unsigned int *)value)[i]=vec[i];
          }else{
            print_error(((UserData *)user_data),
                    INVALID_ARGUMENT+":"+except_label+" type must be integer");
          }
          node.set_data(value,rank,dims,type);
          NXfree(&value);
        }else{
        }
*/

// **********************************************************************

/*********************************
/SnsHistogramRetriever::MIME_TYPE 
/*********************************/
const string LoopyRetriever::MIME_TYPE("loopy");

string LoopyRetriever::toString() const
{
  return "["+MIME_TYPE+"] ";
}
