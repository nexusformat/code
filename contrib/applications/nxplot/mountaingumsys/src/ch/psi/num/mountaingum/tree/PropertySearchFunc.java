/**
 * This is a node processor which searches for a given property 
 * with a given value.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, November 2008
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.func.IFunc;

public class PropertySearchFunc implements IFunc {
	private String key, value;
	
	public PropertySearchFunc(String k, String v){
		key = k;
		value = v;
	}
	
	public Object apply(Object o) {
		TreeNode n = (TreeNode)o;
		String myValue = n.getProperty(key);
		if(myValue != null){
			if(myValue.compareToIgnoreCase(value) == 0){
				return o;
			}
		}
		return null;
	}

}
