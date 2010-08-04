/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.nexusformat.nxvalidate;

/**
 *
 * @author ser
 */
public class OSValidator{

	public boolean isWindows(){

		String os = System.getProperty("os.name").toLowerCase();
		//windows
	    return (os.indexOf( "win" ) >= 0);

	}

	public boolean isMac(){

		String os = System.getProperty("os.name").toLowerCase();
		//Mac
	    return (os.indexOf( "mac" ) >= 0);

	}

	public boolean isUnix(){

		String os = System.getProperty("os.name").toLowerCase();
		//linux or unix
	    return (os.indexOf( "nix") >=0 || os.indexOf( "nux") >=0);

	}
}
