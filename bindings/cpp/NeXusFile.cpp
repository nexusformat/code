//
// Some bits for the NeXus CPP interface code
// Author: Freddie Akeroyd, STFC ISIS Facility
//
#include <list>
#include <cstring>
#include <cstdlib>
#include "NeXusFile.hpp"

template<typename T>
	static const T* nullValue(const T* value, int NXTYPE)
	{
		static const int izero = 0;
		static const float fzero = 0.0;
		static const double dzero = 0.0;
		switch(NXTYPE)
		{
			case NX_INT32:
				return (T*)&izero;
			case NX_FLOAT32:
				return (T*)&fzero;
			case NX_FLOAT64:
				return (T*)&dzero;
			case NX_CHAR:
				return (T*)" ";
			default:
				return NULL;
		}
	}

template<typename T, int NXTYPE>
	static int writeAttribute(NXhandle fileid, const char* data_name, 
						const char* attr_name, const T* value, int len, bool convert_null)
	{
		if (len == 0)
		{
			if (convert_null)
			{
				value = nullValue(value, NXTYPE);
				len = 1;
			}
			else
			{
				return 0;
			}
		}
		if (NXopendata(fileid, data_name) == NX_OK)
		{
			NXputattr(fileid, attr_name, (void*)value, len, NXTYPE);
			NXclosedata(fileid);
		}
		return 0;
	}

template<typename T, int NXTYPE>
	static int writeAttribute(NXhandle fileid, const char* data_name, 
				const char* attr_name, T value, bool convert_null)
	{
		return writeAttribute<T,NXTYPE>(fileid, data_name, attr_name, &value, 1, convert_null);
	}

template<typename T, int NXTYPE>
	static int writeData(NXhandle fileid, const char* data_name, 
			const T* value, const int* dims_array, int ndims, bool convert_null, int compression_type)
	{
		NXstatus ret;
		int i, len = 1;
		int dummy_dims_array[1] = { 1 };
		int* chunk_size = new int[ndims];
		for(i=0; i<ndims; i++)
		{
			len *= dims_array[i];
		}
		if (len == 0)
		{
			if (convert_null)
			{
				value = nullValue(value, NXTYPE);
				ndims = 1;
				dims_array = dummy_dims_array;
			}
			else
			{
				delete []chunk_size;
				return 0;
			}
		}
		// we want only the final array dimension to be compressed
		for(i=0; i<ndims; i++)
		{
			chunk_size[i] = 1;
		}
		chunk_size[ndims-1] = dims_array[ndims-1];
		if (compression_type != NX_COMP_NONE)
		{
			ret = NXcompmakedata(fileid, data_name, NXTYPE, ndims, (int*)dims_array, compression_type, chunk_size);
		}
		else
		{
			ret = NXmakedata(fileid, data_name, NXTYPE, ndims, (int*)dims_array);
		}
		delete []chunk_size;
		if (ret != NX_OK)
		{
			return 0;
		}
		NXopendata(fileid, data_name);
		NXputdata(fileid, (void*)value);
		NXclosedata(fileid);
		return 0;
	}



// we want to write a portion of an array to the file
// the passed array is of size (dims_array[ndims]) but we only
// want to write a slab of it
// only for ndims = 1 or 2 at moment
template<typename T, int NXTYPE>
	static int addSlab(NXhandle fileid, const char* data_name, 
			const T* value, const int* dims_array, int ndims, 
			const int* slab_start, const int* slab_size, 
			const int* dest_start, int compression_type)
	{
		const int BUFFER_SIZE = 1024 * 1024;
		NXstatus ret;
		int i, j, offset;
		int dims[10], sizes[10];
		NXopendata(fileid, data_name);
		if (ndims == 1)
		{
			NXputslab(fileid, (void*)(value+slab_start[0]), (int*)dest_start, (int*)slab_size);
		} 
		else if (ndims == 2)
		{
			dims[1] = dest_start[1]; sizes[0] = 1; sizes[1] = slab_size[1];
			for(i=0; i<slab_size[0]; i++)
			{
				offset = (i + slab_start[0]) * dims_array[1] + slab_start[1];
				dims[0] = i + dest_start[0];
				NXputslab(fileid, (void*)(value + offset), dims, sizes);
			}
		}
		else if (ndims == 3)
		{
			dims[2] = dest_start[2]; sizes[0] = 1; sizes[1] = 1; sizes[2] = slab_size[2];
			for(i=0; i<slab_size[0]; i++)
			{
				for(j=0; j<slab_size[1]; j++)
				{
					offset = (i + slab_start[0]) * (dims_array[1] * dims_array[2]) + 
							 (j + slab_start[1]) * dims_array[2] + 
							 slab_start[2];
					dims[1] = j + dest_start[1]; 
					dims[0] = i + dest_start[0];
					NXputslab(fileid, (void*)(value + offset), dims, sizes);
				}
			}
		}
		NXclosedata(fileid);
		return 0;
	}
	
