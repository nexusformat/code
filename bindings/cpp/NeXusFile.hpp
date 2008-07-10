#ifndef NEXUSFILE_HPP
#define NEXUSFILE_HPP 1

#include <map>
#include <string>
#include <utility>
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

  class File
  {
  private:
    NXhandle file_id;

  public:
    File(const std::string filename, const NXaccess access = NXACC_READ);
    
    ~File();

    void flush();

    void initGroupDir();

    std::pair<std::string, std::string> getNextEntry();

    /**
     * Return the entries available in the current place in the file.
     */
    std::map<std::string, std::string> getEntries();
  };
};

#endif
