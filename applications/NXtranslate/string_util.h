#include <string>
#include <vector>

namespace string_util{
  extern bool   starts_with(const std::string &, const std::string &);
  extern std::string trim(const std::string &);
  extern long   str_to_int(const std::string &);
  extern ulong  str_to_uint(const std::string &);
  extern double str_to_float(const std::string &);
  extern std::vector<std::string> string_util::string_to_path(const std::string &);
  extern std::string int_to_str(const int);
  extern std::vector<int> str_to_intVec(std::string &);
  extern void str_to_ucharArray(std::string &,unsigned char *, const uint);
  extern void str_to_shortArray(std::string &,short *, const uint);
  extern void str_to_intArray(std::string &,int *, const uint);
  extern void str_to_longArray(std::string &,long *, const uint);
  extern void str_to_ushortArray(std::string &,ushort *, const uint);
  extern void str_to_uintArray(std::string &,uint *, const uint);
  extern void str_to_ulongArray(std::string &,ulong *, const uint);
  extern void str_to_floatArray(std::string &,float *, const uint);
  extern void str_to_doubleArray(std::string &,double *, const uint);
  extern std::vector<int> int_list_str_to_intVect(std::string &);
  extern std::string intVec_to_str(const std::vector<int> &);
}
