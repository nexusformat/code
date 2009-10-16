/**
 * This is an editor container which does not display an interrupt 
 * button. Interrupt buttons are somewhat silly if the editor is 
 * viewring things from for instance a NeXus tree and not an
 * instrument.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.ui.EditorView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;

public class TreeContainer extends EditorContainer {

	TreeContainer(Composite parent, CommandNodeEditor ne, TreeNode n) throws Exception{
		super(parent, ne, n);
	}

	protected void makeButtons() {
		Composite buttonPanel = new Composite(this,SWT.NONE);
		buttonPanel.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		buttonPanel.setLayout(new FillLayout(SWT.HORIZONTAL));
		
		start = new Button(buttonPanel,SWT.PUSH);
		start.setText("Make It So");
		start.addSelectionListener(this);
	}
}
