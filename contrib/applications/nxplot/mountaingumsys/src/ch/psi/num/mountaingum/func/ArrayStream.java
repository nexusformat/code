/**
 * This is an aray of objects based stream
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.func;

public class ArrayStream implements IStream {

	private Object obj[];
	private int count;
	public ArrayStream(Object o[]){
		obj = o;
		count = 0;
	}
	public Object next() {
		Object o;
		if(count < obj.length ){
			o = obj[count];
			count++;
			return o;
		}
		return null;
	}

}
