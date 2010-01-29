/**********************************************************************
 *          cif2nx -- convert a cif or cbf to a NeXus file            *
 *                                                                    *
 * Part of CBFlib Version 0.7.8 20 September 2007                     *
 * Rev 25 Jan 2010, HJB -- drop CIF down below NXcif group            *
 *                                                                    *
 *                          Paul Ellis and                            *
 *         Herbert J. Bernstein (yaya@bernstein-plus-sons.com)        *
 *                                                                    *
 * (C) Copyright 2006, 2007 Herbert J. Bernstein                      *
 *                                                                    *
 **********************************************************************/

/**********************************************************************
 *                                                                    *
 * YOU MAY REDISTRIBUTE THE CBFLIB PACKAGE UNDER THE TERMS OF THE GPL *
 * WHILE YOU MAY ALTERNATIVE DISTRIBUTE THE API UNDER THE LGPL        *
 * YOU MAY ***NOT*** DISTRBUTE THIS PROGRAM UNDER THE LGPL            *
 *                                                                    *                                                                    *
 **********************************************************************/

/*************************** GPL NOTICES ******************************
 *                                                                    *
 * This program is free software; you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation; either version 2 of     *
 * (the License, or (at your option) any later version.               *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program; if not, write to the Free Software        *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           *
 * 02111-1307  USA                                                    *
 *                                                                    *
 **********************************************************************/

/**********************************************************************
 *                                SYNOPSIS                            *
 *                                                                    *
 *  cif2nx [-i input_cif] [-o output_nx] \                            *
 *    [-e [4|5|x]] [-c [n|g|h|r]]        \                            *
 *    [-v dictionary]*                   \                            *
 *    [input_cif] [output_nx]                                         *
 *                                                                    *
 *  the options are:                                                  *
 *                                                                    *
 *  -i input_cif (default: stdin)                                     *
 *    the input  file in CIF or CBF  format.  If input_cif is not     *
 *    specified or is given as "-", it is copied from stdin to a      *
 *    temporary file.                                                 *
 *                                                                    *
 *  -o output_nx                                                      *
 *    the output NeXus file                                           *
 *                                                                    *
 *  -e [4|5|x] (default x)                                            *
 *    output_nx NeXus encoding to use: 4 for hdf4, 5 for hdf5,        *
 *              x for XML                                             *
 *                                                                    *
 *  -c [n|g|h|r] (default n)                                          *
 *    output dataset compression to use for datasets over 1K          *
 *    n for none, g for gzip, h for Huffman, r for run length         *
 *    encoding                                                        *
 *                                                                    *
 *  -v dictionary specifies a dictionary to be used to validate       *
 *    the input cif and to apply aliases to the output cif.           *
 *    This option may be specified multiple times, with dictionaries  *
 *    layered in the order given.                                     *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

/**********************************************************************
 *                                CREDITS                             *
 *                                                                    *
 *  This program is a Crystallographic Information File (CIF)         *
 *  application.  Please see the IUCR Policy below.   See the IUCR    *
 *  web page (http://www.iucr.org) or its mirrors for background      *
 *  and references on CIF.                                            *
 *                                                                    *
 *  This program is a Crystallographic Binary File (CBF) application. *
 *  Please see the ImgCIF/CBF web page at                             *
 *                                                                    *
 *            http://ndbserver.rutgers.edu/mmcif/cbf                  *
 *                                                                    *
 *  for background and references.  The CBF definition is available   *
 *  on the web page created by Andy Hammersley at                     *
 *                                                                    *
 *     http://www.ersf.fr/computing/Forum/imgCIF/cbf_definition.html  *
 *                                                                    *
 *  This program is a CBFlib application.  See "CBFLIB, An ANSI-C     *
 *  API for Crystallographic Binary Files", Version 0.1, April 1998   *
 *  by Paul J. Ellis, Stanford Synchrotron Radiation Laboratory,      *
 *  ellis@ssrl.slac.stanford.edu                                      *
 *                                                                    *
 *  This program uses routines derived from mpack/munpack version     *
 *  1.5, ftp://ftp.andrew.cmu.edu/pub/mpack by John G. Myers,         *
 *  jgm+@cmu.edu.  "Mpack and munpack are utilties for encoding and   *
 *  decoding ... binary files in MIME ... format."  Please see the    *
 *  copyright notices and disclaimers in the mpack/munpack routines   *
 *                                                                    *
 *  This program uses routines derived from the "RSA Data Security,   *
 *  Inc. MD5 Message-Digest Algorithm."  Please see the copyright     *
 *  notice and disclaimer in md5c.c                                   *
 **********************************************************************/


/**********************************************************************
 *                                                                    *
 *                    Stanford University Notices                     *
 *  for the CBFlib software package that incorporates SLAC software   *
 *                 on which copyright is disclaimed                   *
 *                                                                    *
 * This software                                                      *
 * -------------                                                      *
 * The term 'this software', as used in these Notices, refers to      *
 * those portions of the software package CBFlib that were created by *
 * employees of the Stanford Linear Accelerator Center, Stanford      *
 * University.                                                        *
 *                                                                    *
 * Stanford disclaimer of copyright                                   *
 * --------------------------------                                   *
 * Stanford University, owner of the copyright, hereby disclaims its  *
 * copyright and all other rights in this software.  Hence, anyone    *
 * may freely use it for any purpose without restriction.             *
 *                                                                    *
 * Acknowledgement of sponsorship                                     *
 * ------------------------------                                     *
 * This software was produced by the Stanford Linear Accelerator      *
 * Center, Stanford University, under Contract DE-AC03-76SFO0515 with *
 * the Department of Energy.                                          *
 *                                                                    *
 * Government disclaimer of liability                                 *
 * ----------------------------------                                 *
 * Neither the United States nor the United States Department of      *
 * Energy, nor any of their employees, makes any warranty, express or *
 * implied, or assumes any legal liability or responsibility for the  *
 * accuracy, completeness, or usefulness of any data, apparatus,      *
 * product, or process disclosed, or represents that its use would    *
 * not infringe privately owned rights.                               *
 *                                                                    *
 * Stanford disclaimer of liability                                   *
 * --------------------------------                                   *
 * Stanford University makes no representations or warranties,        *
 * express or implied, nor assumes any liability for the use of this  *
 * software.                                                          *
 *                                                                    *
 * Maintenance of notices                                             *
 * ----------------------                                             *
 * In the interest of clarity regarding the origin and status of this *
 * software, this and all the preceding Stanford University notices   *
 * are to remain affixed to any copy or derivative of this software   *
 * made or distributed by the recipient and are to be affixed to any  *
 * copy of software made or distributed by the recipient that         *
 * contains a copy or derivative of this software.                    *
 *                                                                    *
 * Based on SLAC Software Notices, Set 4                              *
 * OTT.002a, 2004 FEB 03                                              *
 **********************************************************************/


