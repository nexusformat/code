/**
 * This is value class for long ints.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.util.DynamicLongArray;

public class LongValue extends NodeValue {
	protected DynamicLongArray data;
	protected boolean arrayDirty;
	protected boolean textDirty;
	
	public LongValue(int rank, int dim[]){
		this.type = INT;
		this.rank = rank;
		this.dim = dim;
		textData = new String("0");
		data = new DynamicLongArray();
		textDirty = false;
	}

	public void setFromString(String data){
		super.setFromString(data);
		arrayDirty = true;
		textDirty = false;
	}
	public String toString(){
		if(textDirty){
			textData = data.toString();
			textDirty = false;
		}
		return textData;
	}
	public void setDim(int dim[]){
		super.setDim(dim);
		arrayDirty = true;
		textDirty = true;
	}
	public int getLength(){
		return data.getLength();
	}
	public long[] getData(){
		if(arrayDirty){
			data.setFromText(textData.trim());
			arrayDirty = false;
		}
		return data.getData();
	}
	public long getValue(int idx){
		return data.getValue(idx);
	}
	public void setData(long data[]){
		this.data.reset();
		this.data.append(data);
		textDirty = true;
	}
	public void append(long i){
		data.append(i);
		textDirty = true;
	}
	public void append(double d){
		data.append((long)Math.abs(d));
		textDirty = true;
	}
	public void append(int i){
		data.append((long)i);
		textDirty = true;
	}
	public long[] getRawData(){
		return data.getRawArray();
	}
	
	public void reset() {
		super.reset();
		data.reset();
	}
}
