/*-----------------------------------------------------------------------------
 NeXus - Neutron & X-ray Common Data Format
  
 NeXus Browser

 Copyright (C) 2000, Ray Osborn

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

 Contact : R. Osborn <ROsborn@anl.gov>
           Materials Science Division
           Argonne National Laboratory
           Argonne, IL 60439-4845
           USA

 For further information, see <http://www.neutron.anl.gov/NeXus/>

 $Id$
!----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "napi.h"

int NXBdir (NXhandle file_id);
int NXBopen (NXhandle file_id, NXname group_name);
int NXBread (NXhandle file_id, NXname data_name, char *dims);
int print_attr (NXhandle file_id);
void print_dims (int rank, int *dims);
void print_type (int datatype);
void print_data (void* data, int datatype, int num);
int find_group (NXhandle file_id, char *group_name, char *group_class);
int find_data (NXhandle file_id, char *data_name);

int main()
{
   NXhandle file_id;
   char file_name[80], input_text[255], path[80], *c, *command, *dims;
   NXname group_name, data_name;
   int status, group_level = 0, i;

   printf ("Give name of NeXus file : ");
   fgets (file_name, sizeof(file_name), stdin);
   if ((c = strchr(file_name, '\n')) != NULL)
	  *c = '\0';

/* Open input file and output global attributes */
   if (NXopen (file_name, NXACC_READ, &file_id) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", file_name);
      return NX_ERROR;
   }
   print_attr (file_id);
/* Input commands until the EXIT command is given */
   strcpy (path, "NX");
   do {
      printf ("%s> ", path);
      fgets (input_text, sizeof(input_text), stdin);
      if ((c = strchr(input_text, '\n')) != NULL)
	     *c = '\0';
      command = strtok(input_text," ");
      /* Check if a command has been given */
      if (command == NULL) command = " ";
      /* Command is to print a directory of the current group */
      if (!strcmp (command, "DIR") || !strcmp (command, "dir")) {
         status = NXBdir (file_id);
      }    
      /* Command is to open the specified group */
      if (!strcmp (command, "OPEN") || !strcmp (command, "open")) {
         c = strtok(NULL, " "); 
         if (c != NULL) {
            strcpy (group_name, c);
            status = NXBopen (file_id, group_name);
            /* Add the group to the prompt string */
            if (status == NX_OK) {
               strcat (path, "/");
               strcat (path, group_name);
               group_level++;
            }
         }
         else {
            printf ("NX_ERROR: Specify a group\n");
         }
      }
      /* Command is to print the values of the data
         1) if the array index is specified, a single element is output
         2) if no index is given, the minimum and maximum values of the array are output */
      if (!strcmp (command, "READ") || !strcmp (command, "read")) {
         c = strtok (NULL, " [");
         if (c != NULL) {
            strcpy (data_name, c);
            dims = strtok(NULL, "[]");
            status = NXBread (file_id, data_name, dims);
            printf ("\n");
            /* Check for attributes unless a single element is specified */
            if (status == NX_OK && dims == NULL) print_attr (file_id);
         }
         else {
            printf ("NX_ERROR: Specify a data item\n");
         }
      }
      /* Command is to close the current group */
      if (!strcmp (command, "CLOSE") || !strcmp (command, "close")) {
         if (NXclosegroup (file_id) == NX_OK) {
            /* Remove the group from the prompt string */
            c = strrchr (path, '/'); /* position of last group delimiter */
            if (c != NULL) 
               *c = '\0';            /* terminate the string there */
            group_level--;
         }
      }
      /* Command is to print help information */
      if (!strcmp (command, "HELP") || !strcmp (command, "help")) {
         printf ("NXbrowse commands : DIR\n");
         printf ("                    OPEN <group_name>\n");
         printf ("                    READ <data_name>\n");
         printf ("                    READ <data_name>[<dimension indices...>]\n");
         printf ("                    CLOSE\n");
         printf ("                    HELP\n");
         printf ("                    EXIT\n");
      }
      /* Command is to exit the program */
      if (!strcmp (command, "EXIT") || !strcmp (command, "exit") ||
          !strcmp (command, "QUIT") || !strcmp (command, "quit")) {
         for (i = group_level; i > 0; i--) NXclosegroup (file_id);
         NXclose (&file_id);
         return NX_OK;
      }
      status = NX_OK;
   } while (status == NX_OK);
   return NX_OK;
}

