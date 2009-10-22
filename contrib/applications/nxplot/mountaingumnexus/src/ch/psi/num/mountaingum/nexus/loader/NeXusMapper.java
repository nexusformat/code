/**
 * This is a the interface to be implemented by any NeXus data file 
 * mapper. The mappers task is to edit the raw hierarchy as created 
 * by the NexusLoader in order to adapt it to different needs. 
 * 
 * 
 * copyright GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ch.psi.num.mountaingum.tree.TreeNode;

public interface NeXusMapper {

	/**
	 * This method allows the mapper to modify the tree at will.
	 * Usually the mapper may want to check if it can be sensibly applied and just 
	 * return otherwise.
	 * 
	 * @param root the populated tree
	 * @param nf a handle to the file if needed
	 * @param nl The NexusLoader for support functions
	 */
	public void transform(TreeNode root, FlatNexusFile nf, NexusLoader nl);

}
