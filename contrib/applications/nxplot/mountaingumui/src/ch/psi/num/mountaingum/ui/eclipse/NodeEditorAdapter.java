/**
 * This is an empty implementation of the NodeEditor interface to allow to use 
 * inheritance on the interface.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.eclipse;

import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.tree.TreeNode;

public class NodeEditorAdapter implements CommandNodeEditor {

	public void enqueue() {
	}

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		return null;
	}

	public void save() {
	}

	public void start() {
	}
}
