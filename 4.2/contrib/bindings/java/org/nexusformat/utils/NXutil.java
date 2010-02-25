/*
 * Copyright (c) 2003, P.F.Peterson <petersonpf@ornl.gov>
 *               Spallation Neutron Source at Oak Ridge National Laboratory
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
package org.nexusformat.util;

import java.lang.Math;
import java.lang.reflect.Array;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
import org.nexusformat.AttributeEntry;
import org.nexusformat.NeXusFileInterface;
import org.nexusformat.NexusFile;
import org.nexusformat.NexusException;
import ncsa.hdf.hdflib.HDFConstants;

/**
 * This is a class of utility functions for accessing information in a
 * nexus file.
 */
public class NXutil{
  private static final int CHAR      = HDFConstants.DFNT_CHAR;
  private static final int SH_SHORT  = HDFConstants.DFNT_INT8;
  private static final int SH_USHORT = HDFConstants.DFNT_UINT8;
  private static final int SHORT     = HDFConstants.DFNT_INT16;
  private static final int USHORT    = HDFConstants.DFNT_UINT16;
  private static final int INT       = HDFConstants.DFNT_INT32;
  private static final int UINT      = HDFConstants.DFNT_UINT32;
  private static final int FLOAT     = HDFConstants.DFNT_FLOAT32;
  private static final int DOUBLE    = HDFConstants.DFNT_FLOAT64;

  private static final int MAX_BLOB_SIZE=20000;
  private static final boolean DEBUG=false;

  /**
   * Like the name says, this is a utility class. Do not allow anyone
   * to instantiate this class.
   */
  private NXutil(){}

  /**
   * Get the list of attributes on the current level of the file
   * given.
   */
  public static Hashtable getAttrList(NeXusFileInterface file)
                                   throws NexusException, NullPointerException{
    // don't know what to do with a null file pointer
    if(file==null) throw new NullPointerException("null CNexusFile");

    // get the initial hashtable
    Hashtable table=file.attrdir();
    
    // collect the keys
    Enumeration keys=table.keys();

    // go through and replace the values with a string version
    String key=null;
    //System.out.println("getAttrList"); // REMOVE
    while(keys.hasMoreElements()){
      key=(String)keys.nextElement();
      table.put(key,getAttr(file,table,key));
      //System.out.println("  "+key+"="+table.get(key)); // REMOVE
    }

    return table;
  }

  /**
   * Method for converting the AttributeEntry objects into real
   * Atributes. The currently supported Attribute types are CHAR,
   * FLOAT, DOUBLE, INT and UINT.
   */
  private static Object getAttr(NeXusFileInterface file, Hashtable attrlist,
                       String key) throws NexusException, NullPointerException{
    // null pointers are bad
    if(file==null) throw new NullPointerException("null CNexusFile");
    if(attrlist==null) throw new NullPointerException("null Hashtable");
    if(key==null) throw new NullPointerException("null key");

    // get the current value
    Object val=attrlist.get(key);

    // if it is a String the conversion is already done
    if(val instanceof String)  return val;
    if(val instanceof Float)   return val;
    if(val instanceof Integer) return val;

    // initialize some quick arrays to convert with
    int type=((AttributeEntry)val).type;
    int[] lentype={((AttributeEntry)val).length,type};
    if(type==CHAR) lentype[0]++;

    // create the value array
    Object data=null;
    if(type==CHAR)
      data=new byte[lentype[0]];
    else if(type==USHORT || type==SHORT)
      data=getShortArray(lentype[0]);
    else if(type==UINT || type==INT)
      data=getIntArray(lentype[0]);
    else if(type==FLOAT)
      data=getFloatArray(lentype[0]);
    else if(type==DOUBLE)
      data=getDoubleArray(lentype[0]);
    else{
      return "";
    }

    // get the attribute value from the file
    file.getattr(key,data,lentype);

    // return a string version
    if(type==USHORT || type==SHORT){
      if(((short[])data).length==1)
        return new Short(((short[])data)[0]);
      else
        return data;
    }else if(type==UINT || type==INT){
      if(((int[])data).length==1)
        return new Integer(((int[])data)[0]);
      else
        return data;
    }else if(type==FLOAT){
      if(((float[])data).length==1)
        return new Float(((float[])data)[0]);
      else
        return data;
    }else if(type==DOUBLE){
      if(((double[])data).length==1)
        return new Double(((double[])data)[0]);
      else
        return data;
    }else if(type==CHAR){
      String result=new String((byte[])data);
      return result.substring(0,lentype[0]);
    }else{
      return new String("");
    }
  }