/**********************************************************************
 *                                 NOTICE                             *
 * Creative endeavors depend on the lively exchange of ideas. There   *
 * are laws and customs which establish rights and responsibilities   *
 * for authors and the users of what authors create.  This notice     *
 * is not intended to prevent you from using the software and         *
 * documents in this package, but to ensure that there are no         *
 * misunderstandings about terms and conditions of such use.          *
 *                                                                    *
 * Please read the following notice carefully.  If you do not         *
 * understand any portion of this notice, please seek appropriate     *
 * professional legal advice before making use of the software and    *
 * documents included in this software package.  In addition to       *
 * whatever other steps you may be obliged to take to respect the     *
 * intellectual property rights of the various parties involved, if   *
 * you do make use of the software and documents in this package,     *
 * please give credit where credit is due by citing this package,     *
 * its authors and the URL or other source from which you obtained    *
 * it, or equivalent primary references in the literature with the    *
 * same authors.                                                      *
 *                                                                    *
 * Some of the software and documents included within this software   *
 * package are the intellectual property of various parties, and      *
 * placement in this package does not in any way imply that any       *
 * such rights have in any way been waived or diminished.             *
 *                                                                    *
 * With respect to any software or documents for which a copyright    *
 * exists, ALL RIGHTS ARE RESERVED TO THE OWNERS OF SUCH COPYRIGHT.   *
 *                                                                    *
 * Even though the authors of the various documents and software      *
 * found here have made a good faith effort to ensure that the        *
 * documents are correct and that the software performs according     *
 * to its documentation, and we would greatly appreciate hearing of   *
 * any problems you may encounter, the programs and documents any     *
 * files created by the programs are provided **AS IS** without any   *
 * warranty as to correctness, merchantability or fitness for any     *
 * particular or general use.                                         *
 *                                                                    *
 * THE RESPONSIBILITY FOR ANY ADVERSE CONSEQUENCES FROM THE USE OF    *
 * PROGRAMS OR DOCUMENTS OR ANY FILE OR FILES CREATED BY USE OF THE   *
 * PROGRAMS OR DOCUMENTS LIES SOLELY WITH THE USERS OF THE PROGRAMS   *
 * OR DOCUMENTS OR FILE OR FILES AND NOT WITH AUTHORS OF THE          *
 * PROGRAMS OR DOCUMENTS.                                             *
 **********************************************************************/

/**********************************************************************
 *                                                                    *
 *                           The IUCr Policy                          *
 *      for the Protection and the Promotion of the STAR File and     *
 *     CIF Standards for Exchanging and Archiving Electronic Data     *
 *                                                                    *
 * Overview                                                           *
 *                                                                    *
 * The Crystallographic Information File (CIF)[1] is a standard for   *
 * information interchange promulgated by the International Union of  *
 * Crystallography (IUCr). CIF (Hall, Allen & Brown, 1991) is the     *
 * recommended method for submitting publications to Acta             *
 * Crystallographica Section C and reports of crystal structure       *
 * determinations to other sections of Acta Crystallographica         *
 * and many other journals. The syntax of a CIF is a subset of the    *
 * more general STAR File[2] format. The CIF and STAR File approaches *
 * are used increasingly in the structural sciences for data exchange *
 * and archiving, and are having a significant influence on these     *
 * activities in other fields.                                        *
 *                                                                    *
 * Statement of intent                                                *
 *                                                                    *
 * The IUCr's interest in the STAR File is as a general data          *
 * interchange standard for science, and its interest in the CIF,     *
 * a conformant derivative of the STAR File, is as a concise data     *
 * exchange and archival standard for crystallography and structural  *
 * science.                                                           *
 *                                                                    *
 * Protection of the standards                                        *
 *                                                                    *
 * To protect the STAR File and the CIF as standards for              *
 * interchanging and archiving electronic data, the IUCr, on behalf   *
 * of the scientific community,                                       *
 *                                                                    *
 * * holds the copyrights on the standards themselves,                *
 *                                                                    *
 * * owns the associated trademarks and service marks, and            *
 *                                                                    *
 * * holds a patent on the STAR File.                                 *
 *                                                                    *
 * These intellectual property rights relate solely to the            *
 * interchange formats, not to the data contained therein, nor to     *
 * the software used in the generation, access or manipulation of     *
 * the data.                                                          *
 *                                                                    *
 * Promotion of the standards                                         *
 *                                                                    *
 * The sole requirement that the IUCr, in its protective role,        *
 * imposes on software purporting to process STAR File or CIF data    *
 * is that the following conditions be met prior to sale or           *
 * distribution.                                                      *
 *                                                                    *
 * * Software claiming to read files written to either the STAR       *
 * File or the CIF standard must be able to extract the pertinent     *
 * data from a file conformant to the STAR File syntax, or the CIF    *
 * syntax, respectively.                                              *
 *                                                                    *
 * * Software claiming to write files in either the STAR File, or     *
 * the CIF, standard must produce files that are conformant to the    *
 * STAR File syntax, or the CIF syntax, respectively.                 *
 *                                                                    *
 * * Software claiming to read definitions from a specific data       *
 * dictionary approved by the IUCr must be able to extract any        *
 * pertinent definition which is conformant to the dictionary         *
 * definition language (DDL)[3] associated with that dictionary.      *
 *                                                                    *
 * The IUCr, through its Committee on CIF Standards, will assist      *
 * any developer to verify that software meets these conformance      *
 * conditions.                                                        *
 *                                                                    *
 * Glossary of terms                                                  *
 *                                                                    *
 * [1] CIF:  is a data file conformant to the file syntax defined     *
 * at http://www.iucr.org/iucr-top/cif/spec/index.html                *
 *                                                                    *
 * [2] STAR File:  is a data file conformant to the file syntax       *
 * defined at http://www.iucr.org/iucr-top/cif/spec/star/index.html   *
 *                                                                    *
 * [3] DDL:  is a language used in a data dictionary to define data   *
 * items in terms of "attributes". Dictionaries currently approved    *
 * by the IUCr, and the DDL versions used to construct these          *
 * dictionaries, are listed at                                        *
 * http://www.iucr.org/iucr-top/cif/spec/ddl/index.html               *
 *                                                                    *
 * Last modified: 30 September 2000                                   *
 *                                                                    *
 * IUCr Policy Copyright (C) 2000 International Union of              *
 * Crystallography                                                    *
 **********************************************************************/

#include "napi.h"
#include "cbf.h"
#include "cbf_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#ifdef GNUGETOPT
#include "getopt.h"
#endif
#include <unistd.h>

#define C2CBUFSIZ 8192
#define NUMDICTS 50

#ifdef __MINGW32__
#define NOMKSTEMP
#define NOTMPDIR
#endif



int local_exit (int status);
int outerror(int err);

