/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Test program for NeXus C interface
  
  Copyright (C) 1997, 1998, 1999 Freddie Akeroyd
  
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
 
  Contact : Freddie Akeroyd <F.A.Akeroyd@rl.ac.uk>
            ISIS Facility
            Rutherford Appleton Laboratory
            Chilton, Didcot
            Oxfordshire
            OX11 0QX
            United Kingdom
            
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
   unsigned char i1_array[4] = { 1, 2, 3, 4 };
   short int i2_array[4] = { 1000, 2000, 3000, 4000 };
   int i4_array[4] = { 1000000, 2000000, 3000000, 4000000 };
   float r4_array[5][4] = 
   {1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.};
   double r8_array[5][4] = 
   {1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.};
   int array_dims[2] = { 5, 4 };
   int slab_start[2], slab_size[2];
   char name[64], class[64], char_buffer[128];
   NXhandle fileid;

/* create file */
   if (NXopen("NXtest.nxs", NXACC_CREATE, &fileid) != NX_OK) return 1;
   if (NXmakegroup(fileid, "entry", "NXentry") != NX_OK) return 1;
   if (NXopengroup(fileid, "entry", "NXentry") != NX_OK) return 1;
      NXlen = 10;
      if (NXmakedata (fileid, "ch_data", NX_CHAR, 1, &NXlen) != NX_OK) return 1;
      if (NXopendata (fileid, "ch_data") != NX_OK) return 1;
         if (NXputdata(fileid, "NeXus data") != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXmakedata (fileid, "i1_data", NX_INT8, 1, &array_dims[1]) != NX_OK) return 1;
      if (NXopendata (fileid, "i1_data") != NX_OK) return 1;
         if (NXputdata(fileid, i1_array) != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXmakedata (fileid, "i2_data", NX_INT16, 1, &array_dims[1]) != NX_OK) return 1;
      if (NXopendata (fileid, "i2_data") != NX_OK) return 1;
         if (NXputdata(fileid, i2_array) != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXmakedata (fileid, "i4_data", NX_INT32, 1, &array_dims[1]) != NX_OK) return 1;
      if (NXopendata (fileid, "i4_data") != NX_OK) return 1;
         if (NXputdata(fileid, i4_array) != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXmakedata (fileid, "r4_data", NX_FLOAT32, 2, array_dims) != NX_OK) return 1;
      if (NXopendata (fileid, "r4_data") != NX_OK) return 1;
         if (NXputdata(fileid, r4_array) != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
      if (NXmakedata (fileid, "r8_data", NX_FLOAT64, 2, array_dims) != NX_OK) return 1;
      if (NXopendata (fileid, "r8_data") != NX_OK) return 1;
         slab_start[0]=4;slab_start[1]=0;slab_size[0]=1;slab_size[1]=4;
         if (NXputslab(fileid, *r8_array+16, slab_start, slab_size) != NX_OK) return 1;
         slab_start[0]=0;slab_start[1]=0;slab_size[0]=4;slab_size[1]=4;
         if (NXputslab(fileid, r8_array, slab_start, slab_size) != NX_OK) return 1;
         if (NXputattr(fileid, "ch_attribute", "NeXus", strlen("NeXus"), NX_CHAR) != NX_OK) return 1;
         i = 42;
         if (NXputattr(fileid, "i4_attribute", &i, 1, NX_INT32) != NX_OK) return 1;
         r = 3.14159265;
         if (NXputattr(fileid, "r4_attribute", &r, 1, NX_FLOAT32) != NX_OK) return 1;
      if (NXclosedata(fileid) != NX_OK) return 1;
   if (NXclosegroup(fileid) != NX_OK) return 1;
   if (NXclose(&fileid) != NX_OK) return 1;
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
