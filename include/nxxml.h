/*
 * This is the header file for the NeXus XML file driver. 
 *
 *   Copyright (C) 2004 Mark Koennecke
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  For further information, see <http://www.neutron.anl.gov/NeXus/>
 */
#ifndef NEXUSXML
#define NEXUSXML

NX_EXTERNAL  NXstatus CALLING_STYLE NXXopen(CONSTCHAR *filename, NXaccess access_method, NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXclose(NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXflush(NXhandle* pHandle);

NXstatus CALLING_STYLE NXXmakegroup (NXhandle fid, CONSTCHAR *name, 
				     char *nxclass);
NXstatus CALLING_STYLE NXXopengroup (NXhandle fid, CONSTCHAR *name, 
				     char *nxclass);
NXstatus CALLING_STYLE NXXclosegroup (NXhandle fid);

NXstatus CALLING_STYLE NXXcompmakedata (NXhandle fid, CONSTCHAR *name, 
					int datatype, 
					int rank, 
					int dimensions[],
					int compress_type, int chunk_size[]);
NXstatus CALLING_STYLE NXXmakedata (NXhandle fid, 
				    CONSTCHAR *name, int datatype, 
				    int rank, int dimensions[]);
NXstatus CALLING_STYLE NXXopendata (NXhandle fid, CONSTCHAR *name);
NXstatus CALLING_STYLE NXXclosedata (NXhandle fid);
NXstatus CALLING_STYLE NXXputdata (NXhandle fid, void *data);
NXstatus CALLING_STYLE NXXgetdata (NXhandle fid, void *data);
NXstatus CALLING_STYLE NXXgetinfo (NXhandle fid, int *rank, 
				   int dimension[], int *iType);
NXstatus CALLING_STYLE NXXputslab (NXhandle fid, void *data, 
				   int iStart[], int iSize[]);
NXstatus CALLING_STYLE NXXgetslab (NXhandle fid, void *data, 
				   int iStart[], int iSize[]);
NXstatus CALLING_STYLE NXXputattr (NXhandle fid, CONSTCHAR *name, void *data, 
				   int datalen, int iType);
NXstatus CALLING_STYLE NXXgetattr (NXhandle fid, char *name, 
				   void *data, int* datalen, int* iType);

NXstatus CALLING_STYLE NXXgetnextentry (NXhandle fid,NXname name, 
					NXname nxclass, int *datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXgetnextattr(NXhandle handle, 
				NXname pName, int *iLength, int *iType);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXinitgroupdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXinitattrdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXgetattrinfo (NXhandle fid, int *iN);
NX_EXTERNAL  NXstatus CALLING_STYLE NXXgetgroupinfo (NXhandle fid, int *iN, 
					NXname pName, NXname pClass);

NXstatus CALLING_STYLE NXXgetdataID (NXhandle fid, NXlink* sRes);
NXstatus CALLING_STYLE NXXgetgroupID (NXhandle fid, NXlink* sRes);
NXstatus CALLING_STYLE NXXmakelink (NXhandle fid, NXlink* sLink);
NXstatus CALLING_STYLE NXXsameID (NXhandle fileid, NXlink* pFirstID, 
				  NXlink* pSecondID);
#endif
