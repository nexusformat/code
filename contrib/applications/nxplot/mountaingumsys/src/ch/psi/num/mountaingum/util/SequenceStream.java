/**
 * This is a stream wrapper for a sequence
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.util;

import ch.psi.num.mountaingum.func.IStream;

public class SequenceStream implements IStream {
	private int count, length;
	private Sequence s;
	
	public SequenceStream(Sequence s){
		this.s = s;
		count = 0;
		length = s.length();
	}
	public Object next() {
		if(count < length){
			Object o = s.itemAt(count);
			count++;
			return o;
		}
		return null;
	}
}
