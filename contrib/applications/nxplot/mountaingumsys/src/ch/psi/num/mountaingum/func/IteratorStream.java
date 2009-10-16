/**
 * This packages any of the standard Java collection iterators into a
 * stream interface
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.func;
import java.util.Collection;
import java.util.Iterator;

public class IteratorStream implements IStream {

	protected Iterator it;
	
	public IteratorStream(Collection c){
		it = c.iterator();
	}
	public Object next() {
		if(it.hasNext()){
			return it.next();
		}
		return null;
	}
}
