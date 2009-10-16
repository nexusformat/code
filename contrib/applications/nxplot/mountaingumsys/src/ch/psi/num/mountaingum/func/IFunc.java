/**
 * Interface for a function acting upon an object
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.func;

public interface IFunc {
	/**
	 * apply the function to 
	 * @param o The argument object
	 * @return null or a result object
	 */
	public Object apply(Object o);
}