// we want to write a portion of an array to the file
// the passed array is of size (dims_array[ndims]) but we only
// want to write a slab of it
// only for ndims = 1 or 2 at moment
template<typename T, int NXTYPE>
	static int writeSlab(NXhandle fileid, const char* data_name, 
			const T* value, const int* dims_array, int ndims, 
			const int* slab_start, const int* slab_size, bool convert_null, int compression_type)
	{
		const int BUFFER_SIZE = 1024 * 1024;
		NXstatus ret;
		int zero = 0;
		int i, j, offset;
		int dims[10], sizes[10];
		int* chunk_size = new int[ndims];
		// we want only the final array dimension to be compressed
		for(i=0; i<ndims; i++)
		{
			chunk_size[i] = 1;
		}
		chunk_size[ndims-1] = slab_size[ndims-1];
		if (compression_type != NX_COMP_NONE)
		{
			ret = NXcompmakedata(fileid, data_name, NXTYPE, ndims, (int*)slab_size, compression_type, chunk_size);
		}
		else
		{
			ret = NXmakedata(fileid, data_name, NXTYPE, ndims, (int*)slab_size);
		}
		delete[] chunk_size;
		if (ret != NX_OK)
		{
			return 0;
		}
		NXopendata(fileid, data_name);
		if (ndims == 1)
		{
			NXputslab(fileid, (void*)(value+slab_start[0]), &zero, (int*)slab_size);
		} 
		else if (ndims == 2)
		{
			dims[1] = 0; sizes[0] = 1; sizes[1] = slab_size[1];
			for(i=0; i<slab_size[0]; i++)
			{
				offset = (i + slab_start[0]) * dims_array[1] + slab_start[1];
				dims[0] = i;
				NXputslab(fileid, (void*)(value + offset), dims, sizes);
			}
		}
		else if (ndims == 3)
		{
			dims[2] = 0; sizes[0] = 1; sizes[1] = 1; sizes[2] = slab_size[2];
			for(i=0; i<slab_size[0]; i++)
			{
				for(j=0; j<slab_size[1]; j++)
				{
					offset = (i + slab_start[0]) * (dims_array[1] * dims_array[2]) + 
							 (j + slab_start[1]) * dims_array[2] + 
							 slab_start[2];
					dims[1] = j; 
					dims[0] = i;
					NXputslab(fileid, (void*)(value + offset), dims, sizes);
				}
			}
		}
		NXclosedata(fileid);
		return 0;
	}

template<typename T, int NXTYPE>
	static int writeData(NXhandle fileid, const char* data_name, T value, bool convert_null, int compression_type)
	{
		int one(1);
		return writeData<T,NXTYPE>(fileid, data_name, &value, &one, 1, convert_null, compression_type);
	}
	
template<typename T, int NXTYPE>
	static int writeData(NXhandle fileid, const char* data_name, const T* value, int len, bool convert_null, int compression_type)
	{
		return writeData<T,NXTYPE>(fileid, data_name, value, &len, 1, convert_null, compression_type);
	}
	
template<typename T, int NXTYPE>
	static int writeData(NXhandle fileid, const char* data_name, const T* value, int dim1, int dim2, bool convert_null, int compression_type)
	{
		int dims_array[2] = { dim1, dim2 };
		return writeData<T,NXTYPE>(fileid, data_name, value, dims_array, 2, convert_null, compression_type);
	}


