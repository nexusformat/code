/**
 * This is a support class which watches over table cells 
 * and nodes and keeps them in sync.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer.table;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class TableUpdater implements UpdateListener, FocusListener, Listener {
	private HashMap node2Text, text2Node, row2Sel;
	private TreeNode table;
	
	TableUpdater(TreeNode n){
		node2Text = new HashMap();
		text2Node = new HashMap();
		row2Sel = new HashMap();
		table = n;
	}
	
	void clear(){
		node2Text.clear();
		text2Node.clear();
		row2Sel.clear();
	}
	
	void add(ParameterNode pn, Text t){
		node2Text.put(pn, t);
		text2Node.put(t, pn);
		
		try{
			pn.removeUpdateListener(this);
		}catch(Exception e){}
		pn.addUpdateListener(this);
		
		t.addFocusListener(this);
		t.addListener(SWT.KeyUp, this);
	}

	void addRow(int i, Button b){
		row2Sel.put(i,b);
	}
	
	LinkedList collectSelection(){
		LinkedList l = new LinkedList();
		int row;
		Button b;
		
		Iterator it = row2Sel.keySet().iterator(); 
		while(it.hasNext()){
			row = (Integer)it.next();
			b = (Button)row2Sel.get(row);
			if(b.getSelection() == true){
				l.add(row);
			}
		}
		
		return l;
	}

	void clearSelection(){
		Button b;
		
		Iterator it = row2Sel.keySet().iterator();
		while(it.hasNext()){
			b = (Button)row2Sel.get(it.next());
			b.setSelection(false);
		}
	}
	
	public void nodeUpdated(TreeNode n) {
		final ParameterNode pn = (ParameterNode)n;
		final Text t = (Text)node2Text.get(pn);
		t.getDisplay().asyncExec(new Runnable(){
			public void run() {
					t.setText(pn.getValue().toString());
			}});
	}
	public void focusLost(FocusEvent e) {
		if(e.getSource() instanceof Text){
			updateNode((Text)e.getSource());
		}
	}
	public void handleEvent(Event event) {
		if(event.type == SWT.KeyUp && event.item instanceof Text  && event.character == '\n' || event.character == '\r'){
			Text t = (Text)event.item;
			updateNode(t);
		}
	}
	private void updateNode(Text source) {
		ParameterNode pn = (ParameterNode)text2Node.get(source);
		pn.setValue(source.getText());
		table.notifyTreeListeners();
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}
	public void focusGained(FocusEvent e) {
		// Nothing to do
	}

}