int outerror(int err) 
{
	
  if ((err&CBF_FORMAT)==CBF_FORMAT)
    fprintf(stderr, " cif2nx: The file format is invalid.\n");
  if ((err&CBF_ALLOC)==CBF_ALLOC)
    fprintf(stderr, " cif2nx Memory allocation failed.\n");
  if ((err&CBF_ARGUMENT)==CBF_ARGUMENT)
    fprintf(stderr, " cif2nx: Invalid function argument.\n");
  if ((err&CBF_ASCII)==CBF_ASCII)
    fprintf(stderr, " cif2nx: The value is ASCII (not binary).\n");
  if ((err&CBF_BINARY)==CBF_BINARY)
    fprintf(stderr, " cif2nx: The value is binary (not ASCII).\n");
  if ((err&CBF_BITCOUNT)==CBF_BITCOUNT)
    fprintf(stderr, " cif2nx: The expected number of bits does" 
      " not match the actual number written.\n");
  if ((err&CBF_ENDOFDATA)==CBF_ENDOFDATA)
    fprintf(stderr, " cif2nx: The end of the data was reached"
     " before the end of the array.\n");
  if ((err&CBF_FILECLOSE)==CBF_FILECLOSE)
    fprintf(stderr, " cif2nx: File close error.\n");
  if ((err&CBF_FILEOPEN)==CBF_FILEOPEN)
    fprintf(stderr, " cif2nx: File open error.\n");
  if ((err&CBF_FILEREAD)==CBF_FILEREAD)
    fprintf(stderr, " cif2nx: File read error.\n");
  if ((err&CBF_FILESEEK)==CBF_FILESEEK)
    fprintf(stderr, " cif2nx: File seek error.\n");
  if ((err&CBF_FILETELL)==CBF_FILETELL)
    fprintf(stderr, " cif2nx: File tell error.\n");
  if ((err&CBF_FILEWRITE)==CBF_FILEWRITE)
    fprintf(stderr, " cif2nx: File write error.\n");
  if ((err&CBF_IDENTICAL)==CBF_IDENTICAL)
    fprintf(stderr, " cif2nx: A data block with the new name already exists.\n");
  if ((err&CBF_NOTFOUND)==CBF_NOTFOUND)
    fprintf(stderr, " cif2nx: The data block, category, column or"
      " row does not exist.\n");
  if ((err&CBF_OVERFLOW)==CBF_OVERFLOW)
    fprintf(stderr, " cif2nx: The number read cannot fit into the "
      "destination argument.\n        The destination has been set to the nearest value.\n");
  if ((err& CBF_UNDEFINED)==CBF_UNDEFINED)
    fprintf(stderr, " cif2nx: The requested number is not defined (e.g. 0/0).\n");
  if ((err&CBF_NOTIMPLEMENTED)==CBF_NOTIMPLEMENTED)
    fprintf(stderr, " cif2nx: The requested functionality is not yet implemented.\n");
    return 0;

}

#undef cbf_failnez
#define cbf_failnez(x) \
 {int err; \
  err = (x); \
  if (err) { \
    fprintf(stderr," cif2nx: CBFlib fatal error %d\n",err); \
    outerror(err); \
    local_exit (-1); \
  } \
 }

/* format a value into a properly quoted, newly allocated string
   according to type:
   
     "sglq"  -- put '\'' before and after the value
     "dblq"  -- put '"'  before and after the value
     "text"  -- put "\n;" before and "\n;\n" after the value
     for other types ("word" and "null") copy the string unchanged
     unless it is empty, in which case deliver "."     
     
  Note that the newly allocated itembuf must eventually be freed
  to avoid a memory leak */


int cbf_format_dataitem(char ** itembuf, const char* value, const char* type) {

  size_t numchars, slen;
  char * strbuf;
  char * pchar;
  
  slen = strlen(value);
  numchars = slen+1;
  if (slen == 0) numchars++;
  if (!cbf_cistrcmp(type,"sglq") || !cbf_cistrcmp(type,"dblq")) numchars +=2;
  if (!cbf_cistrcmp(type,"text") ) numchars +=5;
  
  if (!(strbuf = (char *)malloc(numchars) ) ) return CBF_ALLOC;
  
  pchar = strbuf;
  
  if (!cbf_cistrcmp(type,"sglq")) {
  	*pchar++='\'';
    strcpy(pchar,value);
    pchar+=slen;
    *pchar++='\'';
  } else if (!cbf_cistrcmp(type,"dblq")) {
  	*pchar++='"';
    strcpy(pchar,value);
    pchar+=slen;
    *pchar++='"';
  } else if (!cbf_cistrcmp(type,"text")) {
    *pchar++='\n'; *pchar++=';';
    strcpy(pchar,value);
    pchar+=slen;
    *pchar++='\n'; *pchar++=';'; *pchar++='\n';
  } else {
    if (slen == 0) {
      strcpy(pchar,".");
      pchar++;
    } else {
    strcpy(pchar,value);
    pchar+=slen;  	    	
    }
  }
  *pchar++ = '\0';
  *itembuf = strbuf;
  return 0;	
}

