/** 
 * \file nexus_util.hpp
 * \brief The class declaration and prototypes for
 *        NexusUtil.cpp.
 */

#ifndef _NEXUS_UTIL_HPP
#define _NEXUS_UTIL_HPP 1

#include "napi.h"
#include <string>
#include <vector>

/**
 * \enum e_nx_access
 * \brief Enumeration for the types of nexus file
 *        access. This allows the compiler to 
 *        check for errors.
 */
typedef enum e_nx_access 
{ 
  READ = NXACC_READ, 
  RDWR = NXACC_RDWR, 
  CREATE = NXACC_CREATE, 
  HDF_FOUR = NXACC_CREATE4, 
  HDF_FIVE = NXACC_CREATE5, 
  XML = NXACC_CREATEXML 
};

/**
 * \enum e_nx_data_type
 * \brief Enumeration for all the types of nexus
 *        data. This allows the compiler to check for
 *        possible errors.
 */
typedef enum e_nx_data_type
{
  CHAR = NX_CHAR,
  FLOAT32 = NX_FLOAT32, 
  FLOAT64 = NX_FLOAT64, 
  INT8 = NX_INT8, 
  UINT8 = NX_UINT8, 
  INT16 = NX_INT16, 
  UINT16 =  NX_UINT16, 
  INT32 = NX_INT32, 
  UINT32 = NX_UINT32

#ifdef NX_INT64
  , INT64 = NX_INT64
#endif

#ifdef NX_UINT64
  , UINT64 = NX_UINT64
#endif
};

/** 
 * \class NexusUtil
 * \brief A nexus utility that opens a file in the
 *        constructor and stores the file handle
 *        as a private variable. The functions also
 *        do appropriate error checking.
 */
class NexusUtil
{
  private:
    NXhandle file_id;
    
    /**
     * \brief Converts an integer to a valid nexus data
     *        type enumeration.
     * \param data_type The nexus data type.
     */
    e_nx_data_type get_nx_data_type(int data_type);

  public:
    /** 
     * \brief Constructor that opens the nexus file with the 
     *        specified format.
     * \param file_path The file path and name to be read or written.
     * \param file_access The enumeration specifying how the file 
     *                    should be opened. Ex - hdf4 write or read.
     * \exception runtime_error Thrown if file doesn't exist or can't be
     *                          opened.
     */
    NexusUtil(const std::string & file_path, 
              const e_nx_access file_access);

    /**
     * \brief Destructor that closes the nexus file.
     * \exception runtime_error Thrown if nexus file can't be closed.
     */
    ~NexusUtil(void);

    /** 
     * \brief Makes a group in a nexus file while checking for
     *        errors.
     * \param name The name of the group.
     * \param path The path to the group.
     * \exception runtime_error Thrown if name or path are blank or if
     *                          group can't be made.
     */
    void make_group(const std::string & name, 
                    const std::string & path);
    
    /** 
     * \brief Opens a group in a nexus file while checking for
     *        errors.
     * \param name The name of the group to be opened.
     * \param path The path to the group.
     * \exception runtime_error Thrown if name or path are blank or if
     *                          group can't be opened.
     */
    void open_group(const std::string & name,  
                    const std::string & path);
    
    /**
     * \brief Closes a group in a nexus file while checking for
     *        errors.
     * \exception runtime_error Thrown if group can't be closed.
     */
    void close_group(void);
    
    /** 
     * \brief Opens a path in a nexus file while checking for
     *        errors.
     * \param path The path to be opened.
     * \exception runtime_error Thrown if path is blank or can't
     *                          be opened.
     */
    void open_path(const std::string & path);
    
    /** 
     * \brief Makes data in a nexus file while checking for
     *        errors.
     * \param name The name of the data.
     * \param nx_data_type The enumeration specifying what type
     *                     of nexus data.
     * \param rank The rank of the data.
     * \param dimensions The array of dimensions.
     * \exception runtime_error Thrown if any of the fields are blank or
     *                          if the data can't be made.
     */
    void make_data(const std::string & name, 
                   const e_nx_data_type nx_data_type, 
                   int rank, 
                   int * dimensions);
    
    /**
     * \brief Opens data in a nexus file while checking for
     *        errors.
     * \param name The name of the data to be opened.
     * \exception runtime_error Thrown if name is blank or data can't
     *                          be opened.
     */
    void open_data(const std::string & name);
    
    /** 
     * \brief Writes data to a nexus file while checking for
     *        errors.
     * \param nx_data The data to be written.
     * \exception runtime_error Thrown if nx_data is NULL or if it 
     *                          can't be written.
     */
    void put_data(void * nx_data);