/* Outputs the contents of a NeXus group */
int NXBdir (NXhandle file_id)
{
   int status, NXtype, NXrank, NXdims[NX_MAXRANK];
   NXname name, class;

   if (NXinitgroupdir (file_id) != NX_OK) return NX_ERROR;
   do {
      status = NXgetnextentry (file_id, name, class, &NXtype);
      if (status == NX_ERROR) break;
      if (status == NX_OK) {
         if (!strncmp(class,"NX",2))
            printf ("  NX Group : %s (%s)\n", name, class);
         if (!strncmp(class,"SDS",3)) {
            printf ("  NX Data  : %s", name);
            if (NXopendata (file_id, name) != NX_OK) return NX_ERROR;
            if (NXgetinfo (file_id, &NXrank, NXdims, &NXtype) != NX_OK) return NX_ERROR;
            print_dims (NXrank, NXdims);
            printf (" ");
            print_type (NXtype);
            printf ("\n");
         }
      }
   } while (status != NX_EOD);
   return status;
}

/* Opens the requested group */
int NXBopen (NXhandle file_id, NXname group_name)
{
   NXname group_class;

   if (group_name == NULL) {
      printf ("NX_ERROR: Specify a group name with the OPEN command\n");
      return NX_ERROR;
   }
   if (find_group (file_id, group_name, group_class) != NX_OK) return NX_ERROR;
   if (NXopengroup (file_id, group_name, group_class) != NX_OK) return NX_ERROR;
   return NX_OK;
}

/* Output requested data */
int NXBread (NXhandle file_id, NXname data_name, char *dimstring)
{
   int NXrank, NXdims[NX_MAXRANK], NXtype, start[NX_MAXRANK], size[NX_MAXRANK], i, j;
   char dims[80], *single_dim;
   void *data_buffer;
  
   /* Check the specified data item exists */
   if (find_data (file_id, data_name) != NX_OK) return NX_ERROR;
   /* Open the data and obtain its type and rank details */
   if (NXopendata (file_id, data_name) != NX_OK) return NX_ERROR;
   if (NXgetinfo (file_id, &NXrank, NXdims, &NXtype) != NX_OK) return NX_ERROR;
   /* Check if a single element has been specified */
   /* If so, read in the indices */
   if (dimstring != NULL) {
      strcpy (dims, dimstring);      
      single_dim = strtok (dims, ",");
      for (i = 0; single_dim != NULL && i < NX_MAXRANK; i++) {
         if (i >= NXrank) {
            printf ("NX_ERROR: Data rank = %d", NXrank);
            return NX_ERROR;
         }
         sscanf (single_dim, "%d", &j);
         if (j > NXdims[i] || j < 1) {
            printf ("NX_ERROR: Data dimension %d = %d", (i+1), NXdims[i]);
            return NX_ERROR;
         }
         start[i] = j - 1;
         size[i] = 1;
         single_dim = strtok (NULL, ",");
      }
      if (i != NXrank) {
         printf ("NX_ERROR: Data rank = %d", NXrank);
         return NX_ERROR;
      }
   }
   /* Otherwise, allocate enough space for the first 3 elements of each dimension */
   else {
      for (i = 0; i < NXrank; i++) {
         if (NXdims[i] > 3 && NXtype != NX_CHAR) {
            start[i] = 0;
            size[i] = 3;
         } /* unless it's a character string */
         else {
            start[i] = 0;
            size[i] = NXdims[i];
         }
      }
   }
   if (NXmalloc((void**)&data_buffer, NXrank, size, NXtype) != NX_OK) return NX_ERROR;
   /* Read in the data with NXgetslab */
   if (NXgetslab (file_id, data_buffer, start, size) != NX_OK) return NX_ERROR;
   /* Output data according to data type */
   printf ("  %s", data_name);
   if (dimstring == NULL)
      print_dims (NXrank, NXdims);
   else 
      printf ("[%s]", dimstring);
   printf (" ");
   print_type (NXtype);
   printf (" = ");
   /* Output the data */
   if (dimstring == NULL) {  /* Print the first few values (max 3) */
      if (NXtype == NX_CHAR) /* If the data is a string, output the whole buffer */
         print_data (data_buffer, NXtype, NXdims[0]);
      else {
         if (NXrank == 1 && NXdims[0] == 1) {    /* It's a scalar */
            print_data (data_buffer, NXtype, 1);
         }
         else {                                  /* It's an array */
            printf ("[ ");
            /* Determine total size of input buffer */
            for (i = 0, j = 0; i < NXrank; i++)
               j += NXdims[i];
            /* Output at least 3 values */
            if (j > 3) {
               print_data (data_buffer, NXtype, 3);
               printf ("...");
            }
            /* unless the total size is smaller */
            else {
               print_data (data_buffer, NXtype, j);
            }
            printf ("]");
         }
      }
   }
   else                      /* Print the requested item */    
      print_data (data_buffer, NXtype, 1);
   if (NXfree((void**)&data_buffer) != NX_OK) return NX_ERROR;
   return NX_OK;
}

