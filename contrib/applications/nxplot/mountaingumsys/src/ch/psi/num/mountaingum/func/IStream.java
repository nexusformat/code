/**
 * This is an interface for streams of objects.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.func;

public interface IStream {
	/**
	 * return next object or null when exhausted
	 * @return an object or null when exhausted.
	 */
	public Object next();
}
