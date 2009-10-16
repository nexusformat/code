/**
 * This is the class representing the value of a parameter node 
 * in MountainGum. Incidentally it is also the node for text data.
 * Further subclasses support integer and float data.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

public class NodeValue {
	/**
	 * type codes
	 */
	public final static int TEXT  = 0;
	public final static int INT   = 1;
	public final static int FLOAT = 2;
	public final static int LONG  = 3;
	
	protected int type;
	protected int rank;
	protected int dim[];
	protected String textData;
	
	public NodeValue(){
		type = TEXT;
		rank = 1;
		dim = new int[1];
		dim[0] = 0;
	}
	public NodeValue(int type, int rank, int dim[]){
		this.type = type;
		this.rank = rank;
		this.dim = dim;
		textData = new String("Undefined");
	}

	public String toString(){
		return textData;
	}
	
	public void setFromString(String data){
		textData = data;
	}

	public int getType() {
		return type;
	}

	public int getRank() {
		return rank;
	}

	public int[] getDim() {
		return dim;
	}

	public void setDim(int[] dim) {
		this.dim = dim;
	}
	protected  boolean testWhiteSpace(String text){
		int i;
		
		for(i = 0; i < text.length(); i++){
			if(!Character.isWhitespace(text.charAt(i))){
				return false;
			}
		}
		return true;
	}
	public int getLength(){
		return 1;
	}
	public void reset(){
		textData = "Undefined";
	}
	/**
	 * These appends make most sense for numbers, not for strings, what this 
	 * really is. 
	 * @param i
	 */
	public void append(int i){
		textData = Integer.toString(i);
	}
	public void append(double i){
		textData = Double.toString(i);
	}
	public void append(long i){
		textData = Long.toString(i);
	}
}
