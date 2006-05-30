#ifndef __STRING_UTIL_H_GUARD
#define __STRING_UTIL_H_GUARD
#include <string>
#include <vector>
#include <stdexcept>

namespace string_util{
  extern bool is_comma(char);
  extern std::vector<std::string> split(const std::string &);
  /**
   * Returns true if the first argument starts with the second.
   */
  extern bool   starts_with(const std::string &, const std::string &);
  extern std::string trim(const std::string &);
  extern long   str_to_int(const std::string &);
  extern unsigned long  str_to_uint(const std::string &);
  extern double str_to_float(const std::string &);
  extern std::vector<std::string> string_to_path(const std::string &);
  extern std::string int_to_str(const int);
  extern std::vector<int> str_to_intVec(std::string &);
  extern void str_to_ucharArray(std::string &,unsigned char *, const unsigned int);
  extern void str_to_shortArray(std::string &,short *, const unsigned int);
  extern void str_to_intArray(std::string &,int *, const unsigned int);
  extern void str_to_longArray(std::string &,long *, const unsigned int);
  extern void str_to_ushortArray(std::string &,unsigned short *, const unsigned int);
  extern void str_to_uintArray(std::string &,unsigned int *, const unsigned int);
  extern void str_to_ulongArray(std::string &,unsigned long *, const unsigned int);
  extern void str_to_floatArray(std::string &,float *, const unsigned int);
  extern void str_to_doubleArray(std::string &,double *, const unsigned int);
  extern std::vector<int> int_list_str_to_intVect(std::string &);
  extern std::string intVec_to_str(const std::vector<int> &);
  extern std::string erase(const std::string &in_str, const std::string &match_str);

/*
 * This method takes a string as an argument and attempts to convert this 
 * string into a vector of integers.  The string is in the form of a comma 
 * separated list.  Each element in the list is a single integer or a range
 * of integers specified by a starting value and ending value separated by
 * a colon.  i.e. 1,2,4:7 translates to a vector that contains the values
 * 1,2,4,5,6,7. Note that ranges must be increasing
 */
  template <class T>
  extern void intListStr_to_intVec(const std::string &intList,
                                                       std::vector<T> &intVec){
    
    using std::string;
    typedef string::size_type string_size;
    typedef std::vector<string> StrVec;
    static const string COLON=":";

    using string_util::split;
    using string_util::trim;
    using string_util::str_to_int;
    using string_util::int_to_str;

    StrVec strVecList = split( intList );
    for( StrVec::const_iterator it=strVecList.begin() ; it!=strVecList.end() ; it++ ){
      string_size colon_pos = it->find(COLON);
      if( colon_pos == string::npos ){     // only one integer
        intVec.push_back(str_to_int(trim(*it)));
      }else{                               // determine range
        // convert end points to integers
        long lowInt  = str_to_int(trim(it->substr(0,colon_pos)));
        long highInt = str_to_int(trim(it->substr(colon_pos+1)));

        // error check
        if ( highInt < lowInt )
          throw std::runtime_error("intList: Ranges must be increasing: "
                             +int_to_str(lowInt)+">"+int_to_str(highInt)+"\n");

        // add to the result vector
        for( long j = lowInt; j <= highInt; j++)
          intVec.push_back(j);
      }
    }
  }

}
#endif
