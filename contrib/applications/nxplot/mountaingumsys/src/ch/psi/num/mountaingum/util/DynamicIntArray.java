/**
 * A dynamic array of ints. As Java does not have such an animal and using the 
 * object based standard collection classes from Java would lead to the creation
 * of many small objects. Which is inefficient.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.util;

public class DynamicIntArray {
	protected int data[];
	protected int capacity, length;

	public DynamicIntArray(){
		capacity = 128;
		length = 0;
		data = new int[128];
	}
	public void reset(){
		length = 0;
	}
	public void append(int i){
		data[length] = i;
		length++;
		if(length >= capacity - 1){
			capacity = capacity*2;
			int[] duta = ArrayTools.resize(data, capacity);
			data = null;
			data = duta;
		}
	}
	public void append(int d[]){
		int duta[];
		if(d.length + length >= capacity - 1){
			capacity = d.length + length*2;
			duta = ArrayTools.resize(data, capacity);
			data = null;
			data = duta;
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
		data = ArrayTools.splitInts(txt, "\\p{Space}+");
		length = data.length;
		capacity = length;
	}
	public String toString(){
		return toText();
	}
	public int[] getData(){
		int r[];
		r = new int[length];
		System.arraycopy(data,0 , r, 0, length);
		return r;
	}
	public int getValue(int idx){
		return data[idx];
	}
	/**
	 * access the raw array: take extra care! The array is certainly longer 
	 * then length but only length values are defined.
	 * Changes to the array which would affect length are not properly honoured
	 * @return a dangerous pointer to the raw data array. 
	 */
	public int[] getRawArray(){
		return data;
	}
}
