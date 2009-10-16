/**
 * This is a dynamic array for long values. Needed to store time stamps
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.util;

public class DynamicLongArray {
	protected long data[];
	protected int capacity, length;

	public DynamicLongArray(){
		capacity = 128;
		length = 0;
		data = new long[128];
	}
	public void reset(){
		length = 0;
	}
	public void append(long i){
		data[length] = i;
		length++;
		if(length >= capacity - 1){
			capacity = capacity*2;
			data = ArrayTools.resize(data, capacity);
		}
	}
	public void append(long d[]){
		if(d.length + length >= capacity - 1){
			capacity = d.length + length*2;
			data = ArrayTools.resize(data, capacity);
		}
		System.arraycopy(d, 0, data, length, d.length);
		length += d.length;
	}
	public int getLength(){
		return length;
	}
	public String toText(){
		StringBuffer stb = new StringBuffer();
		for(int i = 0; i < length; i++) {
			stb.append(data[i]);
			stb.append(' ');
		}
		return stb.toString();
	}
	public void setFromText(String txt){
		data = ArrayTools.splitLongs(txt, "\\p{Space}+");
		length = data.length;
		capacity = length;
	}
	public String toString(){
		return toText();
	}
	public long[] getData(){
		long r[];
		r = new long[length];
		System.arraycopy(data,0 , r, 0, length);
		return r;
	}
	public long getValue(int idx){
		return data[idx];
	}
	/**
	 * access the raw array: take extra care! The array is certainly longer 
	 * then length but only length values are defined.
	 * Changes to the array which would affect length are not properly honoured
	 * @return a dangerous pointer to the raw data array. 
	 */
	public long[] getRawArray(){
		return data;
	}
}
