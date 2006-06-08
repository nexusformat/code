#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "nxconfig.h"
#include "binary/BinaryRetriever.hpp"
#include "string_util.h"

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

using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::vector;
using std::cout;
using std::endl;

typedef uint32_t data_t;


/**
 * \file NXtranslate/binary/BinaryRetriever.cpp
 */

/**
 * All nodes returned by BinaryRetriever::getData will have this name.
 */
static const string NAME("binary");

/**
 * This is the constructor for the object. No resources are allocated.
 *
 * \param str The source string is ignored, but necessary to implement
 * the interface.
 */
BinaryRetriever::BinaryRetriever(const string &source_file): filename(source_file)
{
  // there is nothing additional to allocate
}

/**
 * Since no resources were allocated, the destructor does nothing
 */
BinaryRetriever::~BinaryRetriever()
{
  // there is nothing to deallocate
}

static size_t calculate_position(const vector<int> &file_size, 
                                 const vector<int> &pos)
{
  size_t rank=file_size.size();
  size_t result=0;
  for( size_t i=0 ; i<rank ; ++i )
    {
      result=static_cast<size_t>(file_size[i]*result+pos[i]);
    }

  return result;
}

static bool increment_position(const vector<int> &offset,
                               const vector<int> &size,
                               vector<int> &pos)
{
  int index=size.size()-2;

  while(index>=0){
    if(pos[index]+1<offset[index]+size[index])
      {
        pos[index]++;
        return true;
      }
    else
      {
        pos[index]=offset[index];
        index--;
      }
  }
  return false;
}

/**
 * This is the method for retrieving data from a file. The string must
 * be of the form "type:offset,delta,number".
 *
 * \param location is the string that is used by the retriever to
 * create the data.
 * \param tr is the tree to put the result into.
 */
void BinaryRetriever::getData(const string &location, tree<Node> &tr)
{
  // check that the argument is not an empty string
  if(location.size()<=0)
    {
      throw invalid_argument("cannot parse empty string");
    }

  // break the location string into three parts: file_size,data_start,data_size
  string file_size_str;
  string start_str;
  string size_str;
  {
    vector<string> temp=string_util::split(location,"][");
    if(temp.size()!=3)
      {
        throw invalid_argument("wrong number of groups in location string");
      }
    file_size_str=temp[0].substr(1);
    start_str=temp[1];
    size_str=temp[2].substr(0,temp[2].size()-1);
  }

  // convert the parts into vector<int>
  vector<int> file_size=string_util::str_to_intVec(file_size_str);
  vector<int> start=string_util::str_to_intVec(start_str);
  vector<int> size=string_util::str_to_intVec(size_str);

  // check for the same dimension
  int rank=file_size.size();
  if(start.size()!=rank || size.size()!=rank)
    {
      throw invalid_argument("All parts of the location string must be the same rank");
    }

  // confirm that the size doesn't have a zero component
  for( size_t i ; i<rank ; ++i )
    {
      if( size[i]<=0 )
        {
          throw invalid_argument("the size arguments must all be greater than zero");
        }
    }

  // check that the request doesn't seek past the end of the file
  for( size_t i ; i<rank ; ++i )
    {
      if(file_size[i]<start[i]+size[i])
        {
          throw invalid_argument("start[i]+size[i] must be less than file_size[i]");
        }
    }

  // determine the total file size
  size_t tot_file_size=1;
  for( vector<int>::iterator it=file_size.begin() ; it!=file_size.end() ; ++it)
    {
      tot_file_size*=(*it);
    }

  // set up the starting position
  vector<int> pos;
  for( size_t i=0 ; i<start.size() ; i++ )
    {
      pos.push_back(start[i]);
    }

  // create integer copy of size
  int dims[size.size()];
  for( size_t i=0 ; i<size.size() ; ++i )
    {
      dims[i]=size[i];
    }

  // allocate the space for the result
  void *data;
  if(NXmalloc(&data,rank,dims,NX_UINT32)!=NX_OK)
    {
      throw runtime_error("NXmalloc failed");
    }

  // prepare data buffer
  size_t data_size=sizeof(data_t);
  /*
  size_t num_items=1;
  size_t buffer_size=num_items; // read a single element at a time
  data_t data_buffer[buffer_size];
  */

  // open the file
  std::ifstream data_file(filename.c_str(), std::ios::binary);
  if(!(data_file.is_open()))
    {
      throw runtime_error("Failed opening file");
    }

  // check the file size against claims
  {
    data_file.seekg(0,std::ios::beg);
    long begin=data_file.tellg();
    data_file.seekg(0,std::ios::end);
    long end=data_file.tellg();
    if( (end-begin)!=(tot_file_size*data_size) )
      {
        throw runtime_error("Actual file size does not match claimed file size");
      }
  }

  // number for the scalar version of the offset
  size_t scalar_position;

  // index into data array
  size_t data_index=0;

  // buffer to read data into
  size_t num_items=*(size.rbegin());
  size_t buffer_size=num_items*data_size;
  data_t data_buffer[num_items];

  // push through the file grabbing the proper bits
  scalar_position=data_size*calculate_position(file_size,pos);
  data_file.seekg(scalar_position,std::ios::beg);
  data_file.read(reinterpret_cast<char *>(data_buffer),buffer_size);

  // copy into final array
  memcpy((static_cast<data_t *>(data))+data_index,data_buffer,buffer_size);
  data_index+=num_items;

  while(increment_position(start,size,pos))
    {
      // calculate where to go and read in a block of data
      scalar_position=data_size*calculate_position(file_size,pos);
      data_file.seekg(scalar_position,std::ios::beg);
      data_file.read(reinterpret_cast<char *>(data_buffer),buffer_size);
      // copy into final array
      memcpy((static_cast<data_t *>(data))+data_index,data_buffer,buffer_size);
      data_index+=num_items;
    }

  // close the file
  data_file.close();

  // create the node - this copies the data
  Node node=Node(NAME,data,rank,dims,NX_UINT32);

  // insert the data into the tree
  tr.insert(tr.begin(),node);

  // delete the data
  if(NXfree(&data)!=NX_OK)
    {
      throw runtime_error("NXfree failed");
    }
}

/**
 * The MIME_TYPE is necessary so the retriever can be selected by the
 * factory.
 */
const string BinaryRetriever::MIME_TYPE("binary");

/**
 * This function returns a string representation of the retriever for
 * debugging. Since no resources are allocated the string is always
 * identical.
 *
 * \return The string returned is always "[loopy]".
 */
string BinaryRetriever::toString() const
{
  return "["+MIME_TYPE+"]:"+filename;
}