namespace NeXus
{

File::File(nx_error_t error_func, void* error_arg) : m_fileid(0), m_options(0), m_compression_type(NX_COMP_NONE) 
{
	NXMSetError(error_arg, error_func);
}

int File::open(const char* filename, int mode, int type)
{
	if (mode == File::Write)
	{
		remove(filename);
		if (type == File::HDF5)
		{
			return NXopen(filename, NXACC_CREATE5, &m_fileid);
		}
		else
		{
			return NXopen(filename, NXACC_CREATE, &m_fileid);
		}
	}
	else if (mode == File::Read)
	{
		return NXopen(filename, NXACC_READ, &m_fileid);
	}
	else
	{
		m_fileid = 0;
		return File::Error;
	}
}

int File::getLink(const char* name, const char* nxclass, NXlink& link)
{
	if (NXopendata(m_fileid, name)  == NX_OK)
	{
		NXgetdataID(m_fileid, &link);
		return NXclosedata(m_fileid);
	}
	else if (NXopengroup(m_fileid, name, nxclass) == NX_OK)
	{
		NXgetgroupID(m_fileid, &link);
		return NXclosegroup(m_fileid);
	}
	else
	{
		return NX_ERROR;
	}
}

int File::makeLink(const char* name, const char* nxclass, NXlink& link)
{
	return NXmakelink(m_fileid, &link);
}

int File::close()
{
	int stat = NX_OK;
	if (m_fileid != NULL)
	{
	    stat = NXclose(&m_fileid);
	    m_fileid = NULL;
	}
	return stat;
}

int File::addSlab(const char* data_name, const int* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size, const int* dest_start)
{
	return ::addSlab<int,NX_INT32>(m_fileid, data_name, value, dims_array, ndims, slab_start, slab_size, dest_start, m_compression_type);
}

int File::addSlab(const char* data_name, const unsigned long* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size, const int* dest_start)
{
	return ::addSlab<unsigned long,NX_INT32>(m_fileid, data_name, value, dims_array, ndims, slab_start, slab_size, dest_start, m_compression_type);
}

int File::writeData(const char* data_name, int value)
{
	return ::writeData<int,NX_INT32>(m_fileid, data_name, value, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const int* value, const int* dims_array, int ndims)
{
	return ::writeData<int,NX_INT32>(m_fileid, data_name, value, dims_array, ndims, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const int* value, int len)
{
	return ::writeData<int,NX_INT32>(m_fileid, data_name, value, len, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const int* value, int dim1, int dim2)
{
	return ::writeData<int,NX_INT32>(m_fileid, data_name, value, dim1, dim2, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeSlab(const char* data_name, const int* value, const int* dims_array, int ndims, const int* slab_start, const int* slab_size)
{
	return ::writeSlab<int,NX_INT32>(m_fileid, data_name, value, dims_array, ndims, slab_start, slab_size, testOptions(WriteDummyOnNULL), m_compression_type);
}


int File::writeData(const char* data_name, float value)
{
	return ::writeData<float,NX_FLOAT32>(m_fileid, data_name, value, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const float* value, const int* dims_array, int ndims)
{
	return ::writeData<float,NX_FLOAT32>(m_fileid, data_name, value, dims_array, ndims, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const float* value, int len)
{
	return ::writeData<float,NX_FLOAT32>(m_fileid, data_name, value, len, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const float* value, int dim1, int dim2)
{
	return ::writeData<float,NX_FLOAT32>(m_fileid, data_name, value, dim1, dim2, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, double value)
{
	return ::writeData<double,NX_FLOAT64>(m_fileid, data_name, value, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const double* value, const int* dims_array, int ndims)
{
	return ::writeData<double,NX_FLOAT64>(m_fileid, data_name, value, dims_array, ndims, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const double* value, int len)
{
	return ::writeData<double,NX_FLOAT64>(m_fileid, data_name, value, len, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const double* value, int dim1, int dim2)
{
	return ::writeData<double,NX_FLOAT64>(m_fileid, data_name, value, dim1, dim2, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeData(const char* data_name, const char* value, int dim1, int dim2)
{
	return ::writeData<char,NX_CHAR>(m_fileid, data_name, value, dim1, dim2, testOptions(WriteDummyOnNULL), m_compression_type);
}

int File::writeAttribute(const char* data_name, const char* attr_name, const int* value, int len)
{
	return ::writeAttribute<int,NX_INT32>(m_fileid, data_name, attr_name, value, len, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, float value)
{
	return ::writeAttribute<float,NX_FLOAT32>(m_fileid, data_name, attr_name, value, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, const float* value, int len)
{
	return ::writeAttribute<float,NX_FLOAT32>(m_fileid, data_name, attr_name, value, len, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, double value)
{
	return ::writeAttribute<double,NX_FLOAT64>(m_fileid, data_name, attr_name, value, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, const double* value, int len)
{
	return ::writeAttribute<double,NX_FLOAT64>(m_fileid, data_name, attr_name, value, len, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, const char* value, int len)
{
	return ::writeAttribute<char,NX_CHAR>(m_fileid, data_name, attr_name, value, len, testOptions(WriteDummyOnNULL));
}

int File::writeAttribute(const char* data_name, const char* attr_name, const char* value)
{
	return writeAttribute(data_name, attr_name, value, strlen(value));
}


int File::makeNewGroup(const char* name, const char* nxclass)
{
	NXmakegroup(m_fileid, name, (char*)nxclass);
	NXopengroup(m_fileid, name, (char*)nxclass);
	return 0;
}

int File::closeGroup()
{
	NXclosegroup(m_fileid);
	return 0;
}

const char* File::ISOtime(time_t time)
{
	static char buffer[256];
	struct tm *tm_struct;
	tm_struct = localtime(&time);
	if (tm_struct != 0)
	{
		strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_struct);
	}
	else
	{
		strncpy(buffer, "<UNKNOWN>", sizeof(buffer));
	}
	buffer[sizeof(buffer)-1] = '\0';
	return buffer;
}


// ISO time of form "2004-11-10T17:04:54"
// return -1 on error
time_t File::unixTime(const char* isotime)
{
	int n;
	struct tm tm_struct;
	if (strlen(isotime) < 19)
	{
		return (time_t)-1;
	}
	n = sscanf(isotime, "%d-%d-%dT%d:%d:%d", 
				&tm_struct.tm_year, &tm_struct.tm_mon, &tm_struct.tm_mday,
				&tm_struct.tm_hour, &tm_struct.tm_min, &tm_struct.tm_sec);
	
	if (n != 6)  // enginx had " " rather than "T" separating the bits
	{   
		n = sscanf(isotime, "%d-%d-%d %d:%d:%d", 
					&tm_struct.tm_year, &tm_struct.tm_mon, &tm_struct.tm_mday,
					&tm_struct.tm_hour, &tm_struct.tm_min, &tm_struct.tm_sec);
	}
	if (n != 6)
	{
		return (time_t)-1;
	}
	tm_struct.tm_year -= 1900;	// year field is 1900 based
	tm_struct.tm_mon -= 1;		// January is 0 not 1 in this structure
	tm_struct.tm_isdst = -1;	// status of DST is unknown - CRT will decide
	return mktime(&tm_struct);
}


int File::readData(const char* data_name, std::string& value)
{
	int n, rank, dims[20], data_type;
	char* char_data;
	if (NXopendata(m_fileid, data_name) == NX_OK)
	{
		NXgetinfo(m_fileid, &rank, dims, &data_type);
		if (rank == 1 && data_type == NX_CHAR)
		{
			n = dims[0];
			char_data = new char[n + 1];
			NXgetdata(m_fileid, char_data);
			char_data[n] = '\0';
			value = char_data;
		}
		else
		{
			value = "";
		}
		NXclosedata(m_fileid);
		return File::OK;
	}
	else
	{
		return File::Error;
	}
}

int File::readData(const char* data_name, int& value)
{
	int rank, dims[20], data_type;
	if (NXopendata(m_fileid, data_name) == NX_OK)
	{
		NXgetinfo(m_fileid, &rank, dims, &data_type);
		if (rank == 1 && data_type == NX_INT32 && dims[0] == 1)
		{
			NXgetdata(m_fileid, &value);
		}
		else
		{
			value = 0;
		}
		NXclosedata(m_fileid);
		return File::OK;
	}
	else
	{
		return File::Error;
	}
}

int File::listEntries(std::list<std::string>& entries, bool groups_only)
{
	char entry_name[100], entry_class[100];
	int data_type;
	entries.clear();
	NXinitgroupdir(m_fileid);
	while(NXgetnextentry(m_fileid, entry_name, entry_class, &data_type) == NX_OK)
	{
		if ( groups_only && ((strlen(entry_class) == 0) || !strcmp(entry_class, "SDS")) ) 
		{
			;
		}
		else
		{
			entries.push_back(entry_name);
		}
	}
	return File::OK;
}

int File::openGroup(const char* group_name, const char* group_class) 
{
	return NXopengroup(m_fileid, group_name, (char*)group_class);
}

// we want to write a portion of an array to the file
// the passed array is of size (dims_array[ndims]) but we only
// want to write a slab of it
// only for ndims = 1 or 2 at moment
	int File::makeData(const char* data_name, int nxtype, const int* dims_array, int ndims, int compression_type)
	{
		NXstatus ret;
		int len = 1;
		int dummy_dims_array[1] = { 1 };
		int* chunk_size = new int[ndims];
		for(int i=0; i<ndims; i++)
		{
			len *= dims_array[i];
		}
		if (len == 0)
		{
			delete []chunk_size;
			return NX_ERROR;
		}
		// we want only the final array dimension to be compressed
		for(int i=0; i<ndims; i++)
		{
			chunk_size[i] = 1;
		}
		chunk_size[ndims-1] = dims_array[ndims-1];
		if (compression_type != NX_COMP_NONE)
		{
			ret = NXcompmakedata(m_fileid, data_name, nxtype, ndims, (int*)dims_array, compression_type, chunk_size);
		}
		else
		{
			ret = NXmakedata(m_fileid, data_name, nxtype, ndims, (int*)dims_array);
		}
		delete []chunk_size;
		return ret;
	}



}
