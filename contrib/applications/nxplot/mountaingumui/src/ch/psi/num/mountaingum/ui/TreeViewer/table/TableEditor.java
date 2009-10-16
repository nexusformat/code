/**
 * This is the editor for tables. Tables are expected to have a special
 * node structure:
 * - template: holds as children the column names of the table
 * - data: holds as children the row data
 * - clear A command to empty the table
 * - addrow c1 c2 c3 ... A command to add a row to a table.
 * 
 * I have to use an ugly trick to initialize the CompositeTable. The table needs header and 
 * row classes. The table insists to use the Row(Composite, int) constructor. But in order 
 * to construct this nicely I need model data, i.e. the configuration of the table. CompositeTable 
 * only accepts static private or normal classes as for rows and headers. No easy way to pass in the 
 * necessary information. The workaround is:
 * 1) A SWT widget can have a key value pair set as data. I use this to store the template
 * 2) The header and row classes search backwards for the CompositeTable instance and retrieve 
 *    the data.  
 *   
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer.table;

import java.util.Iterator;
import java.util.LinkedList;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.nebula.widgets.compositetable.IRowContentProvider;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;

import ch.psi.num.mountaingum.batch.BatchNode;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.util.Fmt;

public class TableEditor implements NodeEditor, IRowContentProvider, SelectionListener, TreeListener {
	protected TreeNode table, template, data;
	protected  CompositeTable tableView;
	private Button clear, addrow, delete;
	protected boolean batch;
	protected TableUpdater uppi;
	
		
	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		table = n;
		template = TreeUtil.findChild(n, "template");
		data = TreeUtil.findChild(n, "data");
		data.addTreeListener(this);
		
		if(n instanceof BatchNode){
			batch = true;
		} else {
			batch = false;
		}
		
		uppi = new TableUpdater(n);
		
		Composite view = new Composite(parent, SWT.NONE);
		view.setLayout(new GridLayout());
		view.setBackground(view.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tableView = new CompositeTable(view,SWT.NULL);
		tableView.setBackground(view.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tableView.setLayoutData(new GridData(GridData.FILL_BOTH));
		tableView.setData("template", template);
		new TableHeader(tableView,SWT.NONE);
		new TableRow(tableView,SWT.NONE);
		tableView.addRowContentProvider(this);
		tableView.setRunTime(true);
		tableView.setNumRowsInCollection(data.countChildren());
		
		makeButtonRow(view);
		
		return view;
	}
	
	protected  void makeButtonRow(Composite view){
		Composite buttonRow = new Composite(view,SWT.NONE);
		buttonRow.setLayout(new RowLayout(SWT.HORIZONTAL));
		clear = new Button(buttonRow,SWT.PUSH);
		clear.setText("Clear");
		clear.addSelectionListener(this);

		addrow = new Button(buttonRow,SWT.PUSH);
		addrow.setText("Addrow");
		addrow.addSelectionListener(this);
		
		delete = new Button(buttonRow,SWT.PUSH);
		delete.setText("Delete");
		delete.addSelectionListener(this);
		
		addTableButtons(buttonRow);
	}

	private void addTableButtons(Composite buttonRow) {
		TreeNode com;
		
		if(batch){
			return;
		}
		ChildStream cs = new ChildStream(table);
		while((com = (TreeNode)cs.next()) != null){
			if(com.getProperty("tablecommand") != null){
				Button bx = new Button(buttonRow, SWT.PUSH);
				bx.setText(com.getName());
				bx.addSelectionListener(this);
			}
		}
		
	}

	/*
	 * fill a table row
	 * @param  sender The table to fill
	 * @param row The number of the row to fill
	 * @param r The control for the row
	 */
	public void refresh(CompositeTable sender, int row,
			Control r) {
		TableRow tr = (TableRow)r;
		TreeNode pn;
		ParameterNode pn2;
		int i;
		
		uppi.addRow(row, tr.selection);
		
		if(row < data.countChildren()){
			pn = (TreeNode)data.getChild(row);
			for(i = 0; i < tr.rowData.length; i++){
				pn2 = (ParameterNode)pn.getChild(i);
				tr.rowData[i].setText(pn2.getValue().toString());
				uppi.add(pn2, tr.rowData[i]);
			}
		} else {
			tr.selection.setSelection(false);
			for(i = 0; i  < tr.rowData.length; i++){
				tr.rowData[i].setText("99");
			}
		}
	}
	public void treeStructureChanged(TreeNode parent) {
		tableView.getDisplay().asyncExec(new Runnable(){
			public void run() {
				uppi.clear();
				tableView.setNumRowsInCollection(data.countChildren());
				tableView.refreshAllRows();
			}});
	}

	public void widgetSelected(SelectionEvent e) {
		Object o = e.getSource();
		
		if(o == clear){
			clear();
		} else if(o == addrow){
			addrow();
		} else if(o == delete ){
			doDelete();
		} else {
			Button b = (Button)e.getSource();
			invokeTableCommand(b.getText());
		}
	}
	private void invokeTableCommand(String text) {
		TreeNode tn;
		
		CommandNode com = (CommandNode)TreeUtil.findChild(table, text);
		if(com == null){
			return;
		}
		
		LinkedList l = uppi.collectSelection();
		Integer ig;
		
		Iterator it = l.iterator();
		LinkedList toDelete = new LinkedList();
		while(it.hasNext()){
			ig = (Integer)it.next();
			toDelete.add(data.getChild(ig.intValue()));
		}
		it = toDelete.iterator();
		StringBuffer args = new StringBuffer();
		while(it.hasNext()){
			tn = (TreeNode)it.next();
			args.append(tn.getName());
			args.append(' ');
		}
		ParameterNode a = (ParameterNode)TreeUtil.findChild(com, "args");
		a.setValue(args.toString().trim());
		com.start();
		uppi.clearSelection();
	}

	private void doDelete() {
		LinkedList l = uppi.collectSelection();
		Integer ig;
		
		if(l.isEmpty()){
			UIRegistry.getAdapter().displayError("Nothing selected to process");
			return;
		}
		Iterator it = l.iterator();
		LinkedList toDelete = new LinkedList();
		while(it.hasNext()){
			ig = (Integer)it.next();
			toDelete.add(data.getChild(ig.intValue()));
		}
		it = toDelete.iterator();
		while(it.hasNext()){
			if(batch){
				data.deleteChild((TreeNode)it.next());
				treeStructureChanged(table);
				table.notifyTreeListeners();
			} else {
				CommandNode cn = (CommandNode)TreeUtil.findChild(table, "del");
				ParameterNode pn = (ParameterNode)TreeUtil.findChild(cn, "id");
				TreeNode n = (TreeNode)it.next();
				pn.setValue(n.getName());
				cn.start();
			}
		}
		uppi.clearSelection();
	}

	private void addrow() {
		ParameterNode pn;
		
		if(batch){
			int i = tableView.getNumRowsInCollection();
			TreeNode row = TreeUtil.privateDeepCopy(template);
			row.setProperty("name", Fmt.fmt(i+1,4,Fmt.ZF));
			data.insertNode(TreeNode.APPEND, row);
			table.notifyTreeListeners();
		} else {
			CommandNode addrow = (CommandNode)TreeUtil.findChild(table, "addrow");
			ChildStream cs = new ChildStream(addrow);
			while((pn = (ParameterNode)cs.next()) != null){
				pn.setValue(".0");
			}
			addrow.start();
		}
	}

	private void clear() {
		CommandNode clear = null;
		
		if(batch){
			data.deleteChildren();
			table.notifyTreeListeners();
		} else {
			clear = (CommandNode)TreeUtil.findChild(table, "clear");
			clear.start();
		}
		
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

}
