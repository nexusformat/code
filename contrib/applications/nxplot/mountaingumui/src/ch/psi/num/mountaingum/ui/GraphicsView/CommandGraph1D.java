/**
 * This is a version of Graph1D where the graphics is created on demand. 
 * The only operation operation overloaded is refresh(). The new refresh will try to 
 * locate a command node called refresh and invoke this one. The underlying system 
 * is supposed to update the data nodes of the graph as a consequence of calling 
 * this command.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class CommandGraph1D extends Graph1DSGT {
	public static final String ID = "mountaingumui.CommandGraph1D";
	public void refresh() {
		CommandNode cn = (CommandNode)TreeUtil.findChild(graphNode,"refresh");
		if(cn != null){
			cn.start();
		} else {
			SysRegistry.getLogger().debug("No refresh command found for " 
					+ graphNode.getName());
		}
	}
}