/* Checks for attributes and outputs their values */
int print_attr (NXhandle file_id)
{
   int status, NXlen, NXtype;
   NXname attr_name;
   void *data_buffer;

   do {
      status = NXgetnextattr (file_id, attr_name, &NXlen, &NXtype);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         NXlen++; /* Add space for string termination */
         if (NXmalloc((void**)&data_buffer, 1, &NXlen, NXtype) != NX_OK) return NX_ERROR;
         if (NXgetattr (file_id, attr_name, data_buffer, &NXlen, &NXtype) != NX_OK) return NX_ERROR;
         printf ("    %s = ", attr_name);
         print_data (data_buffer, NXtype, NXlen);
         printf ("\n");
         if (NXfree((void**)&data_buffer) != NX_OK) return NX_ERROR;
      }
   } while (status != NX_EOD);
   return NX_OK;
}

/* Outputs the specified dimensions as a formatted string */
void print_dims (int rank, int dims[])
{
   int i;

   if (rank > 1 || dims[0] != 1) {
      printf ("[");
      for (i=0; i<rank; i++) {
         if (i > 0) 
            printf (",");
         printf ("%d", dims[i]);
      }
      printf ("]");
   }
}

/* Converts the NeXus data type into a character string */
void print_type (int datatype)
{
   switch (datatype) {
      case NX_CHAR:
        printf ("(NX_CHAR)");
        break;
      case NX_FLOAT32:
        printf ("(NX_FLOAT32)");
        break;
      case NX_FLOAT64:
        printf ("(NX_FLOAT64)");
        break;
      case NX_INT8:
        printf ("(NX_INT8)");
        break;
      case NX_UINT8:
        printf ("(NX_UINT8)");
        break;
      case NX_INT16:
        printf ("(NX_INT16)");
        break;
      case NX_UINT16:
        printf ("(NX_UINT16)");
        break;
      case NX_INT32:
        printf ("(NX_INT32)");
        break;
      case NX_UINT32:
        printf ("(NX_UINT32)");
        break;
      default:
        printf ("(UNKNOWN)");
        break;
   }
}

/* Outputs data items with the requested type */
void print_data (void *data, int type, int num)
{
   int i;

   for (i=0; i<num; i++) {
      switch(type) {
         case NX_CHAR:
            printf ("%c", ((char *)data)[i]);
            break;
         case NX_INT8:
            printf ("%d ", ((char *)data)[i]);
            break;
         case NX_UINT8:
            printf ("%d ", ((unsigned char *)data)[i]);
            break;
         case NX_INT16:
            printf ("%d ", ((short *)data)[i]);
            break;
         case NX_UINT16:
            printf ("%d ", ((unsigned short *)data)[i]);
            break;
         case NX_INT32:
            printf ("%d ", ((long *)data)[i]);
            break;
         case NX_UINT32:
            printf ("%d ", ((unsigned long *)data)[i]);
            break;
         case NX_FLOAT32:
            printf ("%f ", ((float *)data)[i]);
            break;
         case NX_FLOAT64:
            printf ("%f ", ((double *)data)[i]);
            break;
         default:
            printf("print_data: invalid type");
            break;
      }
   }
}

/* Searches group for requested group and return its class */
int find_group (NXhandle file_id, char *group_name, char *group_class)
{
   int status, NXtype;
   NXname name, class;

   NXinitgroupdir (file_id);
   do {
      status = NXgetnextentry (file_id, name, class, &NXtype);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (!strcmp(group_name,name)) {
            strcpy (group_class, class);
            if (!strncmp(group_class,"NX",2)) {
               return NX_OK;
            }
            else {
               printf ("NX_ERROR: %s is not a group\n", group_name);
               return NX_ERROR;
            }
         }
      }
   } while (status != NX_EOD);
   printf ("NX_ERROR: %s does not exist\n", group_name);
   return NX_EOD;
}

/* Searches group for the requested data item */
int find_data (NXhandle file_id, char *data_name)
{
   int status, NXtype;
   NXname name, class;

   NXinitgroupdir (file_id);
   do {
      status = NXgetnextentry (file_id, name, class, &NXtype);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (!strcmp(data_name,name)) {
            if (!strncmp(class,"SDS",3)) { /* Data has class "SDS" */
               return NX_OK;
            }
            else {
               printf ("NX_ERROR: %s is not data\n", data_name);
               return NX_ERROR;
            }
         }
      }
   } while (status != NX_EOD);
   printf ("NX_ERROR: %s does not exist\n", data_name);
   return NX_EOD;
}
