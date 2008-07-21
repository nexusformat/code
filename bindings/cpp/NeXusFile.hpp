#ifndef NEXUSFILE_HPP
#define NEXUSFILE_HPP 1

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "napi.h"

/**
 * \file
 * Documentation for the NeXus C++ API.
 * 2000-2008 the NeXus group.
 */

namespace NeXus {
  /**
   * The primitive types published by this API.
   * \li FLOAT32 float.
   * \li FLOAT64 double
   * \li INT8 int8_t
   * \li UINT8 uint8_t
   * \li INT16 int16_t
   * \li UINT16 uint16_t
   * \li INT32 int32_t
   * \li UINT32 uint32_t
   * \li INT64 int8_t if available on the machine
   * \li UINT64 uint8_t if available on the machine
   */
  enum NXnumtype {
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

  /**
   * The available compression types. These are all ignored in xml files.
   * \li NONE no compression
   * \li LZW Lossless Lempel Ziv Welch compression (recommended)
   * \li RLE Run length encoding (only HDF-4)
   * \li HUF Huffmann encoding (only HDF-4)
   */
  enum NXcompression {
    NONE = NX_COMP_NONE,
    LZW = NX_COMP_LZW,
    RLE = NX_COMP_RLE,
    HUF = NX_COMP_HUF
  };

  /**
   * This structure holds the type and dimensions of a primative field/array.
   */
  struct Info{
    /** The primative type for the field. */
    NXnumtype type;
    /** The dimensions of the file. */
    std::vector<int> dims;
  };

  /** Information about an attribute. */
  struct AttrInfo{
    /** The primative type for the attribute. */
    NXnumtype type;
    /** The length of the attribute. */
    unsigned length;
    /** The name of the attribute. */
    std::string name;
  };

  /**
   * The Object that allows access to the information in the file.
   */
  class File
  {
  private:
    /** The handle for the C-API. */
    NXhandle m_file_id;
    /**
     * \return A pair of the next entry available in a listing.
     */
    std::pair<std::string, std::string> getNextEntry();
    /**
     * \return Information about the next attribute.
     */
    AttrInfo getNextAttr();
    /**
     * This is a deprecated function.
     * \param com The compression type.
     */
    void compress(NXcompression comp);

  public:
    /**
     * Create a new File.
     *
     * \param filename The name of the file to open.
     * \param access How to access the file.
     */
    File(const std::string& filename, const NXaccess access = NXACC_READ);

    /** Destructor. This does close the file. */
    ~File();

    /** Close the file before the constructor is called. */
    void close();

    /** Flush the file. */
    void flush();

    /**
     * Create a new group.
     *
     * \param name The name of the group to create (i.e. "entry").
     * \param class_name The type of group to create (i.e. "NXentry").
     * \param open_group Whether or not to automatically open the group after 
     * creating it.
     */
    void makeGroup(const std::string& name, const std::string& class_name,
                   bool open_group = false);

    /**
     * Open an existing group.
     *
     * \param name The name of the group to create (i.e. "entry").
     * \param class_name The type of group to create (i.e. "NXentry").
     */
    void openGroup(const std::string& name, const std::string& class_name);

    /**
     * Open the NeXus object with the path specified.
     *
     * \param path A unix like path string to a group or field. The path 
     * string is a list of group names and SDS names separated with a slash,
     * '/' (i.e. "/entry/sample/name").
     */
    void openPath(const std::string& path);

    /**
     * Open the group in which the NeXus object with the specified path exists.
     *
     * \param path A unix like path string to a group or field. The path 
     * string is a list of group names and SDS names separated with a slash,
     * '/' (i.e. "/entry/sample/name").
     */
    void openGroupPath(const std::string& path);

    /**
     * Close the currently open group.
     */
    void closeGroup();

    /**
     * Create a data field with the specified information.
     *
     * \param name The name of the field to create (i.e. "distance").
     * \param type The primative type of the field (i.e. "NeXus::FLOAT32").
     * \param dims The dimensions of the field.
     * \param open_data Whether or not to open the data after creating it.
     */
    void makeData(const std::string& name, NXnumtype type,
                  const std::vector<int>& dims, bool open_data = false);

    /**
     * Create a 1D data field with the specified information.
     *
     * \param name The name of the field to create (i.e. "distance").
     * \param type The primative type of the field (i.e. "NeXus::FLOAT32").
     * \param length The number of elements in the field.
     * \param open_data Whether or not to open the data after creating it.
     */
    void makeData(const std::string& name, const NXnumtype type,
                  const int length, bool open_data = false);

    void writeData(const std::string& name, const std::string& value);

    template <typename NumT>
    void writeData(const std::string& name, const std::vector<NumT>& value);

    template <typename NumT>
    void writeData(const std::string& name, const std::vector<NumT>& value,
                   const std::vector<int>& dims);

    void makeCompData(const std::string& name, const NXnumtype type,
                      const std::vector<int>& dims, const NXcompression comp,
                      const std::vector<int>& bufsize, bool open_data = false);

    template <typename NumT>
    void writeCompData(const std::string & name,
                       const std::vector<NumT> & value,
                       const std::vector<int> & dims, const NXcompression comp,
                       const std::vector<int> & bufsize);

    void openData(const std::string& name);

    void closeData();

    void putData(const void* data);

    template <typename NumT>
    void putData(const std::vector<NumT>& data);

    void putAttr(const AttrInfo& info, const void* data);

    template <typename NumT>
    void putAttr(const std::string& name, const NumT value);

    void putAttr(const char* name, const char* value);

    void putAttr(const std::string& name, const std::string value);

    void putSlab(void* data, std::vector<int>& start,
                 std::vector<int>& size);

    template <typename NumT>
    void putSlab(std::vector<NumT>& data, std::vector<int>& start,
                 std::vector<int>& size);

    template <typename NumT>
    void putSlab(std::vector<NumT>& data, int start, int size);

    NXlink getDataID();

    void makeLink(NXlink& link);

    void makeNamedLink(const std::string& name, NXlink& link);

    void openSourceGroup();

    void getData(void* data);

    // caller needs to free the memory allocated
    template <typename NumT>
    std::vector<NumT> * getData();

    std::string getStrData();

    Info getInfo();

    /**
     * Return the entries available in the current place in the file.
     */
    std::map<std::string, std::string> getEntries();

    void getSlab(void* data, const std::vector<int>& start,
                 std::vector<int>& size);

    std::vector<AttrInfo> getAttrInfos();

    void getAttr(const AttrInfo& info, void* data, int length = -1);

    template <typename NumT>
    void getAttr(const AttrInfo& info, NumT& value);

    void getAttr(const AttrInfo& info, std::string& value);

    std::string getStrAttr(const AttrInfo & info);

    NXlink getGroupID();

    int getGroupInfo(std::string& name, std::string& type);

    bool sameID(NXlink& first, NXlink& second);

    void printLink(NXlink & link);

    void initGroupDir();

    void initAttrDir();

    void setNumberFormat(NXnumtype& type, const std::string& format);

    std::string inquireFile(const int buff_length = NX_MAXNAMELEN);

    std::string isExternalGroup(const std::string& name,
                                const std::string& type,
                                const unsigned buff_length = NX_MAXNAMELEN);
    void linkExternal(const std::string& name, const std::string& type,
                      const std::string& url);
  };

  /**
   * This function returns the NXnumtype given a concrete number.
   */
  template <typename NumT>
  NXnumtype getType(NumT number = NumT());

};

#endif
