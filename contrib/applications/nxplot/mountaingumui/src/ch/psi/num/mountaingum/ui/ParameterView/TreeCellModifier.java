/**
 * This is being called by the tree cell editors in order to get the data 
 * and set the new value on a TreeNode
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 * 
 * This is currently unused as the TextCellEditor does not work. It flickers
 * under unix. 
 */
package ch.psi.num.mountaingum.ui.ParameterView;

import org.eclipse.jface.viewers.ICellModifier;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class TreeCellModifier implements ICellModifier {

	public boolean canModify(Object element, String property) {
		TreeNode tn = (TreeNode)element;
		SysRegistry.getLogger().debug("canModify: " +TreeUtil.pathForNode(tn));

		if(element instanceof ExecutableNode){
			ExecutableNode ex = (ExecutableNode)element;
			if(ex.isReadOnly()){
				return false;
			} else {
				return true;
			}
		}
		return false;
	}

	public Object getValue(Object element, String property) {
		TreeNode tn = (TreeNode)element;
		SysRegistry.getLogger().debug("getValue: " +TreeUtil.pathForNode(tn));

		if(element instanceof ParameterNode){
			ParameterNode p = (ParameterNode)element;
			return p.getValue().toString();
		}
		if(element instanceof CommandNode){
			return new String("start");
		}
		return null;
	}

	public void modify(Object element, String property, Object value) {
		/*
		 * Property tells us which column is edited. 
		 */
		TreeNode tn = (TreeNode)element;
		SysRegistry.getLogger().debug("Modify: " +TreeUtil.pathForNode(tn));

		if(element instanceof ParameterNode){
			ParameterNode p = (ParameterNode)element;
			if(value instanceof String){
				p.setValue((String)value);
			}
		}
		if(element instanceof CommandNode){
			CommandNode c = (CommandNode)element;
			c.start();
		}
	}
}