  public static Object getData(NeXusFileInterface file, String key)
                                   throws NexusException, NullPointerException{
    // null pointers are bad
    if(file==null) throw new NullPointerException("null CNexusFile");
    if(key==null) throw new NullPointerException("null key");

    file.opendata(key);
    //long zero=System.currentTimeMillis();
    Object result=getData(file);
    //System.out.println("TIME:"+(System.currentTimeMillis()-zero));
    file.closedata();

    return result;
  }

  /**
   * This is another version of the R.Mikkelson's code that I
   * understand better. When in doesn't work it calls that code.
   */
  private static Object getData(NeXusFileInterface file)
                                   throws NexusException, NullPointerException{
    // null pointers are bad
    if(file==null) throw new NullPointerException("null CNexusFile");

    // only allow data up to four dimensions
    int[] dim=new int[4];
    int[] ranktype=new int[2];

    // determine the dimensions and type
    file.getinfo(dim,ranktype);
    int rank=ranktype[0];
    int type=ranktype[1];

    // check that the rank makes sense
    if(rank<=0) return null;

    // determine the total length
    int tot_length=1;
    for( int i=0 ; i<rank ; i++ )
      tot_length*=dim[i];

    // check if this is a zero length array (mean trick)
    if(tot_length==0){
      return "";
    }

    Object array=null;

    // do something special for character arrays
    if(type==CHAR){
      array=getArray(type,tot_length);
      file.getdata(array);
      String result=new String((byte[])array);
      return result.substring(0,dim[0]);
    }

    // "Seemed like a good idea at the time..."  ;-)
    // read in smaller arrays
    // if(tot_length<MAX_BLOB_SIZE){
      // allocate full array to fill
      // array=getArray(type,tot_length);
      // read in the data b/c it is small
      // file.getdata(array);
    // }else
    if(rank==1){ // code for 1D arrays works
      // allocate full array to fill
      array=getArray(type,tot_length);

      int[] start={0};
      int[] end={Math.min(MAX_BLOB_SIZE,dim[0])};
      Object subarray;
      while(true){
        subarray=getArray(type,end);
        file.getslab(start,end,subarray);
        System.arraycopy(subarray,0,array,start[0],end[0]);
        if(end[0]>=dim[0])
          break;
        start[0]+=end[0];
        end[0]=Math.min(MAX_BLOB_SIZE,dim[0]-start[0]);
        if(end[0]<=0)
          break;
      }
    }else if(rank==2){ // 2-dimensional arrays
      // allocate arrays to use
      array=getArray(type,dim);

      // set up what to read
      int[] start={0,0};
      int[] end={1,dim[1]};

      for(int i=0 ; i<dim[0] ; i++ ){
        Object inner=Array.get(array,i);
        file.getslab(start,end,inner);
        start[0]++;
      }
    }else if(rank==3){ // 3-dimensional arrays
      // allocate arrays to use
      array=getArray(type,dim);
      
      // set up what to read
      int[] start={0,0,0};
      int[] end={1,1,dim[2]};

      for(int i=0 ; i<dim[0] ; i++ ){
        Object inner_i=Array.get(array,i);
        for( int j=0 ; j<dim[1] ; j++ ){
          Object inner_j=Array.get(inner_i,j);
          file.getslab(start,end,inner_j);
          start[1]++;
        }
        start[0]++;
        start[1]=0;
      }
    }else if(rank==4){ // 4-dimensional arrays
      // allocate arrays to use
      array=getArray(type,dim);
      
      // set up what to read
      int[] start={0,0,0,0};
      int[] end={1,1,1,dim[3]};

      for(int i=0 ; i<dim[0] ; i++ ){
        Object inner_i=Array.get(array,i);
        for( int j=0 ; j<dim[1] ; j++ ){
          Object inner_j=Array.get(inner_i,j);
          for( int k=0 ; k<dim[2] ; k++ ){
            Object inner_k=Array.get(inner_j,k);
            file.getslab(start,end,inner_k);
            start[2]++;
          }
          start[1]++;
          start[2]=0;
        }
        start[0]++;
        start[1]=0;
      }
    }else{ // give up and use Ruth's code
      //System.out.println("Ruth"); // REMOVE
      array=ruth_getData(file);
    }

    return array;
  }

