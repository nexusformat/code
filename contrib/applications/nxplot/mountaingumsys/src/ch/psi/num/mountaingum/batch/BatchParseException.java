/**
 * An exception top be thrown when parsing a node from edited data fails. Currently 
 * this jusst identifies the type. More things may be added later on.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.batch;

public class BatchParseException extends Exception {
	public BatchParseException(String txt){
		super(txt);
	}
}
