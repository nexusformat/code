/**
 * This is the view for the graphics display. It contains a list of graphics 
 * datasets on the left and a tabbed pane with activated plots on the right.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import java.util.HashMap;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.TabFolder;
import org.eclipse.swt.widgets.TabItem;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.QuickView.QuickView;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class GraphicsView extends ViewPart implements SelectionListener, TreeListener {
	public static final String ID = "mountaingumui.GraphicsView";

	private CTabFolder tabbi;
	private List graphList;
	private TreeNode currentRoot;
	private final String qvName = "quickview";
	/*
	 * I need to keep track of my viewers in order to be able to locate 
	 * the NodeEditors again for creating hard copy.
	 */
	private HashMap viewers;
	
	public GraphicsView() {
		TreeRegistry.getInstance().getRoot().addTreeListener(this);
		viewers = new HashMap();
	}

	public void createPartControl(Composite parent) {
		GridLayout parentLayout = new GridLayout();
		parentLayout.numColumns = 2;
		parent.setLayout(parentLayout);
		viewers.clear();
		
		buildGraphList(parent);
		tabbi = new CTabFolder(parent,SWT.TOP | SWT.CLOSE);
		GridData tabData = new GridData(GridData.FILL_BOTH);
		tabbi.setLayoutData(tabData);
		treeStructureChanged(TreeRegistry.getInstance().getRoot());
		showGraphView(0);
	}
	private void buildGraphList(Composite parent) {
		Composite panel = new Composite(parent,SWT.NONE);
		panel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gl = new GridLayout();
		gl.numColumns = 1;
		panel.setLayout(gl);
		Label l = new Label(panel,SWT.LEFT);
		l.setText("Graphics");
		graphList = new List(panel, SWT.SINGLE);
		graphList.addSelectionListener(this);
		graphList.setLayoutData(new GridData(GridData.FILL_BOTH));
		graphList.add("Hugo");
		graphList.add("Lieselotte");
		graphList.add(qvName);
	}
	private class FillList implements IFunc {

		public Object apply(Object o) {
			if(o instanceof GraphicsNode){
				GraphicsNode gn = (GraphicsNode)o;
				graphList.add(gn.getName());
			}
			return null;
		}
	}
	public void treeStructureChanged(TreeNode parent) {
		final String graphPath = TreeRegistry.getInstance().getEditorPath("graphview");
		if(graphPath == null){
			SysRegistry.getLogger().debug("Failed to locate graphics path in GraphView");
			return;
		}
		TreeNode tn = TreeUtil.nodeForPath(graphPath);
		if(tn == null){
			tn = TreeRegistry.getInstance().getRoot();
		}
		currentRoot = tn;
		Display d = tabbi.getDisplay();
		final ChildStream commands = new ChildStream(tn);
		d.asyncExec(new Runnable(){
			public void run(){
				try{
					removeOldGraphics();
					FuncUtil.map(commands, new FillList());
					graphList.add(qvName);
					graphList.select(0);
					graphList.getParent().getParent().layout(true);
					showGraphView(0);
					currentRoot.removeTreeListener(GraphicsView.this);
					currentRoot.addTreeListener(GraphicsView.this);
					TreeRegistry.getInstance().getRoot().removeTreeListener(GraphicsView.this);
					TreeRegistry.getInstance().getRoot().addTreeListener(GraphicsView.this);
					//SWTUtil.printHierarchy(master, 0);
				}catch(Exception e){
					SysRegistry.getLogger().exception(e);
			e.printStackTrace();
				}
			}
		});
	}


	private void showGraphView(int idx) {
		if(graphList.getItemCount() < 1){
			return;
		}
		if(!showExistingView(idx)){
			try{
				createNewView(idx);
			}catch(Exception edith){
				SysRegistry.getLogger().exception(edith);
			}
		}
	}
	private void removeOldGraphics() {
		if(graphList != null && !graphList.isDisposed()){
			graphList.removeAll();
			CTabItem it[] = tabbi.getItems();
			if(it != null){
				for(int i = 0; i < it.length; i++){
					if(it[i].getControl() != null){
						it[i].getControl().dispose();
					}
					it[i].dispose();
				}
			}
		}
	}
	private void createNewView(int idx)throws Exception {
		NodeEditor processor = null;
		String processorName = "Undefined";
		int  rank[] = new int[1];
		rank[0] = 0;
		
		String name = graphList.getItem(idx);
		if(name.equalsIgnoreCase(qvName)){
			makeQuickView();
			return;
		}
		GraphicsNode gn = (GraphicsNode)TreeUtil.findChild(currentRoot, name);
		if(gn == null){
			SysRegistry.getLogger().debug("No graphics node found; cannot build graphics yet");
			return;
		}
		processorName = gn.getDisplayKey();
		ParameterNode rankNode = (ParameterNode)TreeUtil.findChild(gn, "rank"); 
		IntValue iv = (IntValue)rankNode.getValue();
		rank = iv.getData();
		if(rank == null || rank.length < 1){
			SysRegistry.getLogger().debug("No rank found, cannot build graphics yet");
			return; // we are not initialized properly yet
		}
		switch(rank[0]){
		case 1:
		    processor = RCPUtil.getNodeEditor(processorName, "mountaingumui.graph1d");
				break;
			case 2:
			    processor = RCPUtil.getNodeEditor(processorName, "mountaingumui.graph2d");
				break;
			case 3:
			    processor = RCPUtil.getNodeEditor(processorName, "mountaingumui.frameseries");
				break;
			default:
				SysRegistry.getLogger().error("Do not know how to plot data of rank: " 
						+ rank[0]);
				return;
		}
		if(processor == null){
			SysRegistry.getLogger().error("Failed to locate default view for rank " 
					+ rank[0]);
			return;
		}
		CTabItem newTab = new CTabItem(tabbi,SWT.NONE);
		newTab.setText(name);
		Control c = (Control)processor.makeEditor(tabbi,gn);
		newTab.setControl(c);
		viewers.put(name, processor);
		tabbi.layout(true);
		tabbi.setSelection(newTab);
	}

	private void makeQuickView() {
		CTabItem newTab = new CTabItem(tabbi,SWT.NONE);
		newTab.setText(qvName);
		QuickView qv = new QuickView();
		Composite c = new Composite(tabbi, SWT.NONE);
		c.setLayout(new FillLayout());
		qv.createPartControl(c);
		newTab.setControl(c);
		tabbi.layout();
		tabbi.setSelection(newTab);
	}

	private boolean showExistingView(int idx) {
		int i;
		String name = graphList.getItem(idx);
		for(i = 0; i < tabbi.getItemCount(); i++){
			CTabItem it = tabbi.getItem(i);
			if(it.getText().equals(name)){
				tabbi.setSelection(i);
				return true;
			}
		}
		return false;
	}

	NodeViewer getPlot(){
		int idx = tabbi.getSelectionIndex();
		if(idx >= 0){
			CTabItem ti = tabbi.getItem(idx);
			String name = ti.getText();
			Object o = viewers.get(name);
			if(o instanceof NodeViewer){
				return (NodeViewer)o;
			}
		}
		return null;
	}
	
	public void setFocus() {
		// nothing to do
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		selectGraphics();
	}

	public void widgetSelected(SelectionEvent e) {
		selectGraphics();
	}
	private void selectGraphics(){
		int idx = graphList.getSelectionIndex();
		showGraphView(idx);
	}
}
