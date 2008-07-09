//
// Some bits for the NeXus CPP interface code
// Author: Freddie Akeroyd, STFC ISIS Facility
//
#ifndef NEXUSFILE_HPP
#define NEXUSFILE_HPP

typedef void (*nx_error_t)(void* arg, char* text);

#include <time.h>
#include <string>
#include "napi.h"

namespace NeXus {

class File
{
	NXhandle m_fileid;
    	int m_options;
	int m_compression_type;

public:
	enum Options { None = 0x0, WriteDummyOnNULL = 0x1, Read = 0x2, Write = 0x4, HDF4 = 0x8, HDF5 = 0x10 };
	enum NXReturnCodes { OK = NX_OK, Error = NX_ERROR };
	File(nx_error_t error_func, void* error_arg);
	int open(const char* filename, int mode, int type = File::HDF5);
	int close();
	int getLink(const char* name, const char* nxclass, NXlink& link);
	int makeLink(const char* name, const char* nxclass, NXlink& link);
	int getLink(const char* name, NXlink& link) { return getLink(name, NULL, link); }
	int makeLink(const char* name, NXlink& link) { return makeLink(name, NULL, link); }
	int writeData(const char* data_name, int value);
	int writeData(const char* data_name, unsigned value) { return writeData(data_name, (int)value); }
	int writeData(const char* data_name, long value) { return writeData(data_name, (int)value); }
	int writeData(const char* data_name, unsigned long value) { return writeData(data_name, (long)value); }
	int writeData(const char* data_name, float value);
	int writeData(const char* data_name, double value);
	int writeAttribute(const char* data_name, const char* attr_name, int value);
	int writeAttribute(const char* data_name, const char* attr_name, float value);
	int writeAttribute(const char* data_name, const char* attr_name, double value);
	int writeData(const char* data_name, const int* value, const int* dims_array, int ndims);
	int writeData(const char* data_name, const float* value, const int* dims_array, int ndims);
	int writeData(const char* data_name, const double* value, const int* dims_array, int ndims);
	int writeData(const char* data_name, const int* value, int len);
	int writeData(const char* data_name, const float* value, int len);
	int writeData(const char* data_name, const double* value, int len);
	int writeData(const char* data_name, const int* value, int dim1, int dim2);
	int writeData(const char* data_name, const unsigned long* value, int dim1, int dim2) { return writeData(data_name, (const int*)value, dim1, dim2); }
	int writeData(const char* data_name, const float* value, int dim1, int dim2);
	int writeData(const char* data_name, const double* value, int dim1, int dim2);
	int writeData(const char* data_name, const char* value);
	int writeData(const char* data_name, const char* value, int len);
	int writeData(const char* data_name, const char* value, int dim1, int dim2);
	int makeData(const char* data_name, int nxtype, const int* dims_array, int ndims, int compression_type);
	int writeSlab(const char* data_name, const int* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size);
	int writeSlab(const char* data_name, const unsigned long* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size) { return writeSlab(data_name, (const int*)value, dims_array, ndims, slab_start, slab_size); }
	int addSlab(const char* data_name, const int* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size, const int* dest_start);
	int addSlab(const char* data_name, const unsigned long* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size, const int* dest_start);
	int writeAttribute(const char* data_name, const char* attr_name, const int* value, int len);
	int writeAttribute(const char* data_name, const char* attr_name, const float* value, int len);
	int writeAttribute(const char* data_name, const char* attr_name, const double* value, int len);
	int writeAttribute(const char* data_name, const char* attr_name, const char* value, int len);
	int writeAttribute(const char* data_name, const char* attr_name, const char* value);
	int makeNewGroup(const char* name, const char* nxclass);
	int closeGroup();
	const char* ISOtime(time_t time);
	time_t unixTime(const char* isotime);
	int enableOptions(Options options) { return (m_options |= options); }
	void enableCompression() { m_compression_type = NX_COMP_LZW; } 
	void disableCompression() { m_compression_type = NX_COMP_NONE; } 
	bool testOptions(Options options) { return ((m_options & options) != 0); }

	int readData(const char* data_name, std::string& value);
	int readData(const char* data_name, int& value);
	int listEntries(std::list<std::string>& entries, bool groups_only = false);
	int openGroup(const char* group_name, const char* group_class); 
	~File() { close(); }
};

};

#endif /* NEXUSFILE_HPP */
