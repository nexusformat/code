/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface (HDF4) Header File
  
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

----------------------------------------------------------------------------*/
#define NXSIGNATURE 959697

#include "napi4.c"

/* 
 * HDF4 interface 
 */

NX_EXTERNAL  NXstatus CALLING_STYLE NX4open(CONSTCHAR *filename, NXaccess access_method, NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4close(NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4flush(NXhandle* pHandle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4makegroup (NXhandle handle, CONSTCHAR* Vgroup, char* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4opengroup (NXhandle handle, CONSTCHAR* Vgroup, char* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4closegroup(NXhandle handle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4makedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4compmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4compress (NXhandle handle, int compr_type);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4opendata (NXhandle handle, CONSTCHAR* label);

NX_EXTERNAL  NXstatus CALLING_STYLE NX4closedata(NXhandle handle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getdata(NXhandle handle, void* data);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getslab(NXhandle handle, void* data, int start[], int size[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4putdata(NXhandle handle, void* data);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4putslab(NXhandle handle, void* data, int start[], int size[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4putattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4initgroupdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getattrinfo(NXhandle handle, int* no_items);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4initattrdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getgroupID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4getdataID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4makelink(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NX4sameID(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);
  
