#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "string_util.h"

using std::cout;
using std::endl;
using std::find;
using std::find_if;
using std::invalid_argument;
using std::runtime_error;
using std::isspace;
using std::string;
using std::vector;

typedef vector<string> StrVec;
typedef vector<string>::const_iterator StrVecIter;

static bool my_isnotdigit(char c){
  return !isdigit(c);
}

static bool has_non_zero(const string &str){
  string::size_type size=str.size();

  if(str.find("1")<size || str.find("2")<size || str.find("3")<size || str.find("4")<size
     || str.find("5")<size || str.find("6")<size || str.find("7")<size || str.find("8")<size
     || str.find("9")<size )
    return true;

  return false;
}

extern bool string_util::starts_with(const string &str1, const string &str2){
  // empty string can't start with anything
  if(str1.empty())
    return false;

  // can't start with something that is longer
  if(str2.size()>str1.size())
    return false;

  // if they are the same then just return true
  if(str1==str2)
    return true;

  // do the actual comparison
  string cmp_str=str1.substr(0,str2.size());
  return (cmp_str==str2);
}

/*
 * strip leading and trailing spaces from the string.
 */
extern string string_util::trim (const string &str) {
  typedef string::size_type string_size;
  string new_str="";
  string_size i=0;
  while(i<str.size()){
    //skip initial whitespace
    while(i<str.size() && isspace(str[i])) {
      i++;
    }

    // find the extent of ending whitespace
    string_size  j=str.size();
    while(j>i && isspace(str[j-1])) {
      j--;
      }

    //copy the non-whitespace into the new string
    
    if (i!=j){
      new_str+=str.substr(i,j-i);
      i=j;
    }
  }
  return new_str;
}

extern long string_util::str_to_int(const string &str){
  if(str.substr(0,1)=="-")
    return -1*str_to_int(str.substr(1,str.size()));

  string::const_iterator it=str.begin();
  it=find_if(it,str.end(),my_isnotdigit);

  if(it!=str.end())
    throw invalid_argument("str_to_int(string) argument is not an integer");

  return atol(str.c_str());
}

extern ulong string_util::str_to_uint(const string &str){
  long num=str_to_int(str);
  if(num<0)
    throw invalid_argument("str_to_uint(string) argument is not an integer");
  return num;
}

extern double string_util::str_to_float(const string &str){
  double num=atof(str.c_str());

  // check if the return is bad
  if((num==0.0) || (!num)){
    string::const_iterator it=str.begin();
    it=find_if(it,str.end(),my_isnotdigit);
    if(it!=str.end() || has_non_zero(str)){
      throw invalid_argument("str_to_float(string) argument is not a float");
    }
  }

  return num;
}

static bool is_bracket(char c){
  static const string BRACKETS="[]";
  return find(BRACKETS.begin(),BRACKETS.end(),c)!=BRACKETS.end();
}

static bool is_comma(char c){
  static const string COMMA=",";
  return find(COMMA.begin(),COMMA.end(),c)!=COMMA.end();
}

/*
 * split a string up using commas as the delimiter
 */
static StrVec split(const string &str){
  StrVec result;
  typedef string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at end of last word
    while(i<str.size() && is_comma(str[i]))
      i++;

    // find end of "word"
    string_size j=i;
    while(j<str.size() && !is_comma(str[j])){
      j++;
    }

    if(i!=j){
      result.push_back(str.substr(i,j-i));
      i=j;
    }
  }

  return result;
}

static bool is_slash(char c){
  static const string SLASH="/";
  return find(SLASH.begin(),SLASH.end(),c)!=SLASH.end();
}

static bool is_colon(char c){
  static const string COLON=":";
  return find(COLON.begin(),COLON.end(),c)!=COLON.end();
}

extern vector<string> string_util::string_to_path(const string &str){
  vector<string> result;
  typedef string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at the end of the last word
    while(i<str.size() && (is_slash(str[i]) || is_colon(str[i])) )
      i++;

    // find end of a "word"
    string_size j=i;
    while(j<str.size() && (!is_slash(str[j])) && (!is_colon(str[j])) )
      j++;

    if(i!=j){
      result.push_back(str.substr(i,j-i));
      i=j;
    }
  }

  return result;
}

