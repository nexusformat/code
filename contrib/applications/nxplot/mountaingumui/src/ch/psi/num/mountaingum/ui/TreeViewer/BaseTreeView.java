/**
 * This is the basic tree viewer.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import java.net.URL;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.TreeEditor;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class BaseTreeView extends ViewPart{
	public static final String ID = "mountaingumui.BaseTreeView";

	protected TreeViewer treeViewer;
    protected TreeEditor eddy;
    protected ImageDescriptor lock;
	protected EditorView eva;
	protected TreeNode root;
	
	public BaseTreeView() {
		super();
		treeViewer = null;
		lock = null;
		eva = null;
	}

	public void createPartControl(Composite parent) {
		parent.setLayout(new GridLayout());
		treeViewer = new TreeViewer(parent,SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION );
		treeViewer.getTree().setLayoutData(new GridData(GridData.FILL_BOTH));
		treeViewer.setContentProvider(new TreeContentProvider());
		treeViewer.setLabelProvider(new TreeLabelProvider());
		createColumns(treeViewer.getTree());
		TreeNode root = TreeRegistry.getInstance().getRoot();
		configureMouseListener();
		treeViewer.expandToLevel(2);
		URL ulli = Platform.getBundle("mountaingumui").getEntry("icons/lock25.gif");
		lock = ImageDescriptor.createFromURL(ulli);
	}
	private void createColumns(Tree tree) {
		TreeColumn nodeColumn = new TreeColumn(tree, SWT.LEFT);
		nodeColumn.setText("name");
		nodeColumn.setWidth(200);

		TreeColumn valueColumn = new TreeColumn(tree, SWT.LEFT);
		valueColumn.setText("value");
		valueColumn.setWidth(200);
		
		tree.setHeaderVisible(true);
		tree.setLinesVisible(true);
	}
	protected class InvokeEdit implements MouseListener{
	    protected ParameterNode editNode;
	    protected int column;
		
		public void mouseDoubleClick(MouseEvent e ) {
			NodeEditor ne = null;
				
			Point pm = new Point(e.x, e.y);
			Tree tree = treeViewer.getTree();
			TreeItem it = tree.getItem(pm);
			if(it == null){
				return;
			}
			Object o = it.getData();
			if(o instanceof CommandNode){
				CommandNode pn = (CommandNode)o;
				String editor = pn.getProperty("viewer");
				if(editor == null && pn.countChildren() == 0){
					pn.start();
				} else {
					ne = RCPUtil.getNodeEditor(editor,"mountaingumui.commandeditor");
					getEditorView().makeEditorContainer(ne, pn);
				}
			}else if (o instanceof GraphicsNode){
				GraphicsNode gn = (GraphicsNode)o;
				String viewerName = gn.getDisplayKey();
				ParameterNode rankNode = (ParameterNode)TreeUtil.findChild(gn, "rank"); 
				IntValue iv = (IntValue)rankNode.getValue();
				int rank[];
				rank = iv.getData();
				if(rank == null || rank.length < 1){
					SysRegistry.getLogger().debug("No rank found, cannot build graphics yet");
					return; // we are not initialized properly yet
				}
				switch(rank[0]){
				case 1:
				         ne  = RCPUtil.getNodeEditor(viewerName, "mountaingumui.graph1d");
						break;
					case 2:
					    ne = RCPUtil.getNodeEditor(viewerName, "mountaingumui.graph2d");
						break;
					case 3:
					    ne = RCPUtil.getNodeEditor(viewerName, "mountaingumui.frameseries");
						break;
					default:
						SysRegistry.getLogger().error("Do not know how to plot data of rank: " 
								+ rank[0]);
						return;
				}
				getEditorView().makeEditorContainer(ne, gn);
			} else {
				TreeNode pn = (TreeNode)o;
				// supress commandset nodes....
				String type = pn.getProperty("type");
				if(type != null && type.endsWith("commandset")){
					return;
				}
				if(pn.countChildren() > 0){
					String editor = pn.getProperty("viewer");
					ne = RCPUtil.getNodeEditor(editor,"mountaingumui.parametereditor");
					getEditorView().makeEditorContainer(ne, pn);
				} else {
					String editor = pn.getProperty("viewer");
					ne = RCPUtil.getNodeEditor(editor,"mountaingumui.SingleParEditor");
					getEditorView().makeEditorContainer(ne, pn);
				}
			}
		}
		public void mouseDown(MouseEvent e) {
			// Nothing to do
		}
		public void mouseUp(MouseEvent e) {
			// Nothing to do
		}

	}
	public void setTree(TreeNode root){
		this.root = root;
		Display d = treeViewer.getTree().getDisplay();
		d.asyncExec(new Runnable(){
			public void run() {
				treeViewer.setInput(BaseTreeView.this.root);
				MasterChangeListener mcl = new MasterChangeListener(treeViewer);
				mcl.treeStructureChanged(BaseTreeView.this.root);
			}});
	}
	protected void configureMouseListener(){
		treeViewer.getTree().addMouseListener(new InvokeEdit());		
	}
	protected  EditorView getEditorView(){
		if(eva == null){
			eva = (EditorView)RCPUtil.findView(EditorView.ID,"1");
		}
		return eva;
	}

	public void setFocus() {
		// Nothing to do
	}

}
