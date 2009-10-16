/**
 * This is small class which handles the instrument information line as a Eclipse-RCP
 * status line contribution. The information is held in a special node in the tree.
 *  Updates to this node are then reflected in this classes text label. For this to 
 *  work there are a couple of dependencies:
 *  - This class has to be installed in ApplicationActionBarAdvisor.fillStatusLine()
 *  - SicsFacade.configureTree has to make sure that the node is created
 *  - SicsFacade.newRole() has to update the content of this node. 
 *  I feel bad about having these dependencies and that this is distributed across so many 
 *  places. Partly this is a consequence of the eclipse framework. An alternative would have 
 *  been to create a RoleListener interface and all the associated mechanics. I choose to rather 
 *  go through the tree for this.    
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, November 2008    
 */
package ch.psi.num.mountaingum.ui.log;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.jface.action.StatusLineLayoutData;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.FontMetrics;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class UserStatusContribution extends ContributionItem implements
		UpdateListener, TreeListener {
	private final static String instNode = "/gui/inststatus";
	
	private Label text;
	private ParameterNode statNode;
	
	public UserStatusContribution(){
		TreeRegistry.getInstance().getRoot().addTreeListener(this);
		treeStructureChanged(TreeRegistry.getInstance().getRoot());
	}
	
	public void fill(Composite parent) {
		text = new Label(parent,SWT.LEFT);
		GC gc = new GC(parent);
		gc.setFont(parent.getFont());
		FontMetrics fm = gc.getFontMetrics();
        int width = 35*fm.getAverageCharWidth();
        int height = fm.getHeight();
        gc.dispose();
        
        StatusLineLayoutData sl = new StatusLineLayoutData();
        sl.widthHint = width;
        sl.heightHint = height;
        text.setLayoutData(sl);
        text.setText("Disconnected..");
        text.setBackground(text.getDisplay().getSystemColor(SWT.COLOR_MAGENTA));
	}

	public void nodeStatusChanged(TreeNode node) {
		// nothing to do here
	}

	public void nodeUpdated(TreeNode n) {
		if(text == null){
			return;
		}

		statNode = (ParameterNode)n;
		final String status = statNode.getValue().toString();
		if(text.isDisposed())return;
		text.getDisplay().asyncExec(new Runnable(){
			public void run(){
				if(!text.isDisposed() && status != null){
					text.setText(status);
				}
			}
		});
	}

	public void treeStructureChanged(TreeNode parent) {
		if(text != null){
			text.getDisplay().asyncExec(new Runnable(){
				public void run(){
					text.setText("Disconnected...");
				}
			});
		}
		statNode = (ParameterNode)TreeUtil.nodeForPath(instNode);
		if(statNode != null){
			statNode.addUpdateListener(this);
			nodeUpdated(statNode);
		} else {
			SysRegistry.getLogger().info("No instrument status line at this time");
		}
	}
}
