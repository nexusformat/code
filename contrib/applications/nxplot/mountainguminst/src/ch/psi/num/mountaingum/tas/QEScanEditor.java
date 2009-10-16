/**
 * This is a special scan editor for triple axis QE-scans.
 * As they love it!
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.tas;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.TreeViewer.ScanEditor;

public class QEScanEditor extends ScanEditor {

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		scrolli = new ScrolledComposite(parent, SWT.V_SCROLL|SWT.H_SCROLL);
		editor = new Composite(scrolli, SWT.NONE);
		editor.setLayout(new GridLayout());
		scanNode = n;
		
		makeControlGroup(editor);
		scanMode.select(1);
		calcMode = CENTERSCAN;
		
		makeScanTable(editor);
		if(TreeUtil.nodeForPath("/instrument/motors/QH") == null){
			UIRegistry.getAdapter().displayError("Malconfigured tree, cannot locate QH, QK, QL");
			return null;
		}
		addScanVar("/instrument/motors/QH");
		addScanVar("/instrument/motors/QK");
		addScanVar("/instrument/motors/QL");
		addScanVar("/instrument/motors/EN");
		
		loadFromTree(n);
		comNode = (CommandNode)n;
		
		scrolli.setContent(editor);
		scrolli.setExpandHorizontal(true);
		scrolli.setExpandVertical(true);
		scrolli.setMinSize(editor.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		
		return scrolli;

	}

}