int main (int argc, char *argv [])
{
  FILE *in, *out=NULL, *file, *dict;
  clock_t a,b;
  cbf_handle cif;
  NXhandle nxf=NULL;
  int nxf_access=NXACC_CREATEXML;
  int nxrank[3];
  int nexus_compression;
  cbf_handle dic;
  cbf_handle odic;
  int devnull = 0;
  int c;
  int errflg = 0;
  char *cifin, *nxout;
  char *dictionary[NUMDICTS];
  char *nxprefix;
  char nxcifbuf[90];
  char nxcifrow[90];
  char *ciftmp=NULL;
#ifndef NOMKSTEMP
  int ciftmpfd;
#endif
  int ciftmpused;
  int padflag;
  int nbytes;
  int ndict = 0;
  int kd;
  int wide = 0;
  int nxcifbufbase = 6;
  int nxcifrowbase = 6;
  char buf[C2CBUFSIZ];
  unsigned int blocks, categories, blocknum, catnum, blockitems, itemnum;
  CBF_NODETYPE itemtype;
  const char *datablock_name;
  const char *saveframe_name;
  const char *category_name;
  const char *column_name;
  const char *value;
  char *cifname;
  unsigned int colnum, rownum;
  unsigned int columns;
  unsigned int rows;

  int mime, digest, encoding, compression, bytedir, cbforcif, term;


     /* Extract options */

/**********************************************************************
 *  cif2nx [-i input_cif] [-o output_nx] \                            *
 *    [-e [4|5|x]] [-c [n|g|h|r]]        \                            *
 *    [-n cifname] [-p prefix ]          \                                                  *
 *    [-v dictionary]* \                                              *
 *    [input_cif] [output_nx]                                         *
 *                                                                    *
 **********************************************************************/

   mime = 0;
   digest = 0;
   encoding = 0;
   compression = 0;
   nexus_compression=NX_COMP_LZW;
   bytedir = 0;
   ndict = 0;
   padflag = 0;

   cifin = NULL;
   nxout = NULL;
   cifname = NULL;
   nxprefix = NULL;
   ciftmpused = 0;
   
   
   
   while ((c = getopt(argc, argv, "i:o:v:e:c:n:p:")) != EOF) {
     switch (c) {
       case 'i':
         if (cifin) errflg++;
         else cifin = optarg;
         break;
       case 'o':
         if (nxout) errflg++;
         else nxout = optarg;
         break;
       case 'n':
         if (cifname) errflg++;
         else cifname = optarg;
         break;
       case 'p':
         if (nxprefix) errflg++;
         else nxprefix = optarg;
         break;
       case 'v':
         if (ndict < NUMDICTS)
           dictionary[ndict++] = optarg;
         else if (ndict == NUMDICTS) {
           errflg++;
           ndict++;
           fprintf(stderr, " cif2nx: Too many dictionaries, increase NUMDICTS");
         }
         break;
       case 'e':
         if (optarg[0]=='4')  {
         	nxf_access=NXACC_CREATE4;
         } else if (optarg[0]=='5') {
         	nxf_access=NXACC_CREATE5;
         } else if (optarg[0]=='x') {
         	nxf_access=NXACC_CREATEXML;
         } else errflg++;
     	 break;
       case 'c':
         if (optarg[0]=='n')  {
         	compression=0;
         } else if (optarg[0]=='g') {
            compression=1; nexus_compression=NX_COMP_LZW;
         } else if (optarg[0]=='h') {
            compression=1; nexus_compression=NX_COMP_HUF;
         } else if (optarg[0]=='r') {
            compression=1; nexus_compression=NX_COMP_RLE;
         } else errflg++;
         break;
       default:
         errflg++;
         break;
      }
    }
   for (; optind < argc; optind++) {
     if (!cifin) {
        cifin = argv[optind];
     } else {
       if (!nxout) {
         nxout = argv[optind];
       } else {
         errflg++;
       }
     }
   }
   if (errflg) {
     fprintf(stderr," cif2nx:  Usage: \n");
     fprintf(stderr,
       "  cif2nx [-i input_cif] [-o output_nx] \\\n");
     fprintf(stderr,
       "    [-e [4|5|x]] [-c [n|g|h|r]] \\\n");
     fprintf(stderr,
       "    [-n cifname] [-p nxprefix]  \\\n");
     fprintf(stderr,
       "    [-v dictionary]* \\\n");
     fprintf(stderr,
       "    [input_cif] [output_nx] \n\n");
     exit(2);
   }


    /* Use "NXcif_" as a prefix if nothing has been specified */
    
    if (!nxprefix) nxprefix = "NXcif_";
    strncpy(nxcifbuf,nxprefix,75);
    nxcifbuf[75] = '\0';
    strcpy(nxcifrow,nxprefix);
    nxcifbufbase=strlen(nxcifbuf);
    nxcifrowbase=strlen(nxcifrow);
    

    /* Read the cif */

   if (!cifin || strcmp(cifin?cifin:"","-") == 0) {
     ciftmp = (char *)malloc(strlen("/tmp/cif2nxXXXXXX")+1);
#ifdef NOTMPDIR
     strcpy(ciftmp, "cif2nxXXXXXX");
#else
     strcpy(ciftmp, "/tmp/cif2nxXXXXXX");
#endif
#ifdef NOMKSTEMP
     if ((ciftmp = mktemp(ciftmp)) == NULL ) {
       fprintf(stderr," cif2nx: Can't create temporary file name %s.\n", ciftmp);
       fprintf(stderr,"%s\n",strerror(errno));
       local_exit (1);
     }
     if ( (file = fopen(ciftmp,"wb+")) == NULL) {
       fprintf(stderr," cif2nx: Can't open temporary file %s.\n", ciftmp);
       fprintf(stderr,"%s\n",strerror(errno));
       local_exit (1);     	
     }
#else
     if ((ciftmpfd = mkstemp(ciftmp)) == -1 ) {
       fprintf(stderr," cif2nx: Can't create temporary file %s.\n", ciftmp);
       fprintf(stderr,"%s\n",strerror(errno));
       local_exit (1);
     }
     if ( (file = fdopen(ciftmpfd, "w+")) == NULL) {
       fprintf(stderr," cif2nx: Can't open temporary file %s.\n", ciftmp);
       fprintf(stderr,"%s\n",strerror(errno));
       local_exit (1);
     }
#endif
     while ((nbytes = fread(buf, 1, C2CBUFSIZ, stdin))) {
       if(nbytes != fwrite(buf, 1, nbytes, file)) {
         fprintf(stderr," cif2nx: Failed to write %s.\n", ciftmp);
         local_exit (1);
       }
     }
     fclose(file);
     cifin = ciftmp;
     ciftmpused = 1;
   }
   if ( cbf_make_handle (&cif) ) {
     fprintf(stderr," cif2nx: Failed to create handle for input_cif\n");
     local_exit (1);
   }
   
   if ( cbf_make_handle (&dic) ) {
     fprintf(stderr," cif2nx: Failed to create handle for dictionary\n");
     local_exit (1);
   }


   for (kd=0; kd< ndict; kd++) {
   
     if (!(dict = fopen (dictionary[kd], "rb")))  {
     	fprintf (stderr," cif2nx: Couldn't open the dictionary %s\n", dictionary[kd]);
        exit (1);
     }
     cbf_failnez(cbf_read_widefile(dic, dict, MSG_DIGEST))
     cbf_failnez(cbf_convert_dictionary(cif,dic))
   	
   }
   
   a = clock ();

   /* Read the file */
   if (!(in = fopen (cifin, "rb"))) {
     fprintf (stderr," cif2nx: Couldn't open the input CIF file %s\n", cifin);
     exit (1);
   }

   if (ciftmpused) {
     if (unlink(ciftmp) != 0 ) {
       fprintf(stderr," cif2cif:  Can't unlink temporary file %s.\n", ciftmp);
       fprintf(stderr,"%s\n",strerror(errno));
       local_exit (1);
     }
   }

   cbf_failnez (cbf_read_widefile (cif, in, MSG_DIGEST))
   
   /* Open the output file */
   
   if (NXopen (nxout, nxf_access, &nxf) != NX_OK) {
     fprintf (stderr," cif2nx: Couldn't open the output NeXus file %s\n", nxout);
   }
   
   if (!cifname) cifname = "NXcif";
       
   if (NXmakegroup (nxf, cifname, "NXcif") != NX_OK) {
     	fprintf(stderr," cif2nx: Failed to create group NXcif::%s\n",cifname);
        local_exit (1);
   }
   if (NXopengroup (nxf, cifname, "NXcif") != NX_OK) {
     	fprintf(stderr," cif2nx: Failed to open group NXcif::%s\n",cifname);
        local_exit (1);
   }
    

   cbf_failnez (cbf_rewind_datablock(cif))

   cbf_failnez (cbf_count_datablocks(cif, &blocks))

   for (blocknum = 0; blocknum < blocks;  blocknum++ )
   { /* start of copy loop */
       
       char ** columnarray = NULL; 
       char ** columnarraytype; 
       
       cbf_failnez (cbf_select_datablock(cif, blocknum))
       cbf_failnez (cbf_datablock_name(cif, &datablock_name))
       strcpy(nxcifbuf+nxcifbufbase,"datablock");
       if (NXmakegroup (nxf, datablock_name, nxcifbuf) != NX_OK) {
           fprintf(stderr," cif2nx: Failed to create datablock %s\n",datablock_name);
           local_exit (1);
       }
       if (NXopengroup (nxf, datablock_name, nxcifbuf) != NX_OK) {
           fprintf(stderr," cif2nx: Failed to open datablock %s\n",datablock_name);
           local_exit (1);
       }
       
       if ( !cbf_rewind_blockitem(cif, &itemtype) ) {
           cbf_failnez (cbf_count_blockitems(cif, &blockitems))
           
           for (itemnum = 0; itemnum < blockitems;  itemnum++) {
               cbf_select_blockitem(cif, itemnum, &itemtype);
               if (itemtype == CBF_CATEGORY) {
                   cbf_category_name(cif,&category_name);
                   /* Create the category group for this category */
                   strcpy(nxcifbuf+nxcifbufbase,"category");
                   if (NXmakegroup (nxf, category_name, nxcifbuf) != NX_OK) {
                       fprintf(stderr," cif2nx: Failed to create category %s\n",category_name);
                       local_exit (1);
                   }
                   /*  Open the category group for this category */
                   if (NXopengroup (nxf, category_name, nxcifbuf) != NX_OK) {
                       fprintf(stderr," cif2nx: Failed to open category %s\n",category_name);
                       local_exit (1);
                   }
                   cbf_count_rows(cif,&rows);
                   nxrank[0]=rows;
                   nxrank[1]=2;
                   if (rows) columnarray = (char **)malloc(rows*sizeof(char *)*2);
                   if (rows && !columnarray) {
                       fprintf(stderr," cif2nx: Failed to allocate columnarray %s\n",category_name);
                       local_exit (1);
                   }
                   columnarraytype = columnarray+rows;
                   cbf_count_columns(cif,&columns);
                   
                   /*  Transfer the columns names from cif to nexus */
                   if (columns && ! cbf_rewind_column(cif) ) {
                       int somebinary;
                       unsigned int irn=0;
                       int irnrank=1;
                       char scratchbuf[40];
                       do {
                           cbf_failnez(cbf_column_name(cif, &column_name))
                           cbf_rewind_row(cif);
                           somebinary=0;
                           /* Transfer column from cif to nexus */
                           for (rownum = 0; rownum < rows; rownum++) {
                               const char *typeofvalue;
                               columnarray[rownum]=NULL;
                               columnarraytype[rownum]=NULL;
                               cbf_failnez(cbf_select_row(cif,rownum))
                               if ( ! cbf_get_value(cif, &value) ) {
                                   cbf_failnez (cbf_get_typeofvalue(cif, &typeofvalue))
                                   columnarray[rownum]=(char *)value;
                                   columnarraytype[rownum]=(char *)typeofvalue;
                                   if (!columnarray[rownum]) {
                                       columnarray[rownum] = ".";
                                       columnarraytype[rownum] = "null";
                                   }
                                   if (columnarraytype[rownum]==NULL) columnarraytype[rownum]= "undefined";
                                   
                                   /* DEBUG:  fprintf(stderr,"Column %s, row %d, value %s, type %s\n",
                                    column_name, rownum, columnarray[rownum], columnarraytype[rownum] ); */
                                   
                                   if (somebinary) {
                                       char * strbuf;
                                       sprintf(nxcifrow,"data[%-d]",rownum);
                                       if (rows==1)sprintf(nxcifrow,"data");
                                       cbf_failnez(cbf_format_dataitem(&strbuf,columnarray[rownum],columnarraytype[rownum]))
                                       irnrank=strlen(strbuf)+1;
                                       if (irnrank < 4096 || !compression){
                                           if (NXmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);
                                           }
                                       } else {
                                           if (NXcompmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank, nexus_compression, &irnrank) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);
                                           }                 	
                                       }
                                       if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to open row %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);
                                       }
                                       if (NXputdata (nxf, strbuf ) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1); 	
                                       } 
                                       free(strbuf);
                                       if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store itemcount of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       NXclosedata(nxf);
                                   }
                               } else {
                                   char * typebuffer;
                                   void * array;
                                   int binary_id, elsigned, elunsigned;
                                   size_t elements,elements_read, elsize;
                                   int minelement, maxelement;
                                   unsigned int cifcompression;
                                   int realarray;
                                   const char *byteorder;
                                   size_t dim1, dim2, dim3, padding;
                                   
                                   /* If this is the first encounter with a binary in the column
                                    we need to create a group for the column and move whatever rows
                                    were being saved in columnarray into individual datasets  */
                                   
                                   if (!somebinary) {
                                       
                                       strcpy(nxcifbuf+nxcifbufbase,"column");
                                       if (NXmakegroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                           local_exit (1);
                                       }
                                       if (NXopengroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                           local_exit (1);
                                       }
                                       /* copy the rows that were already done */
                                       for (irn = 0; irn < rownum; irn++) {
                                           char * strbuf;
                                           sprintf(nxcifrow,"data[%-d]",irn);
                                           if (rows==1) sprintf(nxcifrow,"data");
                                           cbf_failnez(cbf_format_dataitem(&strbuf,columnarray[irn],columnarraytype[irn]))
                                           irnrank=strlen(strbuf)+1;
                                           if (irnrank < 4096 || !compression){
                                               if (NXmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);
                                               }
                                           } else {
                                               if (NXcompmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank, nexus_compression, &irnrank) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);
                                               }                 	
                                           }
                                           
                                           if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to open %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);
                                           }
                                           if (NXputdata (nxf, strbuf ) != NX_OK ) {
                                               fprintf(stderr," cif2nx: Failed to store %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1); 	
                                           } 
                                           free(strbuf);
                                           if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                               fprintf(stderr," cif2nx: Failed to store itemcount of %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);	
                                           }
                                           NXclosedata(nxf);
                                       }
                                       somebinary = 1;
                                   }
                                   
                                   
                                   /* The data we get from CIF may be an integer array of up to 3 dimensions
                                    or a real array of up to 3 dimensions.  (Note that the limit to 3 dimensions
                                    is just a matter of the current CBFlib API.  CBF itself may have any number
                                    of dimensions, and this should be allowed for in anything we do here) */
                                   
                                   cbf_failnez(cbf_get_arrayparameters_wdims(
                                                                             cif, &cifcompression,
                                                                             &binary_id, &elsize, &elsigned, &elunsigned,
                                                                             &elements, &minelement, &maxelement, &realarray,
                                                                             &byteorder, &dim1, &dim2, &dim3, &padding))
                                   if ((array=malloc(elsize*elements))) {
                                       
                                       int ii, arrayrank, nexustype;
                                       
                                       if (!realarray)  {
                                           cbf_failnez (cbf_get_integerarray(
                                                                             cif, &binary_id, array, elsize, elsigned,
                                                                             elements, &elements_read))
                                       } else {
                                           cbf_failnez (cbf_get_realarray(
                                                                          cif, &binary_id, array, elsize,
                                                                          elements, &elements_read))
                                           elsigned = 1;
                                       }
                                       nxrank[0] = dim1;
                                       nxrank[1] = dim2;
                                       nxrank[2] = dim3;
                                       
                                       nexustype = NX_UINT8;
                                       if (realarray) {
                                           if (elsize == 4) nexustype = NX_FLOAT32;
                                           else if (elsize == 8) nexustype = NX_FLOAT64;
                                       } else {
                                           if (elunsigned) {
                                               if (elsize == 1) nexustype = NX_UINT8;
                                               else if (elsize == 2) nexustype = NX_UINT16;
                                               else if (elsize == 4) nexustype = NX_UINT32;                       	
                                           } else {
                                               if (elsize == 1) nexustype = NX_INT8;
                                               else if (elsize == 2) nexustype = NX_INT16;
                                               else if (elsize == 4) nexustype = NX_INT32;                       	
                                               
                                           }
                                           
                                       }
                                       
                                       arrayrank=1;
                                       for (ii=0; ii< 3; ii++)  {
                                           if (nxrank[ii] == 0) nxrank[ii] = 1;
                                           if (nxrank[ii] > 1) arrayrank = ii+1;
                                       }
                                       if (nexustype == NX_UINT8) nxrank[0] *= elsize;
                                       
                                       
                                       /* Now we have already made the column into a group and the rows into
                                        data sets, the group for the column is open.  We need to create and
                                        populate the data set as the next row */ 
                                       
                                       sprintf(nxcifrow,"data[%-d]",rownum);
                                       if (rows==1)sprintf(nxcifrow,"data");
                                       if (!compression)  {
                                           if (NXmakedata (nxf, nxcifrow, nexustype, arrayrank, nxrank) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);
                                           }
                                       } else  {
                                           if (NXcompmakedata (nxf, nxcifrow, nexustype, arrayrank, nxrank,nexus_compression,nxrank) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                               local_exit (1);
                                           }                   	
                                       }
                                       if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to open %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);
                                       }
                                       if (NXputdata (nxf,array) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to put data in open %s in column %s\n",nxcifrow,column_name);
                                       }
                                       free(array);
                                       if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store itemcount of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       if (NXputattr(nxf,"NXciftype","binary",strlen("binary"),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store CIF type of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       if (NXputattr(nxf,"NXcifarrayelementtype",
                                                     realarray?"real":"integer",
                                                     strlen(realarray?"real":"integer"),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store CIF arrayelementtype of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       sprintf(scratchbuf,"%-ld",elsize);
                                       if (NXputattr(nxf,"NXcifarrayelementsize",scratchbuf,strlen(scratchbuf),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store CIF arrayelementsize of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       if (NXputattr(nxf,"NXcifarrayelementsign",
                                                     elunsigned?"unsigned":"signed",
                                                     strlen(elunsigned?"unsigned":"signed"),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store CIF arrayelementsign of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       if (NXputattr(nxf,"NXcifarraybyteorder", (char *)byteorder, strlen(byteorder),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store CIF arraybyteorder of %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       NXclosedata(nxf);
                                       
                                       NXflush(&nxf);
                                       
                                       
                                   } else {
                                       fprintf(stderr,
                                               "\nFailed to allocate memory %ld bytes",
                                               (long) elsize*elements);
                                       local_exit (1);
                                   }
                                   
                               }
                           }
                           for (rownum = 0; rownum < rows; rownum++) {
                               if (columnarraytype[rownum]==NULL) columnarraytype[rownum]= "null";
                               /* DEBUG:  fprintf(stderr,"Column %s, row %d, value %s, type %s\n",
                                column_name, rownum, columnarray[rownum], columnarraytype[rownum] ); */
                               
                           }
                           
                           if (!somebinary) {
                               
                               int numchars;
                               char * strbuf;
                               char * pchar;
                               
                               /* Create and open a column group to hold the entire column */
                               
                               strcpy(nxcifbuf+nxcifbufbase,"column");
                               if (NXmakegroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                   fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                   local_exit (1);
                               }
                               if (NXopengroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                   fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                   local_exit (1);
                               }
                               
                               /* The rows have not been transferred.  We will convert to one
                                large concatenated string, putting a newline between rows */
                               
                               numchars = 0;
                               for (rownum = 0; rownum < rows;rownum++) {
                                   numchars++;
                                   numchars+=strlen(columnarray[rownum]);
                                   if (!cbf_cistrcmp(columnarraytype[rownum],"sglq") || 
                                       !cbf_cistrcmp(columnarraytype[rownum],"dblq")) numchars+=2;
                                   if (!cbf_cistrcmp(columnarraytype[rownum],"text")) numchars+=5;
                               }
                               
                               numchars++;
                               
                               if (!(strbuf = (char *)malloc(numchars) ) ) {
                                   fprintf(stderr," cbf2nx -- Failed to allocate memory for category %s column %s\n",
                                           category_name,column_name);
                               }
                               
                               pchar = strbuf;
                               if (rows > 1) *pchar++='\n';
                               
                               for (rownum = 0; rownum < rows; rownum++) {
                                   if (rownum > 0)  *pchar++='\n';
                                   if (!cbf_cistrcmp(columnarraytype[rownum],"sglq")) {
                                       *pchar++='\'';
                                       strcpy(pchar,columnarray[rownum]);
                                       pchar+=strlen(columnarray[rownum]);
                                       *pchar++='\'';
                                       continue;
                                   }
                                   if (!cbf_cistrcmp(columnarraytype[rownum],"dblq")) {
                                       *pchar++='\"';
                                       strcpy(pchar,columnarray[rownum]);
                                       pchar+=strlen(columnarray[rownum]);
                                       *pchar++='\"';
                                       continue;
                                   }
                                   if (!cbf_cistrcmp(columnarraytype[rownum],"text")) {
                                       *pchar++='\n'; *pchar++=';';
                                       strcpy(pchar,columnarray[rownum]);
                                       pchar+=strlen(columnarray[rownum]);
                                       *pchar++='\n'; *pchar++=';';
                                       continue;
                                   }
                                   strcpy(pchar,columnarray[rownum]);
                                   pchar+=strlen(columnarray[rownum]);
                               }
                               *pchar++='\0';
                               numchars = strlen(strbuf)+1;
                               
                               if (numchars < 4096 || !compression) {
                                   if (NXmakedata (nxf, "data", NX_CHAR, 1, &numchars) != NX_OK) {
                                       fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                       local_exit (1);
                                   }	
                               } else  {
                                   if (NXcompmakedata (nxf, "data", NX_CHAR, 1, &numchars,nexus_compression, &numchars) != NX_OK) {
                                       fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                       local_exit (1);
                                   }
                               }
                               if (NXopendata (nxf, "data") != NX_OK) {
                                   fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                   local_exit (1);
                               }
                               if (NXputdata (nxf, strbuf) != NX_OK ) {
                                   fprintf(stderr," cif2nx: Failed to store column %s\n",column_name);
                                   local_exit (1);	
                               }
                               sprintf(nxcifrow,"%-d",rows);
                               if (NXputattr(nxf,"items",nxcifrow,strlen(nxcifrow),NX_CHAR) != NX_OK ) {
                                   fprintf(stderr," cif2nx: Failed to store itemcount %s in column %s\n",nxcifrow,column_name);
                                   local_exit (1);	
                               }
                               free(strbuf);
                               NXclosedata( nxf ); /* close the columns as a data set */
                           }
                           NXclosegroup( nxf); /* close the columns as a group */
                       } while ( ! cbf_next_column(cif) );
                       if (rows) free(columnarray);
                   }
                   NXclosegroup( nxf ); /* close the category */
               } else {
                   cbf_saveframe_name(cif,&saveframe_name);
                   strcpy(nxcifbuf+nxcifbufbase,"saveframe");
                   if (NXmakegroup (nxf, saveframe_name, nxcifbuf) != NX_OK) {
                       fprintf(stderr," cif2nx: Failed to create saveframe %s\n",saveframe_name);
                       local_exit (1);
                   }
                   if (NXopengroup (nxf, saveframe_name, nxcifbuf) != NX_OK) {
                       fprintf(stderr," cif2nx: Failed to open saveframe %s\n",saveframe_name);
                       local_exit (1);
                   }
                   if ( !cbf_rewind_category(cif) ) {
                       cbf_failnez (cbf_count_categories(cif, &categories))
                       
                       for (catnum = 0; catnum < categories;  catnum++) {
                           cbf_select_category(cif, catnum);
                           cbf_category_name(cif,&category_name);
                           strcpy(nxcifbuf+nxcifbufbase,"category");
                           if (NXmakegroup (nxf, category_name, nxcifbuf) != NX_OK) {
                               fprintf(stderr," cif2nx: Failed to create category %s\n",category_name);
                               local_exit (1);
                           }
                           if (NXopengroup (nxf, category_name, nxcifbuf) != NX_OK) {
                               fprintf(stderr," cif2nx: Failed to open category %s\n",category_name);
                               local_exit (1);
                           }
                           cbf_count_rows(cif,&rows);
                           nxrank[0]=rows;
                           nxrank[1]=2;
                           if (rows) columnarray = (char **)malloc(rows*sizeof(char *)*2);
                           if (rows && !columnarray) {
                               fprintf(stderr," cif2nx: Failed to allocate columnarray %s\n",category_name);
                               local_exit (1);
                           }
                           columnarraytype = columnarray+rows;
                           cbf_count_columns(cif,&columns);
                           
                           /*  Transfer the columns names from cif to nexus */
                           if (columns && ! cbf_rewind_column(cif) ) {
                               int somebinary;
                               somebinary = 0;
                               unsigned int irn;
                               int irnrank=1;
                               char scratchbuf[40];
                               do {
                                   cbf_failnez(cbf_column_name(cif, &column_name))
                                   cbf_rewind_row(cif);
                                   /* Transfer column from cif to nexus */
                                   for (rownum = 0; rownum < rows; rownum++) {
                                       const char *typeofvalue;
                                       columnarray[rownum]=NULL;
                                       columnarraytype[rownum]=NULL;
                                       cbf_failnez(cbf_select_row(cif,rownum))
                                       if ( ! cbf_get_value(cif, &value) ) {
                                           cbf_failnez (cbf_get_typeofvalue(cif, &typeofvalue))
                                           columnarray[rownum]=(char *)value;
                                           columnarraytype[rownum]=(char *)typeofvalue;
                                           if (!columnarray[rownum]) {
                                               columnarray[rownum] = ".";
                                               columnarraytype[rownum] = "null";
                                           }
                                           if (columnarraytype[rownum]==NULL) columnarraytype[rownum]= "undefined";
                                           if (somebinary) {
                                               char * strbuf;
                                               sprintf(nxcifrow,"data[%-d]",rownum);
                                               if (rows==1)sprintf(nxcifrow,"data");
                                               cbf_failnez(cbf_format_dataitem(&strbuf,columnarray[rownum],columnarraytype[rownum]))
                                               irnrank=strlen(strbuf)+1;
                                               if (irnrank < 4096 || !compression){
                                                   if (NXmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank) != NX_OK) {
                                                       fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);
                                                   }
                                               } else {
                                                   if (NXcompmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank, nexus_compression, &irnrank) != NX_OK) {
                                                       fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);
                                                   }                 	
                                               }
                                               if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to open%s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);
                                               }
                                               if (NXputdata (nxf, strbuf ) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1); 	
                                               } 
                                               free(strbuf);
                                               if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store itemcount of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               NXclosedata(nxf);
                                           }
                                       } else {
                                           char * typebuffer;
                                           void * array;
                                           int binary_id, elsigned, elunsigned;
                                           size_t elements,elements_read, elsize;
                                           int minelement, maxelement;
                                           unsigned int cifcompression;
                                           int realarray;
                                           const char *byteorder;
                                           size_t dim1, dim2, dim3, padding;
                                           
                                           /* If this is the first encounter with a binary in the column
                                            we need to create a group for the column and move whatever rows
                                            were being saved in columnarray into individual datasets  */
                                           
                                           if (!somebinary) {
                                               
                                               strcpy(nxcifbuf+nxcifbufbase,"column");
                                               if (NXmakegroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                                   local_exit (1);
                                               }
                                               if (NXopengroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                                   local_exit (1);
                                               }
                                               /* copy the rows that were already done */
                                               for (irn = 0; irn < rownum; irn++) {
                                                   char* strbuf;
                                                   sprintf(nxcifrow,"data[%-d]",irn);
                                                   if (rows==1) sprintf(nxcifrow,"data");
                                                   cbf_failnez(cbf_format_dataitem(&strbuf,columnarray[irn],columnarraytype[irn]))
                                                   irnrank=strlen(strbuf)+1;
                                                   if (irnrank < 4096 || !compression){
                                                       if (NXmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank) != NX_OK) {
                                                           fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                           local_exit (1);
                                                       }
                                                   } else {
                                                       if (NXcompmakedata (nxf, nxcifrow, NX_CHAR, 1, &irnrank, nexus_compression, &irnrank) != NX_OK) {
                                                           fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                           local_exit (1);
                                                       }                 	
                                                   }
                                                   if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                                       fprintf(stderr," cif2nx: Failed to open %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);
                                                   }
                                                   if (NXputdata (nxf, strbuf ) != NX_OK ) {
                                                       fprintf(stderr," cif2nx: Failed to store %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1); 	
                                                   } 
                                                   free(strbuf);
                                                   
                                                   if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                                       fprintf(stderr," cif2nx: Failed to store rowspan of row %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);	
                                                   }
                                                   NXclosedata(nxf);
                                               }
                                               somebinary = 1;
                                           }
                                           
                                           
                                           /* The data we get from CIF may be an integer array of up to 3 dimensions
                                            or a real array of up to 3 dimensions.  (Note that the limit to 3 dimensions
                                            is just a matter of the current CBFlib API.  CBF itself may have any number
                                            of dimensions, and this should be allowed for in anything we do here) */
                                           
                                           cbf_failnez(cbf_get_arrayparameters_wdims(
                                                                                     cif, &cifcompression,
                                                                                     &binary_id, &elsize, &elsigned, &elunsigned,
                                                                                     &elements, &minelement, &maxelement, &realarray,
                                                                                     &byteorder, &dim1, &dim2, &dim3, &padding))
                                           if ((array=malloc(elsize*elements))) {
                                               
                                               int ii, arrayrank, nexustype;
                                               
                                               if (!realarray)  {
                                                   cbf_failnez (cbf_get_integerarray(
                                                                                     cif, &binary_id, array, elsize, elsigned,
                                                                                     elements, &elements_read))
                                               } else {
                                                   cbf_failnez (cbf_get_realarray(
                                                                                  cif, &binary_id, array, elsize,
                                                                                  elements, &elements_read))
                                                   elsigned = 1;
                                               }
                                               nxrank[0] = dim1;
                                               nxrank[1] = dim2;
                                               nxrank[2] = dim3;
                                               
                                               nexustype = NX_UINT8;
                                               if (realarray) {
                                                   if (elsize == 4) nexustype = NX_FLOAT32;
                                                   else if (elsize == 8) nexustype = NX_FLOAT64;
                                               } else {
                                                   if (elunsigned) {
                                                       if (elsize == 1) nexustype = NX_UINT8;
                                                       else if (elsize == 2) nexustype = NX_UINT16;
                                                       else if (elsize == 4) nexustype = NX_UINT32;                       	
                                                   } else {
                                                       if (elsize == 1) nexustype = NX_INT8;
                                                       else if (elsize == 2) nexustype = NX_INT16;
                                                       else if (elsize == 4) nexustype = NX_INT32;
                                                   }
                                               }
                                               
                                               arrayrank=1;
                                               for (ii=0; ii< 3; ii++)  {
                                                   if (nxrank[ii] == 0) nxrank[ii] = 1;
                                                   if (nxrank[ii] > 1) arrayrank = ii+1;
                                               }
                                               if (nexustype == NX_UINT8) nxrank[0] *= elsize;
                                               
                                               /* Now we have already made the column into a group and the rows into
                                                data sets, the group for the column is open.  We need to create and
                                                populate the data set as the next row */ 
                                               
                                               
                                               sprintf(nxcifrow,"data[%-d]",rownum);
                                               if (rows==1)sprintf(nxcifrow,"data");
                                               if (!compression) {
                                                   if (NXmakedata (nxf, nxcifrow, nexustype, 1, nxrank) != NX_OK) {
                                                       fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);
                                                   }	
                                               } else {
                                                   if (NXcompmakedata (nxf, nxcifrow, nexustype, 1, nxrank, nexus_compression,nxrank) != NX_OK) {
                                                       fprintf(stderr," cif2nx: Failed to create %s in column %s\n",nxcifrow,column_name);
                                                       local_exit (1);
                                                   }	
                                               }
                                               if (NXopendata (nxf, nxcifrow) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to open %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);
                                               }
                                               if (NXputdata (nxf,array) != NX_OK) {
                                                   fprintf(stderr," cif2nx: Failed to put data in open %s in column %s\n",nxcifrow,column_name);
                                               }
                                               free(array);
                                               if (NXputattr(nxf,"items","1",1,NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store rowspan of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               if (NXputattr(nxf,"NXciftype","binary",strlen("binary"),NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store CIF type of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               if (NXputattr(nxf,"NXcifarrayelementtype",
                                                             realarray?"real":"integer",
                                                             strlen(realarray?"real":"integer"),NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store CIF arrayelementtype of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               sprintf(scratchbuf,"%-ld",elsize);
                                               if (NXputattr(nxf,"NXcifarrayelementsize",scratchbuf,strlen(scratchbuf),NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store CIF arrayelementsize of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               if (NXputattr(nxf,"NXcifarrayelementsign",
                                                             elunsigned?"unsigned":"signed",
                                                             strlen(elunsigned?"unsigned":"signed"),NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store CIF arrayelementsign of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               if (NXputattr(nxf,"NXcifarraybyteorder", (char *)byteorder, strlen(byteorder),NX_CHAR) != NX_OK ) {
                                                   fprintf(stderr," cif2nx: Failed to store CIF arraybyteorder of %s in column %s\n",nxcifrow,column_name);
                                                   local_exit (1);	
                                               }
                                               NXclosedata(nxf);  /* Close the data set, leave the column open */
                                               
                                           } else {
                                               fprintf(stderr,
                                                       "\nFailed to allocate memory %ld bytes",
                                                       (long) elsize*elements);
                                               local_exit (1);
                                           } 
                                       }
                                   } /* for (rownum = 0; rownum < rows; rownum++) */
                                   
                                   /* See if we got to the end without hitting a binary */
                                   if (!somebinary) {
                                       
                                       int numchars;
                                       char * strbuf;
                                       char * pchar;
                                       
                                       /* Create and open an column group to hold the entire column */
                                       
                                       strcpy(nxcifbuf+nxcifbufbase,"column");
                                       if (NXmakegroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                           local_exit (1);
                                       }
                                       if (NXopengroup (nxf, column_name, nxcifbuf) != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                           local_exit (1);
                                       }
                                       
                                       /* The rows have not been transferred.  We will convert to one
                                        large concatenated string, putting a newline between rows */
                                       
                                       for (rownum = 0; rownum < rows;rownum++) {
                                           numchars++;
                                           numchars+=strlen(columnarray[rownum]);
                                           if (!cbf_cistrcmp(columnarraytype[rownum],"sglq") || 
                                               !cbf_cistrcmp(columnarraytype[rownum],"dblq")) numchars+=2;
                                           if (!cbf_cistrcmp(columnarraytype[rownum],"text")) numchars+=5;
                                       }
                                       
                                       numchars++;
                                       
                                       if (!(strbuf = (char *)malloc(numchars) ) ) {
                                           fprintf(stderr," cbf2nx -- Failed to allocate memory for category %s column %s\n",
                                                   category_name,column_name);
                                       }
                                       
                                       pchar = strbuf;
                                       if (rows > 1) *pchar++='\n';
                                       
                                       for (rownum = 0; rownum < rows; rownum++) {
                                           if (rownum > 0)  *pchar++='\n';
                                           if (!cbf_cistrcmp(columnarraytype[rownum],"sglq")) {
                                               *pchar++='\'';
                                               strcpy(pchar,columnarray[rownum]);
                                               pchar+=strlen(columnarray[rownum]);
                                               *pchar++='\'';
                                               continue;
                                           }
                                           if (!cbf_cistrcmp(columnarraytype[rownum],"dblq")) {
                                               *pchar++='\"';
                                               strcpy(pchar,columnarray[rownum]);
                                               pchar+=strlen(columnarray[rownum]);
                                               *pchar++='\"';
                                               continue;
                                           }
                                           if (!cbf_cistrcmp(columnarraytype[rownum],"text")) {
                                               *pchar++='\n'; *pchar++=';';
                                               strcpy(pchar,columnarray[rownum]);
                                               pchar+=strlen(columnarray[rownum]);
                                               *pchar++='\n'; *pchar++=';';
                                               continue;
                                           }
                                           strcpy(pchar,columnarray[rownum]);
                                           pchar+=strlen(columnarray[rownum]);
                                       }
                                       *pchar++='\0';
                                       numchars = strlen(strbuf)+1;
                                       
                                       if (numchars < 4096 || !compression) {
                                           if (NXmakedata (nxf, "data", NX_CHAR, 1, &numchars) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                               local_exit (1);
                                           }	
                                       } else {
                                           if (NXcompmakedata (nxf, "data", NX_CHAR, 1, &numchars, nexus_compression, &numchars) != NX_OK) {
                                               fprintf(stderr," cif2nx: Failed to create column %s\n",column_name);
                                               local_exit (1);
                                           }	
                                           
                                       }
                                       if (NXopendata (nxf, "data") != NX_OK) {
                                           fprintf(stderr," cif2nx: Failed to open column %s\n",column_name);
                                           local_exit (1);
                                       }
                                       if (NXputdata (nxf, strbuf) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store column %s\n",column_name);
                                           local_exit (1);	
                                       }
                                       sprintf(nxcifrow,"%-d",rows);
                                       if (NXputattr(nxf,"items",nxcifrow,strlen(nxcifrow),NX_CHAR) != NX_OK ) {
                                           fprintf(stderr," cif2nx: Failed to store itemcount %s in column %s\n",nxcifrow,column_name);
                                           local_exit (1);	
                                       }
                                       
                                       free(strbuf);
                                       NXclosedata( nxf );  /* Close the column as a dataset */
                                   } 
                                   NXclosegroup( nxf );  /* Close the column as a group */
                                   somebinary = 0;
                               } while ( ! cbf_next_column(cif) );
                               if (rows) free(columnarray);
                           }
                           NXclosegroup( nxf );  /* close the category */
                       }
                   }
                   NXclosegroup( nxf ); /* close the save frame */
               }
           }
           NXclosegroup( nxf ); /* close the data block */
       }
       
   }
   NXclosegroup( nxf ); /* close the NXcif group */

   b = clock ();
   fprintf (stderr,
     " Time to read input_cif: %.3fs\n",
       ((b - a) * 1.0) / CLOCKS_PER_SEC);
   a = clock ();

   NXclose( &nxf );
   b = clock ();
   fprintf (stderr, " Time to write the NeXus file: %.3fs\n",
       ((b - a) * 1.0) / CLOCKS_PER_SEC);

   exit(0);

}

int local_exit (int status)
{
  exit(status);
  return 1; /* avoid warnings */
}





