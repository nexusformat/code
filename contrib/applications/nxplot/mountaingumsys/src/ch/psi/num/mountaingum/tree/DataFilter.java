/**
 * This is an interface for all data filters. A data filter transforms 
 * the data read into something else.
 * 
 * Mark Koennecke, May 2008
 */
package ch.psi.num.mountaingum.tree;


public interface DataFilter {
	public void process(TreeNode n) throws Exception;
}
