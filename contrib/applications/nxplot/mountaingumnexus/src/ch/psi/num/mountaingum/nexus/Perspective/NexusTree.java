/**
 * This is derived class of BaseTreeView which locates
 * a TreeEditorView instead of a EditroView. All in an ongoing 
 * effort not do display interrupt buttons in the NeXus perspective
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.Perspective;

import java.util.Comparator;

import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;
import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.nexus.labelprovider.IconTreeLabelProvider;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.TreeViewer.BaseTreeView;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class NexusTree extends BaseTreeView {
	public static final String ID = "mountaingumnexus.NexusTree";

	protected EditorView getEditorView() {
		if(eva == null){
			eva = (EditorView)RCPUtil.findView(TreeEditorView.ID,"1");
		}
		return eva;
	}
	
	public TreeNode getTree(){
		return root;
	}

	@Override
	public void createPartControl(Composite parent) {
		super.createPartControl(parent);
		treeViewer.setLabelProvider(new IconTreeLabelProvider());
		ViewerComparator comparator = new ViewerComparator(new Comparator<Object>() {
			public int compare(Object o1, Object o2) {
				TreeNode node1;
				TreeNode node2;
				if (o1 instanceof TreeNode) {
					node1 = (TreeNode) o1;
				} else return -1;
				if (o2 instanceof TreeNode) {
					node2 = (TreeNode) o2;
				} else return 1;
				
				String type1 = node1.getProperty("type");
				String type2 = node2.getProperty("type");
				String name1 = node1.getProperty("name");
				String name2 = node2.getProperty("name");				
				if (type1 == null && type2 == null) return 0;
				if (type1 == null) return 1;
				if (type2 == null) return -1;
				if (type1.equals(type2)) 
					return name1.compareTo(name2);
				if (type1.startsWith("graph")) return -1;
				if (type2.startsWith("graph")) return 1;
				return name1.compareTo(name2);
			}
		}) {
			public int compare(Viewer viewer, Object e1, Object e2) {
				return getComparator().compare(e1, e2);
			}
		};
		treeViewer.setComparator(comparator);
	}
}
