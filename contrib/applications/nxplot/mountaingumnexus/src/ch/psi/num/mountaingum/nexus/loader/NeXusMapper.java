/**
 * This is a the interface to be implemented by any NeXus data file 
 * mapper. The mappers task is to map nodes in the NeXus files 
 * hierarchy to nodes in the tree structure for mountaingum.
 * 
 * copyright GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.nexus.loader;


import ch.psi.num.mountaingum.tree.TreeNode;

public interface NeXusMapper {
	/**
	 * Tests if this mapper can be applied to the 
	 * give NeXus file.
	 * @param nf The NeXus file to test. 
	 * @return true when this mapper is applicable, 
	 * false else. 
	 */
	public boolean isApplicable(FlatNexusFile nf);
	/**
	 * The actual payload method. This map has to analyze 
	 * the item under nxpath and install it into tree, the 
	 * root of which is given by root. The caller must assume that 
	 * map jumps up and down in the file to its liking. 
	 * @param root This is the root node of the tree to 
	 * generate.
	 * @param nxpath The path of the item in the NeXus file 
	 * to map.
	 */
	public void map(TreeNode root, FlatNexusFile nf, String nxclass, String nxpath);
	/**
	 * Reset the internal data structures of this mapper to be 
	 * able to handle another file.
	 */
	public void reset();
}
