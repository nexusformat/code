/**
 * This is a stream to iterate over a Java Enumeration
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.func;
import java.util.Enumeration;

public class EnumerationStream implements IStream {
	private Enumeration eno;
	
	public EnumerationStream(Enumeration eno){
		this.eno = eno;
	}
	public Object next() {
		if(eno.hasMoreElements()){
			return eno.nextElement();
		} else {
			return null;
		}
	}
}
