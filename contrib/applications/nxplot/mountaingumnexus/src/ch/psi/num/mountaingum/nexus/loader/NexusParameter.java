/**
 * This is a very simple derived class of InternalParameter which makes a 
 * property by setting the property mogified if it has been changed. 
 * The intent is that, when saving files, this property will be examined and 
 * only those fields changed on file which have this property.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ch.psi.num.mountaingum.tree.TreeNode;


public class NexusParameter extends ch.psi.num.mountaingum.tree.InternalParameter {

	public NexusParameter(TreeNode p, String name) {
		super(p, name);
	}

	public void setValue(String val) {
		super.setValue(val);
		setProperty("mogrified", "true");
	}

	
}
