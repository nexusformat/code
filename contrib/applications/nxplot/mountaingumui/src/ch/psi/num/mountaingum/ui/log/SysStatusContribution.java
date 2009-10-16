/**
 * This is small class which handles the system status line as a Eclipse-RCP
 * status line contribution. A path to a node holding the system status 
 * information is retrieved from the SysRegistry. Updates to this node 
 * are then reflected in this classes text label.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007    
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

public class SysStatusContribution extends ContributionItem implements
		UpdateListener, TreeListener {

	private Label text;
	private ParameterNode statNode;
	
	public SysStatusContribution(){
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
		String statPath = TreeRegistry.getInstance().getEditorPath("statuspath");
		if(statPath != null){
			try{
				statNode = (ParameterNode)TreeUtil.nodeForPath(statPath);
				if(statNode != null){
					statNode.addUpdateListener(this);
					nodeUpdated(statNode);
				}
			}catch(Exception eva){
				SysRegistry.getLogger().debug(statPath + " is invalid status node");
				SysRegistry.getLogger().debug("Exception was: " + eva.getMessage());
			}
		}
	}
}
