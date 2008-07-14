#ifndef NEXUSFILE_HPP
#define NEXUSFILE_HPP 1

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "napi.h"

namespace NeXus {
  typedef enum NXnumtype {
    FLOAT32 = NX_FLOAT32,
    FLOAT64 = NX_FLOAT64,
    INT8 = NX_INT8,
    UINT8 = NX_UINT8,
    // BOOLEAN = NX_BOOLEAN, // NX_BOOLEAN is currently broken
    INT16 = NX_INT16,
    UINT16 = NX_UINT16,
    INT32 = NX_INT32,
    UINT32 = NX_UINT32,
    INT64 = NX_INT64,
    UINT64 = NX_UINT64,
    CHAR = NX_CHAR,
    BINARY = NX_BINARY
  };

  typedef enum NXcompression {
    NONE = NX_COMP_NONE,
    LZW = NX_COMP_LZW,
    RLE = NX_COMP_RLE,
    HUF = NX_COMP_HUF
  };

  struct Info{
    NXnumtype type;
    std::vector<int> dims;
  };

  struct AttrInfo{
    NXnumtype type;
    uint length;
    std::string name;
  };

  class File
  {
  private:
    NXhandle file_id;
    std::pair<std::string, std::string> getNextEntry();
    AttrInfo getNextAttr();
    void compress(NXcompression comp);

  public:
    File(const std::string filename, const NXaccess access = NXACC_READ);
    
    ~File();

    void flush();

    void makeGroup(const std::string & name, const std::string & class_name);

    void openGroup(const std::string & name, const std::string & class_name);

    void openPath(const std::string & path);

    void openGroupPath(const std::string & path);

    void closeGroup();

    void makeData(const std::string & name, NXnumtype type,
                  const std::vector<int> & dims);

    void writeData(const std::string & name, const std::string & value);

    template <typename NumT>
    void writeData(const std::string & name, const std::vector<NumT> & value);

    template <typename NumT>
    void writeData(const std::string & name, const std::vector<NumT> & value,
                   const std::vector<int> & dims);

    void makeCompData(const std::string & name, NXnumtype type,
                      std::vector<int> & dims, NXcompression comp,
                      std::vector<int> & bufsize);

    void openData(const std::string & name);

    void closeData();

    void putData(const void * data);

    template <typename NumT>
    void putData(const std::vector<NumT> & data);

    void putAttr(const AttrInfo & info, const void * data);

    template <typename NumT>
    void putAttr(const std::string & name, const NumT value);

    void putAttr(const char * name, const char * value);

    void putAttr(const std::string & name, const std::string value);

    void putSlab(void * data, std::vector<int> & start,
                 std::vector<int> & size);

    template <typename NumT>
    void putSlab(std::vector<NumT> & data, std::vector<int> & start,
                 std::vector<int> & size);

    template <typename NumT>
    void putSlab(std::vector<NumT> & data, int start, int size);

    NXlink getDataID();

    void makeLink(NXlink & link);

    void makeNamedLink(const std::string & name, NXlink & link);

    void openSourceGroup();

    void getData(void * data);

    template <typename NumT>
    void getData(std::vector<NumT> & data);

    Info getInfo();

    /**
     * Return the entries available in the current place in the file.
     */
    std::map<std::string, std::string> getEntries();

    void getSlab(void * data, const std::vector<int> & start,
                 std::vector<int> & size);

    std::vector<AttrInfo> getAttrInfos();

    void getAttr(AttrInfo & info, void * data);

    template <typename NumT>
    void getAttr(AttrInfo & info, NumT & value);

    std::string getStrAttr(AttrInfo & info);

    NXlink getGroupID();

    int getGroupInfo(std::string & name, std::string & type);

    bool sameID(NXlink & first, NXlink & second);

    void initGroupDir();

    void initAttrDir();

    void setNumberFormat(NXnumtype & type, const std::string &format);

    std::string inquireFile(const int buff_length = NX_MAXNAMELEN);

    std::string isExternalGroup(const std::string &name,
                                const std::string & type,
                                 const uint buff_length = NX_MAXNAMELEN);
    void linkExternal(const std::string & name, const std::string & type,
                      const std::string & url);
  };

  void malloc(void ** data, std::vector<int> & dims, NXnumtype type);

  void free(void **data);
};

#endif
