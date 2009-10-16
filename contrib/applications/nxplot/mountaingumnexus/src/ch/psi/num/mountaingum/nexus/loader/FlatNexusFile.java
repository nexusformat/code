/**
 * This is a derived class of NexusFile which uses the protected 
 * native getdata function and HDFnative methods to get plain 
 * 1D array data of various data types. this is what I need for 
 * GTSE.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ncsa.hdf.hdflib.HDFNativeData;

import org.nexusformat.NexusException;
import org.nexusformat.NexusFile;

public class FlatNexusFile extends NexusFile {

	public FlatNexusFile(String arg0, int arg1) throws NexusException {
		super(arg0, arg1);
	}

	protected int getTotalLength(){
		int dim[], info[], length = 1;
		dim = new int[32];
		info = new int[2];
		
		try{
			getinfo(dim,info);
		}catch(NexusException ne){
			return 0;
		}
		
		for(int i = 0; i < info[0]; i++){
			length *= dim[i];
		}
		
		return length;
	}
	public int[] getintdata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length*4];
		nxgetdata(handle,bdata);
		return HDFNativeData.byteToInt(bdata);
	}
	public short[] getshortdata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length*2];
		nxgetdata(handle,bdata);
		return HDFNativeData.byteToShort(bdata);
	}
	public long[] getlongdata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length*8];
		nxgetdata(handle,bdata);
		return HDFNativeData.byteToLong(bdata);
	}
	public float[] getfloatdata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length*4];
		nxgetdata(handle,bdata);
		return HDFNativeData.byteToFloat(bdata);
	}
	public double[] getdoubledata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length*8];
		nxgetdata(handle,bdata);
		return HDFNativeData.byteToDouble(bdata);
	}
	public String gettextdata(){
		int length = getTotalLength();
		byte bdata[] = new byte[length];
		nxgetdata(handle,bdata);
		return  new String(bdata);
	}

	public byte[] getrawslab(int start[], int size[], int type){
		int i, length = 1;
		for(i = 0; i < size.length; i++){
			length *= size[i]; 
		}
		switch(type){
			case NexusFile.NX_CHAR:
			case NexusFile.NX_INT8:
			case NexusFile.NX_UINT8:
				break;
			case NexusFile.NX_INT16:
			case NexusFile.NX_UINT16:
				length *= 2;
				break;
			case NexusFile.NX_INT32:
			case NexusFile.NX_UINT32:
			case NexusFile.NX_FLOAT32:
				length *= 4;
				break;
			case NexusFile.NX_INT64:	
			case NexusFile.NX_UINT64:
			case NexusFile.NX_FLOAT64:
				length *= 8;
		}
		byte result[] = new byte[length];
		nxgetslab(handle,start,size,result);
		return result;
	}
}
