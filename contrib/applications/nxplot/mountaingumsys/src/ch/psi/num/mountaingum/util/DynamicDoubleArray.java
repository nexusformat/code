/**
 * A dynamic array of doubles. As Java does not have such an animal and using the 
 * object based standard collection classes from Java would lead to the creation
 * of many small objects. Which is inefficient.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.util;

public class DynamicDoubleArray {
	protected double data[];
	protected int capacity, length;

	public DynamicDoubleArray(){
		capacity = 8;
		length = 0;
		data = new double[capacity];
	}
	public void reset(){
		length = 0;
	}
	public void append(double i){
		data[length] = i;
		length++;
		if(length >= capacity - 2){
			capacity = capacity*2;
			data = ArrayTools.resize(data, capacity);
		}
	}
	public void append(double d[]){
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
	public String toString(){
		return toText();
	}
	public void setFromText(String txt){
		data = ArrayTools.splitDoubles(txt, "\\p{Space}+");
		length = data.length;
		capacity = length;
	}
	public double[] getData(){
		double r[];
		r = new double[length];
		System.arraycopy(data,0 , r, 0, length);
		return r;
	}
	public double getValue(int idx){
		return data[idx];
	}
	/**
	 * access the raw array: take extra care! The array is certainly longer 
	 * the length but onbly length values are defined.
	 * Changes to the array which would affect length are not properly hounoured
	 * @return a dangerous pointer to the raw data array. 
	 */
	public double[] getRawArray(){
		return data;
	}
	protected void finalize() throws Throwable {
		super.finalize();
		data = null;
	}
}
