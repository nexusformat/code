#include <stdio.h>
#include "napi.h"

static void print_data (const char *prefix, void *data, int type, int num);

int main ()
{
  int i, j, u, NXrank, NXdims[32], NXtype, NXlen, entry_status, attr_status;
  double r;
  void *data_buffer;
  unsigned char i1_array[4] = {1, 2, 3, 4};
  short int i2_array[4] = {1000, 2000, 3000, 4000};
  int i4_array[4] = {1000000, 2000000, 3000000, 4000000};
  float r4_array[5][4] =
  {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20.};
  double r8_array[5][4] =
  {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20.};
  int array_dims[2] = {5, 4};
  int unlimited_dims[1] = {NX_UNLIMITED};
  int chunk_size[2]={5,4};
  int slab_start[2], slab_size[2];
  char name[64], char_class[64], char_buffer[128];
  char group_name[64], class_name[64];
  NXhandle fileid;
  NXlink glink, dlink;
  int comp_array[1000][20];
  int dims[2];
  int cdims[2];

/* create file */
  if (NXopen ("NXtest.h5", NXACC_CREATE5, &fileid) != NX_OK) return 1;
  if (NXmakegroup (fileid, "entry", "NXentry") != NX_OK) return 1;
  if (NXopengroup (fileid, "entry", "NXentry") != NX_OK) return 1;
  if (NXmakegroup (fileid, "det", "NXdetector") != NX_OK) return 1;
  if (NXopengroup (fileid, "det", "NXdetector") != NX_OK) return 1;
  if (NXmakegroup (fileid, "da", "NXdata") != NX_OK) return 1;
  if (NXopengroup (fileid, "da", "NXdata") != NX_OK) return 1;
  NXgetgroupID (fileid, &glink);
     NXlen = 10;
     if (NXmakedata (fileid, "ch_data", NX_CHAR, 1, &NXlen) != NX_OK) return 1;
     if (NXopendata (fileid, "ch_data") != NX_OK) return 1;
        if (NXputdata (fileid, "NeXus data") != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     if (NXmakedata (fileid, "i1_data", NX_INT8, 1, &array_dims[1]) != NX_OK) return 1;
     if (NXopendata (fileid, "i1_data") != NX_OK) return 1;
        if (NXputdata (fileid, i1_array) != NX_OK) return 1;
        if (NXgetdataID (fileid, &dlink) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     if (NXmakedata (fileid, "i2_data", NX_INT16, 1, &array_dims[1]) != NX_OK) return 1;
     if (NXopendata (fileid, "i2_data") != NX_OK) return 1;
        if (NXputdata (fileid, i2_array) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     dims[0] = 1000;
     dims[1] = 20;
     for(i = 0; i < 1000; i++)
         {
         for(j = 0; j < 20; j++)
            {
              comp_array[i][j] = 1;
            }
         }
     cdims[0] = 20;
     cdims[1] = 20;
     /*
     if (NXcompmakedata (fileid, "comp_data", NX_INT32, 2, dims, NX_COMP_LZW, cdims) != NX_OK) return 1;
     if (NXopendata (fileid, "comp_data") != NX_OK) return 1;
        if (NXputdata (fileid, comp_array) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     */
     if (NXmakedata (fileid, "i4_data", NX_INT32, 1, &array_dims[1]) != NX_OK) return 1;
     if (NXopendata (fileid, "i4_data") != NX_OK) return 1;
        if (NXputdata (fileid, i4_array) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     if (NXcompmakedata (fileid, "r4_data", NX_FLOAT32, 2, array_dims,NX_COMP_LZW,chunk_size) != NX_OK) return 1;
     if (NXopendata (fileid, "r4_data") != NX_OK) return 1;
        if (NXputdata (fileid, r4_array) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     if (NXmakedata (fileid, "r8_data", NX_FLOAT64, 2, array_dims) != NX_OK) return 1;
     if (NXopendata (fileid, "r8_data") != NX_OK) return 1;
        slab_start[0] = 4; slab_start[1] = 0; slab_size[0] = 1; slab_size[1] = 4;
        if (NXputslab (fileid, *r8_array + 16, slab_start, slab_size) != NX_OK) return 1;
        slab_start[0] = 0; slab_start[1] = 0; slab_size[0] = 4; slab_size[1] = 4;
        if (NXputslab (fileid, r8_array, slab_start, slab_size) != NX_OK) return 1;
        if (NXputattr (fileid, "ch_attribute", "NeXus", strlen ("NeXus"), NX_CHAR) != NX_OK) return 1;
        i = 42;
        if (NXputattr (fileid, "i4_attribute", &i, 1, NX_INT32) != NX_OK) return 1;
        r = 3.14159265;
        if (NXputattr (fileid, "r4_attribute", &r, 1, NX_FLOAT64) != NX_OK) return 1;
        if (NXputattr (fileid, "r4_attribute", &r, 1, NX_FLOAT64) != NX_OK) return 1;
     if (NXclosedata (fileid) != NX_OK) return 1;
     /* subgroup detector */
     if (NXmakegroup (fileid, "zdetector", "NXdetector") != NX_OK) return 1;
     if (NXopengroup (fileid, "zdetector", "NXdetector") != NX_OK) return 1;
     if (NXclosegroup (fileid) != NX_OK) return 1;
  /* close NXentry group */
  if (NXclosegroup (fileid) != NX_OK) return 1;
  if (NXclosegroup (fileid) != NX_OK) return 1;
  if (NXclosegroup (fileid) != NX_OK) return 1;

  /* test data linking  */
  if (NXmakegroup (fileid, "data", "NXdata") != NX_OK) return 1;
  if (NXopengroup (fileid, "data", "NXdata") != NX_OK) return 1;
     NXmakelink (fileid, &dlink);
  if (NXclosegroup (fileid) != NX_OK) return 1;
  /* test group linking  */ 
  if (NXmakegroup (fileid, "data1", "NXdata") != NX_OK) return 1;
  if (NXopengroup (fileid, "data1", "NXdata") != NX_OK) return 1;
     NXmakelink (fileid, &glink);
  if (NXclosegroup (fileid) != NX_OK) return 1;

  /* test flush and unlimited dimensions */
  if (NXflush (&fileid) != NX_OK) return 1;
  if (NXopengroup (fileid, "entry", "NXentry") != NX_OK) return 1;
  if (NXmakedata (fileid, "r9_data", NX_INT32, 1, unlimited_dims) != NX_OK) return 1;
  slab_size[0] = 1;
  for (i = 0; i < 7; i++)
    {
      slab_start[0] = i;
      if (NXopendata (fileid, "r9_data") != NX_OK) return 1;
      if (NXputslab (fileid, &i, slab_start, slab_size) != NX_OK) return 1;
      if (NXflush (&fileid) != NX_OK) return 1;
    }

  if (NXclose (&fileid) != NX_OK) return 1;
  /* finish writing */

   
  /* start read data */


  if (NXopen ("NXtest.h5", NXACC_RDWR, &fileid) != NX_OK) return 1;
  do
    {
    attr_status = NXgetnextattr (fileid, name, NXdims, &NXtype);
    if (attr_status == NX_ERROR) return 1;
    if (attr_status == NX_OK)
      {
       switch (NXtype)
         {
         case NX_CHAR:
           NXlen = sizeof (char_buffer);
           if (NXgetattr (fileid, name, char_buffer, &NXlen, &NXtype) != NX_OK) return 1;
           printf ("%s : %s\n", name, char_buffer);
           break;
         }
      }
    }
  while (attr_status == NX_OK);
  if (NXopengroup (fileid, "entry", "NXentry") != NX_OK) return 1;
  if (NXopengroup (fileid, "det", "NXdetector") != NX_OK) return 1;
  if (NXopengroup (fileid, "da", "NXdata") != NX_OK) return 1;
  do
    {
    entry_status = NXgetnextentry (fileid, name, char_class, &NXtype);
    if (entry_status == NX_ERROR) return 1;
    if (strcmp(char_class,"SDS") != 0) {
       /*subgroup */
       if (entry_status != NX_EOD) { 
          entry_status = NX_OK;
       }
    } else {
       if (entry_status == NX_OK) {
          if (NXopendata (fileid, name) != NX_OK) return 1;
          if (NXgetinfo (fileid, &NXrank, NXdims, &NXtype) != NX_OK) return 1;
             printf ("%s : %d\n", name, NXtype);
          if (NXmalloc ((void **) &data_buffer, NXrank, NXdims, NXtype) != NX_OK) return 1;
          if (NXtype == NX_CHAR) {
             if (NXgetdata (fileid, data_buffer) != NX_OK) return 1;
             print_data ("Values : ", data_buffer, NXtype, 10);
          } else if (NXtype != NX_FLOAT32 && NXtype != NX_FLOAT64) {
             if (NXgetdata (fileid, data_buffer) != NX_OK) return 1;
             print_data ("Values : ", data_buffer, NXtype, 4);
          } else {
             slab_start[0] = 0;
             slab_start[1] = 0;
             slab_size[0] = 1;
             slab_size[1] = 4;
             if (NXgetslab (fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
                print_data ("Values : ", data_buffer, NXtype, 4);
             slab_start[0] = 1;
             if (NXgetslab (fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
                print_data ("       : ", data_buffer, NXtype, 4);
             slab_start[0] = 2;
             if (NXgetslab (fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
                print_data ("       : ", data_buffer, NXtype, 4);
             slab_start[0] = 3;
             if (NXgetslab (fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
                print_data ("       : ", data_buffer, NXtype, 4);
             slab_start[0] = 4;
             if (NXgetslab (fileid, data_buffer, slab_start, slab_size) != NX_OK) return 1;
                print_data ("       : ", data_buffer, NXtype, 4);
             do
               {
               attr_status = NXgetnextattr (fileid, name, NXdims, &NXtype);
               if (attr_status == NX_ERROR) return 1;
               if (attr_status == NX_OK) {
                  switch (NXtype)
                    {
                    case NX_INT32:
                       NXlen = 1;
                       if (NXgetattr (fileid, name, &i, &NXlen, &NXtype) != NX_OK) return 1;
                          printf ("%s : %d\n", name, i);
                       break;
                    case NX_FLOAT32:
                       NXlen = 1;
                       if (NXgetattr (fileid, name, &r, &NXlen, &NXtype) != NX_OK) return 1;
                          printf ("%s : %f\n", name, r);
                       break;
                    case NX_CHAR:
                       NXlen = sizeof (char_buffer);
                       if (NXgetattr (fileid, name, char_buffer, &NXlen, &NXtype) != NX_OK) return 1;
                          printf ("%s : %s\n", name, char_buffer);
                       break;
                    }
                 }
              } while (attr_status == NX_OK);
           }
           if (NXclosedata (fileid) != NX_OK) return 1;
           if (NXfree ((void **) &data_buffer) != NX_OK) return 1;
         }
      }
  } while (entry_status == NX_OK);
  if (NXopendata (fileid, "r8_data") != NX_OK) return 1;
  if (NXgetattrinfo (fileid, &i) != NX_OK) return 1;
     printf ("Number of attributes : %d\n", i);
  if (NXclosedata (fileid) != NX_OK) return 1;
  if (NXopendata (fileid, "r4_data") != NX_OK) return 1;
  if (NXgetattrinfo (fileid, &i) != NX_OK) return 1;
     printf ("Number of attributes : %d\n", i);
  if (NXclosedata (fileid) != NX_OK) return 1;
  if (NXgetgroupinfo (fileid, &u, group_name, class_name) != NX_OK) return 1;
     printf ("Group:'%s' with class name: %s and %d item(s)\n", group_name, class_name, u);
  if (NXclosegroup (fileid) != NX_OK) return 1;
  if (NXclosegroup (fileid) != NX_OK) return 1;
  if (NXclosegroup (fileid) != NX_OK) return 1;
  NXlen = sizeof (char_buffer);
  NXtype = NX_CHAR;
  if (NXgetattr (fileid, "HDF5_Version", char_buffer, &NXlen, &NXtype) != NX_OK) return 1;
      printf ("HDF5_Version: %s Length:(%d)\n", char_buffer, NXlen);
  if (NXgetattrinfo (fileid, &i) != NX_OK) return 1;
     printf ("Number of attributes : %d\n", i);
  if (NXclose (&fileid) != NX_OK) return 1;

  return 0;
}

static void
print_data (const char *prefix, void *data, int type, int num)
{
  int i;
  printf ("%s", prefix);
  for (i = 0; i < num; i++)
    {
      switch (type)
        {
        case NX_CHAR:
          printf ("%c", ((char *) data)[i]);
          break;

        case NX_INT8:
          printf ("%d ", ((unsigned char *) data)[i]);
          break;

        case NX_INT16:
          printf ("%d ", ((short *) data)[i]);
          break;

        case NX_INT32:
          printf ("%d ", ((int *) data)[i]);
          break;

        case NX_FLOAT32:
          printf ("%f ", ((float *) data)[i]);
          break;

        case NX_FLOAT64:
          printf ("%f ", ((double *) data)[i]);
          break;

        default:
          printf ("print_data: invalid type");
          break;
        }
    }
  printf ("\n");
}
