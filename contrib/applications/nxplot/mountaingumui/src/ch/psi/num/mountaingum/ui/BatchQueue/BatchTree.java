/**
 * This is a specialized version of the Tree viewer to use in the batch 
 * perspective. It mostly changes what happens if one double clicks a 
 * tree node.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, November 2008
 */
package ch.psi.num.mountaingum.ui.BatchQueue;

import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.TreeViewer.TreeView;

public class BatchTree extends TreeView {
	public static String ID = "mountaingumui.BatchTree";

	protected void configureMouseListener(){
		treeViewer.getTree().addMouseListener(new InvokeBatchEdit());		
	}
	
	protected class InvokeBatchEdit implements MouseListener{
	    protected ParameterNode editNode;
	    protected int column;
		
		public void mouseDoubleClick(MouseEvent e ) {
			TreeNode node, batchNode;
			BatchAdapter ba;
			Point pm = new Point(e.x, e.y);
			Tree tree = treeViewer.getTree();
			TreeItem it = tree.getItem(pm);
			Object o = it.getData();
			node = (TreeNode)o;
			ba= SysRegistry.getBatchAdapter();
			batchNode = ba.cloneBatchNode(node);
			ba.append(batchNode);
		}
		public void mouseDown(MouseEvent e) {
			// Nothing to do
		}
		public void mouseUp(MouseEvent e) {
			// Nothing to do
		}
	}
}
