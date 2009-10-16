/**
 * This is a container for showing NodeEditors in a tabbed pane. I put this 
 * feature into a separate view in order to have more freedom with the 
 * placement of these things in the UI.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007 
 * 
 * Added makeEditorContainer, makeBatchContainer while implementing batch
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.EditorView;

import java.util.HashMap;
import java.util.Iterator;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabFolder2Listener;
import org.eclipse.swt.custom.CTabFolderEvent;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.sys.IDisconnectListener;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.UIRegistry;

public class EditorView extends ViewPart implements CTabFolder2Listener, IDisconnectListener {
	public final static String ID = "mountaingumui.EditorView";
	
	protected HashMap tabs;
	protected  CTabFolder tabbi;
	
	public EditorView() {
		tabbi = null;
	}

	public void createPartControl(Composite parent) {
		Composite c = new Composite(parent,SWT.NONE);
		c.setLayout(new FillLayout());
		tabbi = new CTabFolder(c,SWT.TOP | SWT.CLOSE);
		tabbi.addCTabFolder2Listener(this);
		tabs = new HashMap();
		SysRegistry.getSystemAdapter().addDisconnectListener(this);
	}

	public void addNodeEditor(NodeEditor egon, TreeNode n){
		if(tabbi == null || tabbi.isDisposed()){
			return;
		}
		if(makeExistingVisible(n)){
			return;
		}
		try{
			Control c = (Control)egon.makeEditor(tabbi, n);
			CTabItem newTab = new CTabItem(tabbi,SWT.NONE);
			newTab.setControl(c);
			newTab.setText(n.getName());
			tabbi.getParent().layout(true,true);
			tabbi.setSelection(newTab);
			tabs.put(TreeUtil.pathForNode(n), newTab);
		}catch(Exception eva){
			UIRegistry.getAdapter().displayError("Exception while loading editor: " + eva.getMessage());
			//eva.printStackTrace();
		}
	}
	public void makeEditorContainer(NodeEditor egon, TreeNode n){
		if(makeExistingVisible(n)){
			return;
		}
		if(! (egon instanceof CommandNodeEditor)){
			addNodeEditor(egon,n);
			return;
		}
		try{
			CommandNodeEditor xegon = (CommandNodeEditor)egon;
			Control c = new EditorContainer(tabbi, xegon,n);
			CTabItem newTab = new CTabItem(tabbi,SWT.NONE);
			newTab.setControl(c);
			newTab.setText(n.getName());
			tabbi.getParent().layout(true,true);
			tabbi.setSelection(newTab);
			tabs.put(TreeUtil.pathForNode(n), newTab);
		}catch(Exception eva){
			UIRegistry.getAdapter().displayError("Exception while loading editor: " + eva.getMessage());
			eva.printStackTrace();
		}
	}
	protected boolean makeExistingVisible(TreeNode n){
		CTabItem it = (CTabItem)tabs.get(TreeUtil.pathForNode(n));
		if(it != null){
			tabbi.setSelection(it);
			return true;
		}
		return false;
	}
	public void setFocus() {
		// Nothing to do here
	}

	public void close(CTabFolderEvent event) {
		String key;
		Object o = event.item;
		
		Iterator it = tabs.keySet().iterator();
		while(it.hasNext()){
			key = (String)it.next();
			if(tabs.get(key) == o){
				tabs.remove(key);
				return;
			}
		}
	}
	public void disconnect() {
		tabs.clear();
		tabbi.getDisplay().asyncExec(new Runnable() {
			public void run() {
				CTabItem c[] = tabbi.getItems();
				for(int i = 0; i < c.length; i++){
					c[i].dispose();
				}
				tabbi.getParent().layout(true, true);
			}});
	}

	public void maximize(CTabFolderEvent event) {
		// Nothing to do
	}
	public void minimize(CTabFolderEvent event) {
		// Nothing to do
	}
	public void restore(CTabFolderEvent event) {
		// Nothing to do
	}
	public void showList(CTabFolderEvent event) {
		// Nothing to do
	}
}
