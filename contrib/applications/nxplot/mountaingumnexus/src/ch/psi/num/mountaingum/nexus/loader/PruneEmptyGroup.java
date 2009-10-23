/**
 * This is a NeXusMapper which removes empty groups
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;

public class PruneEmptyGroup implements NeXusMapper {

	private class TestFunc implements IFunc {
		public Object apply(Object o) {
			TreeNode n = (TreeNode)o;
			if( ! (o instanceof ParameterNode) && n.countChildren() < 1){
				n.getParent().deleteChild(n);
			}
			return null;
		}
		
	}
	public void transform(TreeNode root, FlatNexusFile nf, NexusLoader nl) {
		IStream s = root.getTreeStream();
		FuncUtil.map(s, new TestFunc());
	}

}