static StrVec shrink_and_split(string &str){
  static const char *COMMA=",";

  // replace brackets with commas
  for( string::iterator ch=str.begin() ; ch!=str.end() ; ch++ ){
    if(is_bracket(*ch))
      *ch=*COMMA;
  }

  // replace the first space with a comma
  bool space=false;
  for( string::iterator ch=str.begin() ; ch!=str.end() ; ch++ ){
    if(isspace(*ch)){
      if(!space){
        space=true;
        *ch=*COMMA;
      }
    }else{
      space=false;
    }
  }

  // remove spaces
  {
    typedef string::size_type string_size;

    string new_str="";
    string_size i=0;
    while(i<str.size()){
      // skip initial whitespace
      while(i<str.size() && isspace(str[i]))
        i++;

      // find the end of the non-whitespace section
      string_size j=i;
      while(j<str.size() && !isspace(str[j]))
        j++;

      // copy the non-whitespace into the new string
      if(i!=j){
        new_str+=str.substr(i,j-i);
        i=j;
      }
    }
    str=new_str;
  }

  // trim extra commas off of the beginning
  while(str.substr(0,1)==COMMA)
    str.erase(0,1);

  // trim extra commas off of the end
  while(str.substr(str.size()-1,str.size())==COMMA)
    str.erase(str.size()-1,str.size());

  return split(str);
}

extern void string_util::str_to_shortArray(std::string & str,short *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(short)str_to_int(*(strIt+i));
}

extern void string_util::str_to_intArray(std::string & str,int *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(int)str_to_int(*(strIt+i));
}

extern vector<int> string_util::str_to_intVec(string &str){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  // turn each string into an integer
  vector<int> result;
  for( StrVec::const_iterator str=splitted.begin() ; str!=splitted.end() ; str++ )
    result.push_back(str_to_int(*str));

  // return the vector<int>
  return result;
}

extern void string_util::str_to_longArray(std::string & str,long *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(long)str_to_int(*(strIt+i));
}

extern void string_util::str_to_ushortArray(std::string & str,ushort *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(ushort)str_to_uint(*(strIt+i));
}

extern void string_util::str_to_uintArray(std::string & str,uint *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(uint)str_to_uint(*(strIt+i));
}

extern void string_util::str_to_ulongArray(std::string & str,ulong *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(ulong)str_to_uint(*(strIt+i));
}

extern void string_util::str_to_floatArray(std::string & str,float *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(float)str_to_float(*(strIt+i));
}

extern void string_util::str_to_doubleArray(std::string & str,double *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(double)str_to_float(*(strIt+i));
}

extern void string_util::str_to_ucharArray(std::string &str,unsigned char *array, const uint len){
  // break it up into a string vector
  StrVec splitted=shrink_and_split(str);

  if(splitted.size()!=len)
    throw runtime_error("array and string not same size");

  // turn each string into a short
  StrVecIter strIt=splitted.begin();
  for( uint i=0 ; i<len ; i++ )
    *(array+i)=(unsigned char)str_to_int(*(strIt+i));
}

/*
 * This method takes a string as an argument and attempts to convert this 
 * string into a vector of integers.  The string is in the form of a comma 
 * separated list.  Each element in the list is a single integer or a range
 * of integers specified by a starting value and ending value separated by
 * a colon.  i.e. 1,2,4:7 translates to a vector that contains the values
 * 1,2,4,5,6,7. Note that ranges must be increasing
 */
extern vector<int> string_util::int_list_str_to_intVect(std::string &intListStr){

  vector<int> intListVect;
  StrVec strVecList = split( intListStr );
  int ii;
  for (ii=0; ii< strVecList.size(); ii++) {
    string::size_type colon_pos = strVecList[ii].find(":");
    if ( colon_pos == string::npos ) {     //only one integer
      intListVect.resize(intListVect.size() + 1);
      intListVect[intListVect.size() - 1] = str_to_int(trim(strVecList[ii]));
    }
    else{
      string lowStr = strVecList[ii].substr(0, colon_pos);
      int lowInt = str_to_int(trim(lowStr));
      string highStr = 
	strVecList[ii].substr(colon_pos+1, strVecList[ii].size());
      int highInt = str_to_int(trim(highStr));
      int oldSize = intListVect.size();
      if ( highInt < lowInt ) {
	throw runtime_error("intList: Ranges must be increasing\n");
      }
      intListVect.resize(oldSize + (highInt - lowInt + 1));
      int jj;
      for (jj = lowInt; jj <= highInt; jj++) {
	intListVect[oldSize + jj-lowInt] = lowInt + (jj-lowInt);;
      }
    }
  }
  return intListVect;
}

extern string string_util::int_to_str(const int num){
  char temp[20];
  sprintf(temp,"%d",num);
  return string(temp);
}

extern string string_util::intVec_to_str(const vector<int> &vec){

  string result("[");
  for( vector<int>::const_iterator it=vec.begin() ; it!=vec.end() ; it++ ){
    result+=int_to_str(*it);
    if(it+1!=vec.end())
      result+=",";
  }
  result+="]";

  return result;
}