    /** 
     * \brief Writes data to a nexus file while checking for
     *        errors.
     * \param nx_data The vector of data to be written to the
     *                nexus file.
     * \exception runtime_error Thrown if data is empty of if
     *                          writing data fails. 
     */
    template <typename NumT>
    void put_data(const std::vector<NumT> & nx_data);
    
    /**
     * \brief Closes data in a nexus file while checking for
     *        errors.
     * \exception runtime_error Thrown if data can't be closed.
     */
    void close_data(void);
    
    /**
     * \brief Writes an attribute to a piece of data 
     *        while checking for errors.
     * \param name The name of the attribute.
     * \param value The attribute value.
     * \param length The length of the attribute.
     * \param nx_data_type The enumeration specifying what type
     *                     of nexus data.
     * \exception runtime_error Thrown if any of the parameters are
     *                          empty or NULL or if the attribute can't
     *                          be written.
     */
    void put_attr(const std::string & name, 
                  void * value, 
                  int length, 
                  const e_nx_data_type nx_data_type);

    /**
     * \brief Overloaded function that is specifically
     *        meant for strings.
     * \param name The name of the attribute.
     * \param value The value of the attribute.
     * \exception runtime_error Thrown if the name of value is blank or
     *                          if the attribute can't be written.
     */
    void put_attr(const std::string & name,
                  const std::string & value);
    
    /**
     * \brief Writes a chunk of data to a nexus file 
     *        while checking for errors.
     * \param nx_data The data to be written.
     * \param start The starting indecies in the data.
     * \param size The length of data in each dimension.
     * \exception runtime_error Thrown if any of the parameters are
     *                          NULL or if the slab can't be written.
     */
    void put_slab(void * nx_data, 
                  int * start,
                  int * size);

    /**
     * \brief Writes a chunk of a vector to a nexus file 
     *        while checking for errors.
     * \param nx_data The vector of data.
     * \param start The starting index in the vector.
     * \param block_size The size of the chunk of data to 
     *                   be written.
     * \exception runtime_error Thrown if the data is NULL or
     *                          if the slab can't be written.
     */
    template <typename NumT>
    void put_slab(const std::vector<NumT> & nx_data, 
                  const int start,
                  const int block_size);

    /**
     * \brief Writes all the data using put_slab and a loop
     *        with a given block size. Also error checks.
     * \param nx_data The data to be written.
     * \param block_size The size of the blocks to write to the
     *                   file.
     * \exception runtime_error Thrown if data is empty or if
     *                          the slab can't be written.
     */
    template <typename NumT>
    void NexusUtil::put_data_with_slabs(std::vector<NumT> & nx_data,
                                        int block_size);

    /**
     * \brief Gets data from the nexus file 
     *        while checking for errors.
     * \param nx_data The pointer to the memory the data will be 
     *                stored in.
     * \exception runtime_error Thrown if the data is NULL or if it
     *                          can't be stored.
     */
    void get_data(void * nx_data);
    
    /**
     * \brief Gets a chunk of data from the nexus file 
     *        while checking for errors.
     * \param nx_data The memory for the data chunk to be stored in.
     * \param start The array of starting indecies in data memory.
     * \param size The array of sizes to read in each dimension.
     * \exception runtime_error Thrown if any of the parameters are
     *                          NULL or if the slab can't be stored.
     */
    void get_slab(void * nx_data, 
                  int * start, 
                  int * size);

    /**
     * \brief Uses NXmalloc while checking for errors.
     * \param nx_data The memory to be allocated.
     * \param rank The rank of the memory.
     * \param dimensions The array of dimensions to allocate.
     * \param nx_data_type The enumeration specifying what type
     *                     of data to allocate.
     * \exception runtime_error Thrown if any of the parameters are
     *                          NULL or if the malloc call fails.
     */
    void malloc(void ** nx_data,
                int rank,
                int * dimensions,
                const e_nx_data_type nx_data_type);

    /** 
     * \brief Calls NXfree while checking for errors.
     * \param nx_data The data to be released.
     * \exception runtime_error Thrown if the free call fails.
     */
    void free(void ** nx_data);

    /**
     * \brief Gets nexus data information 
     *        while checking for errors.
     * \param rank The integer to store the rank value in.
     * \param dimensions The array to store the dimensions in.
     * \param nx_data_type The enumerated data type to store 
     *                     the nexus data type in.
     * \exception runtime_error Thrown if any of the parameters are
     *                          NULL or if getting info fails.
     */
    void get_info(int * rank,
                  int * dimensions,
                  e_nx_data_type & nx_data_type);
};

#endif
