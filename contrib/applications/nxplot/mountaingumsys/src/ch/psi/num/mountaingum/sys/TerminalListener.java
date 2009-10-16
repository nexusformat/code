/**
 * This is an interface for an object to listen for output from 
 * a command line application.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.sys;

import java.util.List;

public interface TerminalListener {
	/**
	 * handle output from the CLI
	 * @param lines is a list of lines. 
	 */
	public void handleOutput(List lines);
}
