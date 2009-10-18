/**
 * This is a the interface to be implemented by any NeXus data file 
 * mapper. The mapper's task is to create instrument specific graphics 
 * nodes in the tree hierarchy or do other customisations before the tree
 * is displayed to the user.
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
	 */
	public void transform(TreeNode root, FlatNexusFile nf);

}
