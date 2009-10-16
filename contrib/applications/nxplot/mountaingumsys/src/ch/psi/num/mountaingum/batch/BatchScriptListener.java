/**
 * This is a listener for events from processing scripted batch 
 * files.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.batch;

public interface BatchScriptListener {
	/**
	 * some log data
	 * @param txt The log data;
	 */
	public void log(String txt);
	/**
	 * change the text range which is currently executing
	 * @param start of the range
	 * @param end of the range
	 */
	public void range(int start, int end);
	/**
	 * inidication that the script terminated
	 */
	public void terminated();
}