  /**
   * This is an attempt to take R.Mikkelson's code for loading data.
   */
  private static Object ruth_getData(NeXusFileInterface file)
                                   throws NexusException, NullPointerException{
    //System.out.println("00:"); // REMOVE
    // null pointers are bad
    if(file==null) throw new NullPointerException("null CNexusFile");

    // only allow data up to four dimensions
    int[] dim=new int[4];
    int[] ranktype=new int[2];

    // determine the dimensions and type
    file.getinfo(dim,ranktype);
    int rank=ranktype[0];
    int type=ranktype[1];

    // check that the rank makes sense
    if(rank<=0) return null;

    // declare variables for grabbing sub-arrays
    int pos=-1; // position in dim where sub-elements start
    int step=-1; // number of blocks in that position that can be
                 // retrieved before getting above MAX_BLOB_SIZE
    int lenn=-1; // the number of array elements that will be
                 // retrieved in each blob

    //System.out.println("01:"); // REMOVE
    // initialize variables for grabbing sub-arrays
    if(dim[rank-1]>=MAX_BLOB_SIZE){
      pos=rank-2;
      step=1;
      lenn=1;
    }else if(rank==1){
      pos=-1;
      step=1;
      lenn=1;
    }else{
      int prod=dim[rank-1];
      pos=rank-1;
      lenn=dim[rank-1];
      while( (prod<MAX_BLOB_SIZE) && (pos>0) ){
        pos--;
        prod=prod*dim[pos];
        if(prod<MAX_BLOB_SIZE)
          lenn=prod;
      }
      if(prod>=MAX_BLOB_SIZE){
        prod=prod/dim[pos];
        step=MAX_BLOB_SIZE/prod;
        if(step<1) step=1;
      }else{
        step=dim[0];
        lenn=lenn/dim[0];
      }
    }

    //System.out.println("02:"); // REMOVE
    // set up the full array to put information into
    int tot_length=1;
    for( int i=0 ; i<rank ; i++ )
      tot_length*=dim[i];
    Object array=getArray(type,tot_length);

    //System.out.println("03:"); // REMOVE
    // set up information for stepping through the slabs
    int k_array=0;
    int[] start=new int[rank];
    int[] size=new int[rank];
    for( int i=0 ; i<rank ; i++ )
      start[i]=0;
    if(pos>=0){
      for( int i=0 ; i<pos ; i++ )
        size[i]=1;
      size[pos]=step;
      for( int i=pos+1 ; i<rank ; i++ )
        size[i]=dim[i];
    }else{
      for( int i=0 ; i<rank ; i++ )
        size[i]=dim[i];
      lenn=1;
    }
    Object subarray=getArray(type,tot_length);

    //System.out.println("04:"); // REMOVE

    // step through the subarrays and copy into full array
    while(true){
      //System.out.println(StringUtil.toString(start)+"->"+StringUtil.toString(size)); // REMOVE
      file.getslab(start,size,subarray); // ruth's line 606

      System.arraycopy(subarray,0,array,k_array,lenn*size[(int)Math.max(pos,0)]);
      k_array+=lenn*size[(int)Math.max(pos,0)];
      if(pos>=0){
        start[pos]+=size[pos];
        if(start[pos]>=dim[pos]){
          start[pos]=0;
          int i;
          for( i=pos-1 ; (i>=0)&&(start[i]+1>=dim[i]) ; i-- )
            start[i]=0;
          if(i<0)
            break;
          else if(i<pos)
            start[i]+=1;
        }
        size[pos]=(int)Math.min(step,dim[pos]-start[pos]);
      }else{ // case where all are done
        break;
      }
    }

    //System.out.println("05:"); // REMOVE

    return array;
  }

  private static int[] getDimensions(Object array){
    Vector vec_dim=new Vector();

    int length=Array.getLength(array);
    vec_dim.add(new Integer(length));
    Object inner=Array.get(array,0);
    while(inner.getClass().isArray()){
      length=Array.getLength(inner);
      vec_dim.add(new Integer(length));
      inner=Array.get(inner,0);
    }


    // convert the vector to an integer array
    int[] dim=new int[vec_dim.size()];
    for( int i=0 ; i<vec_dim.size() ; i++ )
      dim[i]=((Integer)vec_dim.elementAt(i)).intValue();

    return dim;
  }

