/**
 * This is the interface to be implemented by all classes which contribute 
 * a graphical tool for editing or viewing a node or a part of a node hierarchy. 
 * According to their use, the NodeEditor lives in idfferent containers which supply
 * the buttons.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007 
 * 
 * Revised during batch editor implementation, Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.eclipse;

import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.tree.TreeNode;

public interface NodeEditor {
	/**
	 * build the actual editor component
	 * @param parent The parent into which to include the editor
	 * @param n The TreeNode associated with this editor
	 * @return A suitable GUI component for editing this node
	 * @throws Exception If things gow rong 
	 */
	public Object makeEditor(Composite parent, TreeNode n)throws Exception;
}
