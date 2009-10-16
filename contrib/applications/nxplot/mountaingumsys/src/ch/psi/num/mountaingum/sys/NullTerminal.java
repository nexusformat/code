/**
 * This is the default implementation of a terminal interface
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.sys;

import java.util.ArrayList;
import java.util.List;

public class NullTerminal implements TerminalInterface {

	public NullTerminal(){
		l = null;
	}
	protected TerminalListener l;
	public TerminalListener getTerminalListener() {
		return l;
	}

	public void interrupt() {
		reply("ERROR: Not connected, interrupt annihilated");
	}

	public void sendCommand(String command) {
		reply("ERROR: not connected, command " + command + " annihilated");
	}

	public void setTerminalListener(TerminalListener l) {
		this.l = l;
	}
	private void reply(String txt){
		if(l != null){
			List ll = new ArrayList();
			ll.add(txt);
			l.handleOutput(ll);
		}
	}
}
