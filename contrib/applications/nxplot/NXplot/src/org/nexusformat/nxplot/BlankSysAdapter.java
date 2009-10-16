/**
 * This is a dummy system adapter to make NXplot tick
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package org.nexusformat.nxplot;

import ch.psi.num.mountaingum.sys.IDisconnectListener;
import ch.psi.num.mountaingum.sys.ISystemAdapter;
import ch.psi.num.mountaingum.sys.TerminalInterface;
import ch.psi.num.mountaingum.tree.TreeNode;

public class BlankSysAdapter implements ISystemAdapter {

	public void addDisconnectListener(IDisconnectListener l) {
	}

	public boolean canAsync() {
		return false;
	}

	public void closeTerminalInterface(TerminalInterface ti) {
	}

	public void configureTree(TreeNode root) {
	}

	public void interrupt() {
	}

	public TerminalInterface makeTerminalInterface() {
		return null;
	}

	public void removeDisconnectListener(IDisconnectListener l) {
	}

}
