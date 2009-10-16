/**
 * This is a node representing integer data. We do some lazy evaluation 
 * here: i.e. data gets converted from text to array or from array to text
 * only when requested. Please note that client code is responsible for 
 * adapting dimensions for the case of variable length arrays.  
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;
import ch.psi.num.mountaingum.util.DynamicIntArray;

public class IntValue extends NodeValue {
	protected DynamicIntArray data;
	protected boolean arrayDirty;
	protected boolean textDirty;
	
	public IntValue(int rank, int dim[]){
		this.type = INT;
		this.rank = rank;
		this.dim = dim;
		textData = new String("0");
		data = new DynamicIntArray();
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
	public int[] getData(){
		if(arrayDirty){
			data.setFromText(textData.trim());
			arrayDirty = false;
		}
		return data.getData();
	}
	public int getValue(int idx){
		if(arrayDirty){
			data.setFromText(textData.trim());
			arrayDirty = false;
		}
		return data.getValue(idx);
	}
	public void setData(int data[]){
		this.data.reset();
		this.data.append(data);
		textDirty = true;
	}
	public void append(int i){
		data.append(i);
		textDirty = true;
	}
	public void append(double d){
		data.append((int)Math.abs(d));
		textDirty = true;
	}
	public void append(long i){
		data.append((int)i);
		textDirty = true;
	}
	
	public void reset() {
		super.reset();
		data.reset();
	}
	public int[] getRawData(){
		return data.getRawArray();
	}
}
