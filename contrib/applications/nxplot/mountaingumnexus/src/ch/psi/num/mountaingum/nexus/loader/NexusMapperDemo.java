/**
 * This is a demonstration mapper Is useless 
 *   copyright: GPL
 *   
 *   Tobias Richter, October 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ch.psi.num.mountaingum.tree.TreeNode;

public class NexusMapperDemo implements NeXusMapper {
	public void transform(TreeNode root, FlatNexusFile nf, NexusLoader nl) {
		if (false)
		root.insertNode(TreeNode.APPEND, new TreeNode(root, "DemoMapperNode"));
	}
}
