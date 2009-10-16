/**
 * This is a node representing double data. We do some lazy evaluation 
 * here: i.e. data gets converted from text to array or from array to text
 * only when requested. Please note that client code is responsible for 
 * adapting dimensions for the case of variable length arrays.  
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;
import ch.psi.num.mountaingum.util.DynamicDoubleArray;

public class DoubleValue extends NodeValue {
	protected DynamicDoubleArray data;
	protected boolean arrayDirty;
	protected boolean textDirty;
	
	public DoubleValue(int rank, int dim[]){
		this.type = FLOAT;
		this.rank = rank;
		this.dim = dim;
		textData = new String("0");
		data = new DynamicDoubleArray();
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
	public double[] getData(){
		if(arrayDirty){
			data.setFromText(textData.trim());
			arrayDirty = false;
		}
		return data.getData();
	}
	public double getValue(int idx){
		if(arrayDirty){
			data.setFromText(textData.trim());
			arrayDirty = false;
		}
		return data.getValue(idx);
	}
	public void setData(double data[]){
		this.data.reset();
		this.data.append(data);
		textDirty = true;
	}
	public void append(double d){
		data.append(d);
		textDirty = true;
	}
	public void append(int i){
		data.append((double)i);
		textDirty = true;
	}
	public double[] getRawData(){
		return data.getRawArray();
	}
	public void reset() {
		super.reset();
		data.reset();
	}
}
