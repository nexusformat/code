/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Test program for reading NXtest.nxs files written by C or Fortran API
  
  Copyright (C) 1999 Ray Osborn
  
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
 
  Contact : Ray Osborn <ROsborn@anl.gov>
            Materials Science Division
            Argonne National Laboratory
            Argonne, IL 60439-4845
            USA
            
  For further information, see <http://www.neutron.anl.gov/NeXus/>

  $Id$

----------------------------------------------------------------------------*/

#include <stdio.h> 
#include "napi.h"

static void print_data(const char* prefix, void* data, int type, int num);

int main()
{
   int i, NXrank, NXdims[32], NXtype, NXlen, entry_status, attr_status;
   float r;
   void *data_buffer;
   int slab_start[2], slab_size[2];
   char name[64], class[64], char_buffer[128];
   NXhandle fileid;

/* read data */
   if (NXopen("NXtest.nxs", NXACC_READ, &fileid) != NX_OK) return 1;
   if (NXopengroup(fileid, "entry", "NXentry") != NX_OK) return 1;
   do 
   {
      entry_status = NXgetnextentry(fileid, name, class, &NXtype);
      if (entry_status == NX_ERROR) return 1;
      if (entry_status == NX_OK) 
      {
      if (NXopendata (fileid, name) != NX_OK) return 1;
         if (NXgetinfo(fileid, &NXrank, NXdims, &NXtype) != NX_OK) return 1;
         printf("%s : %d\n", name, NXtype);
         if (NXmalloc((void**)&data_buffer, NXrank, NXdims, NXtype) != NX_OK) return 1;
         if (NXtype == NX_CHAR)
         {
            if (NXgetdata(fileid, data_buffer) != NX_OK) return 1;
            print_data("Values : ", data_buffer, NXtype, 10);
         }  
         else if (NXtype != NX_FLOAT32 && NXtype != NX_FLOAT64)
         {
            if (NXgetdata(fileid, data_buffer) != NX_OK) return 1;
            print_data("Values : ", data_buffer, NXtype, 4);
         }
         else
         {
            slab_start[0]=0;slab_start[1]=0;slab_size[0]=1;slab_size[1]=4;
            if (NXgetslab(fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
            print_data("Values : ", data_buffer, NXtype, 4);
            slab_start[0]=1;
            if (NXgetslab(fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
            print_data("       : ", data_buffer, NXtype, 4);
            slab_start[0]=2;
            if (NXgetslab(fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
            print_data("       : ", data_buffer, NXtype, 4);
            slab_start[0]=3;
            if (NXgetslab(fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
            print_data("       : ", data_buffer, NXtype, 4);
            slab_start[0]=4;
            if (NXgetslab(fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
            print_data("       : ", data_buffer, NXtype, 4);
            do
            {
               attr_status = NXgetnextattr(fileid, name, NXdims, &NXtype);
               if (attr_status == NX_ERROR) return 1;
               if (attr_status == NX_OK)
               {
                  switch (NXtype)
                  {
                  case NX_INT32:
                     NXlen = 1;
                     if (NXgetattr(fileid, name, &i, &NXlen, &NXtype) != NX_OK) return 1;
                     printf("%s : %d\n", name, i); break;
                  case NX_FLOAT32:
                     NXlen = 1;
                     if (NXgetattr(fileid, name, &r, &NXlen, &NXtype) != NX_OK) return 1;
                     printf("%s : %f\n", name, r); break;
                  case NX_CHAR:
                     NXlen = sizeof(char_buffer);
                     if (NXgetattr(fileid, name, char_buffer, &NXlen, &NXtype) != NX_OK) return 1;
                     printf("%s : %s\n", name, char_buffer); break;
                  }
               }
            } while (attr_status == NX_OK);
         }
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXfree((void**)&data_buffer) != NX_OK) return 1;
      }
   } while (entry_status == NX_OK);
   if (NXclosegroup(fileid) != NX_OK) return 1;
   if (NXclose(&fileid) != NX_OK) return 1;
   return 0;
}

static void print_data(const char* prefix, void* data, int type, int num)
{
    int i;
    printf ("%s", prefix);
    for(i=0; i<num; i++)
    {
       switch(type)
       {
          case NX_CHAR:
             printf ("%c", ((char*)data)[i]);
             break;

          case NX_INT8:
             printf ("%d ", ((unsigned char*)data)[i]);
             break;

          case NX_INT16:
             printf ("%d ", ((short*)data)[i]);
             break;

          case NX_INT32:
             printf ("%d ", ((int*)data)[i]);
             break;

          case NX_FLOAT32:
             printf ("%f ", ((float*)data)[i]);
             break;

          case NX_FLOAT64:
             printf ("%f ", ((double*)data)[i]);
             break;

          default:
             printf("print_data: invalid type");
             break;
       }
    }
    printf ("\n");
}
