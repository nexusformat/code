/**
 * This is an interface describing a logger. Clients can implement 
 * different implementations in order to realize different logging 
 * strategies.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.sys;

public interface LoggerInterface {
	public void trace(String txt);
	public void debug(String txt);
	public void info(String txt);
	public void warn(String txt);
	public void error(String txt);
	public void sent(String txt);
	public void received(String txt);
	public void exception(Exception eva);
}
