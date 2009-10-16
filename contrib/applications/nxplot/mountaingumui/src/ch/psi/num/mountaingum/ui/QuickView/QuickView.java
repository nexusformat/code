/**
 * This is the view for displaying a quick view of important parameters.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.QuickView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;


public class QuickView extends ViewPart implements TreeListener {
	public static final String ID = "mountaingumui.QuickView"; 
	private ScrolledComposite scrolli;
	private Composite conti;
	
	public void createPartControl(Composite parent) {
		scrolli = new ScrolledComposite(parent, SWT.V_SCROLL);
		scrolli.setExpandHorizontal(true);
		scrolli.setExpandVertical(true);
		conti = new Composite(scrolli,SWT.NONE);
		scrolli.setContent(conti);
		RowLayout rolli = new RowLayout(SWT.VERTICAL);
		rolli.wrap = false;
		rolli.fill = true;
		conti.setLayout(rolli);
		TreeNode root = TreeRegistry.getInstance().getRoot();
		root.addTreeListener(this);
		initView();
	}

	private class AddNode implements IFunc {
		public Object apply(Object o) {
			ParameterNode pn = (ParameterNode)o;
			new QuickPar(conti,pn);
			return null;
		}
	}
	
	public void initView(){
		String parPath = TreeRegistry.getInstance().getEditorPath("quickview");
		if(parPath == null){
			SysRegistry.getLogger().debug("Failed to locate quickview path in QuickView");
			return;
		}
		TreeNode tn = TreeUtil.nodeForPath(parPath);
		if(tn == null){
			SysRegistry.getLogger().debug("Failed to locate quickview node in QuickView");
			return;
		}
		FuncUtil.map(new ChildStream(tn),new AddNode());
		scrolli.setMinSize(conti.computeSize(SWT.DEFAULT,SWT.DEFAULT));
	}
	public void treeStructureChanged(TreeNode parent) {
		final String parPath = TreeRegistry.getInstance().getEditorPath("quickview");
		if(parPath == null){
			SysRegistry.getLogger().debug("Failed to locate quickview path in QuickView");
			return;
		}
		final TreeNode tn = TreeUtil.nodeForPath(parPath);
		if(conti.isDisposed()){
			return;
		}
		conti.getDisplay().asyncExec(new Runnable() {
			public void run() {
				Control old[] = conti.getChildren();
				for(int i = 0; i < old.length; i++){
					old[i].dispose();
				}
				FuncUtil.map(new ChildStream(tn),new AddNode());
				scrolli.setMinSize(conti.computeSize(SWT.DEFAULT,SWT.DEFAULT));
				scrolli.getParent().layout(true);
			}});
	}
	public void setFocus() {
		// Nothing to do
	}
}
