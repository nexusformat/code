/**
 * This changes EditorView to show a TreeContainer 
 * for CommandNodeEditors. This suppresses the interrupt 
 * button which looks silly when displaying a NeXus 
 * tree
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 * 
 */
package ch.psi.num.mountaingum.ui.EditorView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.widgets.Control;

import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;

public class TreeEditorView extends EditorView {
	public final static String ID = "mountaingumui.TreeEditorView";

	public void makeEditorContainer(NodeEditor egon, TreeNode n) {
		if(makeExistingVisible(n)){
			return;
		}
		if(! (egon instanceof CommandNodeEditor)){
			addNodeEditor(egon,n);
			return;
		}
		try{
			CommandNodeEditor xegon = (CommandNodeEditor)egon;
			Control c = new TreeContainer(tabbi, xegon,n);
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

}
