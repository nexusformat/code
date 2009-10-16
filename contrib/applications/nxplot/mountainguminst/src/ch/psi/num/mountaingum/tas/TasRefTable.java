/**
 * This is an editor for a TAS reflection table together with the special 
 * commands required to manage it and calculate UB from. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.tas;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.nebula.widgets.compositetable.IRowContentProvider;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.util.CommandDialog;

public class TasRefTable implements NodeEditor, IRowContentProvider, UpdateListener, SelectionListener {
	private CompositeTable tabbi;
	private String rowData[];
	private TasRefRow rows[];
	private String refID[];
	private final static String datanode = "tasrefdata"; 
	private Button clear, addauxref, addref, addrefang, makeub, del;
	private TreeNode master;
	
	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		Composite con = new Composite(parent, SWT.NONE);
		con.setLayout(new GridLayout());
		con.setBackground(con.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tabbi = new CompositeTable(con,SWT.NONE);
		tabbi.setBackground(con.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tabbi.setLayoutData(new GridData(GridData.FILL_BOTH));
		new TasRefHeader(tabbi,SWT.NONE);
		new TasRefRow(tabbi,SWT.NONE);
		tabbi.addRowContentProvider(this);
		tabbi.setRunTime(true);
		
		ParameterNode pn = (ParameterNode)TreeUtil.findChild(n,datanode);
		pn.addUpdateListener(this);
		nodeUpdated(pn);
		master = n;
		
		Composite buttonRow = new Composite(con,SWT.NONE);
		buttonRow.setLayout(new RowLayout(SWT.HORIZONTAL));
		clear = new Button(buttonRow,SWT.PUSH);
		clear.setText("Clear");
		clear.addSelectionListener(this);

		addauxref = new Button(buttonRow,SWT.PUSH);
		addauxref.setText("Add Aux Ref");
		addauxref.addSelectionListener(this);
		
		addref = new Button(buttonRow,SWT.PUSH);
		addref.setText("Add Ref");
		addref.addSelectionListener(this);
		
		addrefang = new Button(buttonRow,SWT.PUSH);
		addrefang.setText("Add Ref + Ang");
		addrefang.addSelectionListener(this);
		
		makeub = new Button(buttonRow,SWT.PUSH);
		makeub.setText("Calc UB");
		makeub.addSelectionListener(this);
		
		del = new Button(buttonRow,SWT.PUSH);
		del.setText("Delete");
		del.addSelectionListener(this);
		
		return con;
	}

	public void refresh(CompositeTable sender, int idx,
			Control row) {
		int dataidx = idx + 1;
		int i;
		
		TasRefRow trf = (TasRefRow)row;
		if(dataidx < rowData.length){
			trf.selection.setSelection(false);
			String tata[] = rowData[dataidx].trim().split("\\s+");
			for(i = 0; i < 9; i++){
				trf.rowData[i].setText(tata[i+1]);
			}
			refID[idx] = tata[0];
		}
		rows[idx] = trf;
		CommandNode repref = (CommandNode)TreeUtil.findChild(master, "repref");
		trf.setUpdate(repref, idx+1);
	}
	public void nodeUpdated(TreeNode n) {
		ParameterNode pn = (ParameterNode)n;
		rowData = pn.getValue().toString().split("\n");
		rows = new TasRefRow[rowData.length - 1];
		refID = new String[rowData.length -1];
		tabbi.getDisplay().asyncExec(new Runnable(){
			public void run() {
				tabbi.setNumRowsInCollection(rowData.length-1);
				tabbi.refreshAllRows();
			}});	
	}

	public void widgetSelected(SelectionEvent e) {
		ParameterNode pn;
		int i;
		
		Object o = e.getSource();
		if(o == clear){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "clear");
			cn.start();
			return;
		} else if(o == addauxref){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "addauxref");
			CommandDialog cd = new CommandDialog(addauxref.getDisplay().getActiveShell(), cn);
			cd.open();
		} else if(o == addref){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "addref");
			CommandDialog cd = new CommandDialog(addref.getDisplay().getActiveShell(), cn);
			cd.open();
		} else if(o == addrefang){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "addrefang");
			CommandDialog cd = new CommandDialog(addrefang.getDisplay().getActiveShell(), cn);
			cd.open();
		} else if(o == makeub){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "makeub");
			int id = 1;
			for(i = 0; i < rows.length; i++){
				if(rows[i].selection.getSelection()){
					if(id == 1){
						pn = (ParameterNode)TreeUtil.findChild(cn, "id1");
						pn.setValue(refID[i]);
						id++;
						continue;
					}
					if(id == 2){
						pn = (ParameterNode)TreeUtil.findChild(cn, "id2");
						pn.setValue(refID[i]);
						break;
					}
				}
			}
			if(id != 2){
				UIRegistry.getAdapter().displayError("Need two reflections to calculate UB");
				return;
			} else {
				cn.start();
			}
			cleanSelection();
		} else if(o == del){
			CommandNode cn = (CommandNode)TreeUtil.findChild(master, "del");
			for(i = 0; i < rows.length; i++){
				if(rows[i].selection.getSelection()){
					pn = (ParameterNode)TreeUtil.findChild(cn, "id1");
					pn.setValue(refID[i]);
					cn.start();
				}
			}
			cleanSelection();
		}
	}
	
	private void cleanSelection() {
		for(int i = 0; i < rows.length; i++){
			rows[i].selection.setSelection(false);
		}
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

}
