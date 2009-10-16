/**
 * This is the interface to be implemented by tree nodes in the batch queue. I 
 * separated this into an interface in order to allow batch tree nodes to 
 * inherit from real nodes as they might and will share the command formatting
 * code in the SICS implementation.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.batch;

public interface BatchNode {
	/**
	 * Is this node capable to handle textual edits of command strings?
	 * If yes, parse() must do sensible things.
	 * @return true or false
	 */
	public boolean canEdit();
	/**
	 * getDescription returns a textual description of the batch node entry
	 * @return  A string for displaying info about this entry.
	 */
	public String getDescription();
	/**
	 * After text editing, parse the resulting string into the 
	 * node. 
	 * @param txt The text to parse. 
	 */
	public void parse(String txt)throws BatchParseException;
}
