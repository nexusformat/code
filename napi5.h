/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface (HDF5) Header File
  
  Copyright (C) 1997-2002 Mark Koennecke, Przemek Klosowski
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
             
  For further information, see <http://www.neutron.anl.gov/NeXus/>
  
  $Id$

----------------------------------------------------------------------------*/
#define NX5SIGNATURE 959695

#include <hdf5.h>
#include "napi5.c"

/* HDF5 interface */

  NXstatus CALLING_STYLE NX5open(CONSTCHAR *filename, NXaccess access_method, NXhandle* pHandle);
  NXstatus CALLING_STYLE NX5close(NXhandle* pHandle);
  NXstatus CALLING_STYLE NX5flush(NXhandle* pHandle);
  
  NXstatus CALLING_STYLE NX5makegroup (NXhandle handle, CONSTCHAR *name, char* NXclass);
  NXstatus CALLING_STYLE NX5opengroup (NXhandle handle, CONSTCHAR *name, char* NXclass);
  NXstatus CALLING_STYLE NX5closegroup(NXhandle handle);
  
  NXstatus CALLING_STYLE NX5makedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
  NXstatus CALLING_STYLE NX5compmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
  NXstatus CALLING_STYLE NX5compress (NXhandle handle, int compr_type);
  NXstatus CALLING_STYLE NX5opendata (NXhandle handle, CONSTCHAR* label);
  NXstatus CALLING_STYLE NX5closedata(NXhandle handle);
  NXstatus CALLING_STYLE NX5putdata(NXhandle handle, void* data);

  NXstatus CALLING_STYLE NX5putattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
  NXstatus CALLING_STYLE NX5putslab(NXhandle handle, void* data, int start[], int size[]);    

  NXstatus CALLING_STYLE NX5getdataID(NXhandle handle, NXlink* pLink);
  NXstatus CALLING_STYLE NX5makelink(NXhandle handle, NXlink* pLink);

  NXstatus CALLING_STYLE NX5getdata(NXhandle handle, void* data);
  NXstatus CALLING_STYLE NX5getinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
  NXstatus CALLING_STYLE NX5getnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);

  NXstatus CALLING_STYLE NX5getslab(NXhandle handle, void* data, int start[], int size[]);
  NXstatus CALLING_STYLE NX5getnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
  NXstatus CALLING_STYLE NX5getattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
  NXstatus CALLING_STYLE NX5getattrinfo(NXhandle handle, int* no_items);
  NXstatus CALLING_STYLE NX5getgroupID(NXhandle handle, NXlink* pLink);
  NXstatus CALLING_STYLE NX5getgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);
  NXstatus CALLING_STYLE NX5sameID(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);

  NXstatus CALLING_STYLE NX5initgroupdir(NXhandle handle);
  NXstatus CALLING_STYLE NX5initattrdir(NXhandle handle);


herr_t nxgroup_info(hid_t loc_id, const char *name, void *op_data);
herr_t attr_info(hid_t loc_id, const char *name, void *opdata);
herr_t group_info(hid_t loc_id, const char *name, void *opdata);