  private static Object getArray(int type, int length){
    Object array=null;
    int[] dim={length};

    if(type==USHORT||type==SHORT)
      array=getIntArray(dim);
    else if(type==SH_USHORT || type==SH_SHORT)
      array=getIntArray(dim);
    else if(type==UINT||type==INT)
      array=getIntArray(dim);
    else if(type==FLOAT)
      array=getFloatArray(dim);
    else if(type==DOUBLE)
      array=getDoubleArray(dim);
    else if(type==CHAR)
      array=new byte[length];
    else
      throw new IllegalArgumentException("Cannot deal with type in getArray("
                                     +type+","+length+")");

    return array;
  }

  /**
   * Basically does a malloc for an n-dimensional array. The "type"
   * passed to this function must be one of those defined in NeXus.
   *
   * @param type the type as defined in NeXus API
   * @param dim the length of the array in each dimension. Zero
   * elements are ignored.
   */
  public static Object getArray(int type, int[] dim){
    Object array=null;

    if(type==USHORT||type==SHORT)
      array=getIntArray(dim);
    else if(type==SH_USHORT || type==SH_SHORT)
      array=getIntArray(dim);
    else if(type==UINT||type==INT)
      array=getIntArray(dim);
    else if(type==FLOAT)
      array=getFloatArray(dim);
    else if(type==DOUBLE)
      array=getDoubleArray(dim);
    else if(type==CHAR)
      array=new byte[dim[0]];

    return array;
  }

  /**
   * Basically does a malloc for an n-dimensional integer array.
   *
   * @param dim the length of the array in each dimension. Zero
   * elements are ignored.
   */
  public static Object getIntArray(int[] dim){
    // get the dimension of the array to allocate
    int size=dim.length;
    for(int i=size-1 ; i>0 ; i-- ){
      if(dim[i]<=0)
        size--;
      else
        break;
    }

    // allocate the array
    if(size==1)
      return new int[dim[0]];
    else if(size==2)
      return new int[dim[0]][dim[1]];
    else if(size==3)
      return new int[dim[0]][dim[1]][dim[2]];
    else if(size==4)
      return new int[dim[0]][dim[1]][dim[2]][dim[3]];
    else
      return null;
  }

  public static Object getShortArray(int[] dim){
    // get the dimension of the array to allocate
    int size=dim.length;
    for(int i=size-1 ; i>0 ; i-- ){
      if(dim[i]<=0)
        size--;
      else
        break;
    }

    // allocate the array
    if(size==1)
      return new short[dim[0]];
    else if(size==2)
      return new short[dim[0]][dim[1]];
    else if(size==3)
      return new short[dim[0]][dim[1]][dim[2]];
    else if(size==4)
      return new short[dim[0]][dim[1]][dim[2]][dim[3]];
    else
      return null;
  }

  public static Object getFloatArray(int[] dim){
    // get the dimension of the array to allocate
    int size=dim.length;
    for(int i=size-1 ; i>0 ; i-- ){
      if(dim[i]<=0)
        size--;
      else
        break;
    }

    // allocate the array
    if(size==1)
      return new float[dim[0]];
    else if(size==2)
      return new float[dim[0]][dim[1]];
    else if(size==3)
      return new float[dim[0]][dim[1]][dim[2]];
    else if(size==4)
      return new float[dim[0]][dim[1]][dim[2]][dim[3]];
    else
      return null;
  }

  public static Object getDoubleArray(int[] dim){
    // get the dimension of the array to allocate
    int size=dim.length;
    for(int i=size-1 ; i>0 ; i-- ){
      if(dim[i]<=0)
        size--;
      else
        break;
    }

    // allocate the array
    if(size==1)
      return new double[dim[0]];
    else if(size==2)
      return new double[dim[0]][dim[1]];
    else if(size==3)
      return new double[dim[0]][dim[1]][dim[2]];
    else if(size==4)
      return new double[dim[0]][dim[1]][dim[2]][dim[3]];
    else
      return null;
  }

  public static short[] getShortArray(int length){
    int[] dim={length};
    return (short[])getShortArray(dim);
  }

  public static int[] getIntArray(int length){
    int[] dim={length};
    return (int[])getIntArray(dim);
  }

  public static float[] getFloatArray(int length){
    int[] dim={length};
    return (float[])getFloatArray(dim);
  }

  public static double[] getDoubleArray(int length){
    int[] dim={length};
    return (double[])getDoubleArray(dim);
  }

