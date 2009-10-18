/**
 * This is the default implementation for a NeXus file 
 * mapper. It will copy all the normal data elements 
 * into something looking like the file hierarchy. 
 * Larger SDS's get a special treatment: the default 
 * mapper tries to map them into the graphics hierarchy.
 * In order to avoid duplication, the default mapper keeps 
 * track of what it has mapped already and ignores items 
 * which have already been mapped. Or which are linked to 
 * something already mapped. 
 * 
 * Other mappers will probably override this mapper to treat 
 * some special paths differently. 
 * 
 *   copyright: GPL
 *   
 *   Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ch.psi.num.mountaingum.tree.TreeNode;

public class NexusMapperDemo implements NeXusMapper {
	public void transform(TreeNode root, FlatNexusFile nf) {
		if (false)
		root.insertNode(TreeNode.APPEND, new TreeNode(root, "DemoMapperNode"));
	}
}
