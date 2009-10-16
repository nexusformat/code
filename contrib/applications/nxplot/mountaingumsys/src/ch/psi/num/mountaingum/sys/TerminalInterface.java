/**
 * This is an interface to an underlying text based command system
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.sys;

public interface TerminalInterface {
	/**
	 * send a command to the CLI system
	 * @param command The command to send
	 */
	public void sendCommand(String command);
	/**
	 * interrupt what is going on in the CLI
	 */
	public void interrupt();
	/**
	 * set a listener for output from the CLI
	 * @param l The listener to set.
	 */
	public void setTerminalListener(TerminalListener l);
	/**
	 * get the current terminal listener
	 * @return The current TerminalListener
	 */
	public TerminalListener getTerminalListener();
}
