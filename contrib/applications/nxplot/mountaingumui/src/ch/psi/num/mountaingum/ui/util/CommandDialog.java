/**
 * This is a general dialog for collecting parameter values for 
 * a command and running it when accept has been pressed.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.util;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;

public class CommandDialog extends Dialog {
	private CommandNode komma;
	private Text par[];
	
	public CommandDialog(Shell parentShell, CommandNode n) {
		super(parentShell);
		komma = n;
	}

	protected Control createDialogArea(Composite parent) {
		ParameterNode pn;
		Composite c = (Composite)super.createDialogArea(parent);
		GridLayout gustaf = new GridLayout();
		gustaf.numColumns = 2;
		c.setLayout(gustaf);
		
		par = new Text[komma.countChildren()];
		for(int i = 0; i < komma.countChildren(); i++){
			pn = (ParameterNode)komma.getChild(i);
			Label l = new Label(c,SWT.NONE);
			l.setText(pn.getName());
			l.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
			par[i] = new Text(c, SWT.SINGLE);
			par[i].setText(pn.getValue().toString());
			par[i].setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		}
		
		return c;
	}

	protected void okPressed() {
		int i;
		ParameterNode pn;
		for(i = 0; i < komma.countChildren(); i++){
			pn = (ParameterNode)komma.getChild(i);
			pn.setValue(par[i].getText());
		}
		komma.start();
		super.okPressed();
	}

	protected void configureShell(Shell newShell) {
		super.configureShell(newShell);
		newShell.setText(komma.getName());
	}

}