  public static void printArray(Object array){
    int maxmax=10;
    if(array instanceof float[]){
      int maxI=((float[])array).length;
      if(maxI>maxmax) maxI=maxmax;
      System.out.print("[");
      for( int i=0 ; i<maxI ; i++ ){
        System.out.print(((float[])array)[i]);
        if(i<maxI-1)
          System.out.print(", ");
        else if(i==maxI-1)
          System.out.println(", ... ]");
      }
    }else if(array instanceof int[]){
      int maxI=((int[])array).length;
      if(maxI>maxmax) maxI=maxmax;
      System.out.print("[");
      for( int i=0 ; i<maxI ; i++ ){
        System.out.print(((int[])array)[i]);
        if(i<maxI-1)
          System.out.print(", ");
        else if(i==maxI-1)
          System.out.println(", ... ]");
      }
    }else if(array instanceof float[][]){
      int maxI=((float[][])array).length;
      if(maxI>maxmax) maxI=maxmax;
      System.out.print("[");
      for( int i=0 ; i<maxI ; i++ ){
        printArray(((float[][])array)[i]);
        if(i<maxI-1)
          System.out.print(", ");
        else if(i==maxI-1)
          System.out.println(", ... ]");
      }
    }else if(array instanceof int[][]){
      int maxI=((int[][])array).length;
      if(maxI>maxmax) maxI=maxmax;
      System.out.print("[");
      for( int i=0 ; i<maxI ; i++ ){
        printArray(((int[][])array)[i]);
        if(i<maxI-1)
          System.out.print(", ");
        else if(i==maxI-1)
          System.out.println(", ... ]");
      }
    }else{
      System.out.println("NOT PRINTING");
    }
  }

  public static String getNXfileSummary(String filename){
    filename=StringUtil.setForwardSlash(filename);
    String result=null;
    NeXusFileInterface file=null;

    String fname = null;
    String date  = null;
    String name  = null;
    String title = null;

    int index=0;

    try{
      Hashtable attrlist=null;
      Hashtable direc=null;
      Hashtable inner_direc=null;
      Enumeration keys=null;
      Enumeration inner_keys=null;


      file=new NexusFile(filename,NexusFile.NXACC_READ);
      attrlist=getAttrList(file);

      // create some defaults
      String defFile=filename;
      String defDate=(String)attrlist.get("file_time");
      String defName=(String)attrlist.get("user");
      String defTitl="";

      // useful local variables
      String key=null;
      String val=null;

      // find the right group to be in
      direc=file.groupdir();
      keys=direc.keys();
      while(keys.hasMoreElements()){
        key=(String)keys.nextElement();
        if( "NXentry".equals((String)direc.get(key)) ){
          file.opengroup(key,"NXentry");
          inner_direc=file.groupdir();
          if( inner_direc.get("title")!=null )
            break;
          else
            file.closegroup();
        }else{
          key=null;
        }
      }

      // now start looking for the good stuff
      attrlist=file.groupdir();
      // first the date;
      val=(String)attrlist.get("start_time");
      if(val!=null && val.equals("SDS"))
        date=(String)getData(file,"start_time");
      // next the user name
      keys=attrlist.keys();
      while(keys.hasMoreElements()){
        key=(String)keys.nextElement();
        if("NXuser".equals(attrlist.get(key))){
          file.opengroup(key,"NXuser");
          name=(String)getData(file,"name");
          file.closegroup();
          break;
        }
      }
      // finally the title
      val=(String)attrlist.get("title");
      if(val!=null && val.equals("SDS"))
        title=(String)getData(file,"title");

      // use the defaults if there isn't another choice
      if(fname==null || fname.length()==0) fname=defFile;
      if(date==null  || date.length()==0)  date=defDate;
      if(name==null  || name.length()==0)  name=defName;
      if(title==null || title.length()==0) title=defTitl;

      // chop down the filename
      fname=StringUtil.setForwardSlash(fname);
      index=fname.lastIndexOf("/");
      if(index>=0) fname=fname.substring(index+1);
      // chop down the date
      index=date.indexOf(" ");
      if(index>=0) date=date.substring(0,index);

      // format this stuff
      StringBuffer sb=new StringBuffer(80);
      sb.append(fname+"                              ");
      sb.delete(30,sb.length());
      sb.append(date+" ");
      sb.append(name+"                    ");
      sb.delete(55,sb.length());
      sb.append(title+"                                   ");
      result=sb.substring(0,80);
    }catch(NexusException e){
      System.out.println("ENCOUNTERED EXCEPTION"+e.getMessage());
      // let it drop on the floor
    }

    try{
      if(file!=null) file.finalize(); // close the file
    }catch(Throwable e){
      // let it drop on the floor
    }

    return result;
  }
}
