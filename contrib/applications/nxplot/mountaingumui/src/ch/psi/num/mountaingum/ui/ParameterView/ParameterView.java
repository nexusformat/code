/**
 * This is the viewer for the parameter tree.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.ParameterView;

import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.TreeEditor;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.sys.ISystemAdapter;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.log.TreeLogger;

public class ParameterView extends ViewPart implements TreeListener, Listener,
	SelectionListener {
	public static final String ID = "mountaingum.ParameterView";

	protected TreeViewer treeViewer;
	protected TreeLogger logger;
    protected TreeEditor eddy;
	
	public ParameterView() {
		super();
		treeViewer = null;
		logger = null;
	}
	private class InvokeEdit extends MouseAdapter implements  KeyListener,
		MouseListener, FocusListener {

		InvokeEdit(){
			eddyControl = null;
		}
		private Text eddyControl;
	    protected ParameterNode editNode;
	    protected int column;
		
		public void mouseDoubleClick(MouseEvent e ) {
			Point pm = new Point(e.x, e.y);
			Tree tree = treeViewer.getTree();
			TreeItem it = tree.getItem(pm);
			Object o = it.getData();
			for(int i = 0; i < tree.getColumnCount(); i++){
				Rectangle r = it.getBounds(i);
				if(r.contains(pm)){
					column = i;
				}
			}
			if(column != TreeLabelProvider.ENQUEUE){
				column = TreeLabelProvider.START;
			}
			if(o instanceof ParameterNode){
				ParameterNode p = (ParameterNode)o;
				if(!p.isReadOnly()){
					editNode = p;
					eddyControl = new Text(treeViewer.getTree(),SWT.NONE);
					String data = p.getValue().toString();
					eddyControl.setText(data);
					eddyControl.addKeyListener(this);
					eddyControl.addFocusListener(this);
					eddyControl.setSelection(data.length());
					eddyControl.setFocus();
					eddy.setEditor((Control)eddyControl, it, column);
				}
			}
		}
		public void keyPressed(KeyEvent e) {
			if(e.character == '\r' || e.character == '\n'){
				SysRegistry.getLogger().debug(eddyControl.getText());
				if(editNode != null){
					if(column == TreeLabelProvider.START){
						editNode.setValue(eddyControl.getText());
					} else {
						editNode.enqueueValue(eddyControl.getText());
					}
				}
				eddyControl.dispose();
				eddyControl = null;
			}
		}
		public void keyReleased(KeyEvent e) {
			// Do nothing here
		}
		public void focusGained(FocusEvent e) {
			// nothing to do here
		}
		public void focusLost(FocusEvent e) {
			/*
			 * This means I close the editor without saving. T provent it
			 * from hanging around. 
			 */
			if(eddyControl != null){
				eddyControl.dispose();
				eddyControl = null;
			}
		}
		
	};
	/**
	 * This is not yet used
	private class ColumnSelect implements SelectionListener{

		public void widgetDefaultSelected(SelectionEvent e) {
			SysRegistry.getLogger().debug("enqueue columns selected");
		}

		public void widgetSelected(SelectionEvent e) {
			SysRegistry.getLogger().debug("enqueue columns selected");
		}
		
	};
	*/
	public void createPartControl(Composite parent) {
		parent.setLayout(new GridLayout());
		treeViewer = new TreeViewer(parent,SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION );
		treeViewer.getTree().setLayoutData(new GridData(GridData.FILL_BOTH));
		treeViewer.setContentProvider(new TreeContentProvider());
		treeViewer.setLabelProvider(new TreeLabelProvider());
		createColumns(treeViewer.getTree());
		createTreeEditor();
		TreeNode root = TreeRegistry.getInstance().getRoot();
		root.addTreeListener(this);
		treeViewer.getTree().addMouseListener(new InvokeEdit());
		treeViewer.expandToLevel(2);
		treeStructureChanged(root);
		treeViewer.getTree().addListener(SWT.PaintItem, this);
		
		Button inti = new Button(parent,SWT.PUSH);
		inti.setText("Interrupt");
		inti.setBackground(parent.getDisplay().getSystemColor(SWT.COLOR_RED));
		inti.addSelectionListener(this);
		inti.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
	}

	private void createTreeEditor(){
		eddy = new TreeEditor(treeViewer.getTree());
		eddy.grabHorizontal = true;
		eddy.minimumWidth = 200;
	}

	private void createColumns(Tree tree) {
		TreeColumn nodeColumn = new TreeColumn(tree, SWT.LEFT);
		nodeColumn.setText("name");
		nodeColumn.setWidth(200);

		TreeColumn valueColumn = new TreeColumn(tree, SWT.LEFT);
		valueColumn.setText("value");
		valueColumn.setWidth(200);

		TreeColumn stateColumn = new TreeColumn(tree, SWT.LEFT);
		stateColumn.setText("state");
		stateColumn.setWidth(50);

		TreeColumn startColumn = new TreeColumn(tree, SWT.LEFT);
		startColumn.setText("newValue");
		startColumn.setWidth(200);
	
		TreeColumn queueColumn = new TreeColumn(tree, SWT.LEFT);
		queueColumn.setText("enqueue");
//		queueColumn.addSelectionListener(new ColumnSelect());
		queueColumn.setWidth(200);

		TreeColumn logColumn = new TreeColumn(tree, SWT.LEFT);
		logColumn.setText("log");
		logColumn.setWidth(200);
		
		tree.setHeaderVisible(true);
		tree.setLinesVisible(true);
	}

	public void setFocus() {
	}

	/**
	 * we need to locate the tree to display somehow. The tree may 
	 * not be available when the view is generated. What is always 
	 * there is the root of everything. We therefore register a listener 
	 * for tree structure changes on the root node. This listener calls
	 * treeStructureChanged. This function will then try to locate the 
	 * node to display and set it as the TreeViewers input. This not only 
	 * solves the problem of deferred tree generation but also of 
	 * runtime changes to the whole tree. 
	 */
	public void treeStructureChanged(TreeNode parent) {
		final String parPath = TreeRegistry.getInstance().getEditorPath("parametereditor");
		if(parPath == null){
			SysRegistry.getLogger().debug("Failed to locate parametereditor path in ParameterView");
			return;
		}
		final TreeNode tn = TreeUtil.nodeForPath(parPath);
		Display d = treeViewer.getTree().getDisplay();
		if(tn != null){
			d.asyncExec(new Runnable(){
				public void run(){
					treeViewer.setInput(tn);
					MasterChangeListener mcl = new MasterChangeListener(treeViewer);
					mcl.treeStructureChanged(tn);
					makeLogger(parPath);
				}
			});
		} else {
			d.asyncExec(new Runnable() {
				public void run(){
					treeViewer.setInput(TreeRegistry.getInstance().getRoot());
				}
			});
		}
	}
	private void makeLogger(String parPath) {
		if(logger != null){
			ConsolePlugin.getDefault().getConsoleManager().removeConsoles(new IConsole[] {logger.getLogConsole()});
			logger = null;
		}
		logger = new TreeLogger(parPath);
	}

	public void dispose(){
		TreeRegistry.getInstance().getRoot().removeTreeListener(this);
		if(logger != null){
			ConsolePlugin.getDefault().getConsoleManager().removeConsoles(new IConsole[] {logger.getLogConsole()});
		}
		super.dispose();
	}
	public void refresh(){
		treeViewer.refresh(TreeRegistry.getInstance().getRoot());
	}
	/**
	 * Custom painting of the status column
	 */
	public void handleEvent(Event event) {
		Point size;
		int yOffset;
		
		Object o = event.item.getData();
		if(event.type == SWT.PaintItem && event.index == TreeLabelProvider.STATE
				&& o instanceof ParameterNode){
			ParameterNode p = (ParameterNode)o;
			switch(p.getState()){
			case ExecutableNode.OK:
				event.gc.setBackground(event.item.getDisplay().getSystemColor(SWT.COLOR_WHITE));
				event.gc.fillRectangle(event.x, event.y, event.width, event.height);
				size = event.gc.textExtent("OK"); 
				yOffset = Math.max(0,(event.height - size.y)/2);
				event.gc.drawText("OK", event.x + 5, event.y + yOffset);
				break;
			case ExecutableNode.RUN:
				event.gc.setBackground(event.item.getDisplay().getSystemColor(SWT.COLOR_GREEN));
				event.gc.fillRectangle(event.x, event.y, event.width, event.height);
				size = event.gc.textExtent("Run"); 
				yOffset = Math.max(0,(event.height - size.y)/2);
				event.gc.drawText("Run", event.x + 5, event.y + yOffset);
				break;
			case ExecutableNode.ERROR:
				event.gc.setBackground(event.item.getDisplay().getSystemColor(SWT.COLOR_RED));
				event.gc.fillRectangle(event.x, event.y, event.width, event.height);
				size = event.gc.textExtent("ERROR"); 
				yOffset = Math.max(0,(event.height - size.y)/2);
				event.gc.drawText("ERROR", event.x + 5, event.y + yOffset);
				break;
			}
		}
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// nothing to do here
	}

	public void widgetSelected(SelectionEvent e) {
		ISystemAdapter sys = SysRegistry.getSystemAdapter();
		sys.interrupt();
	}
}
