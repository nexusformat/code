/**
 * This is a dialog for saving or laoding a batch buffer
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.BatchQueue;

import java.util.Collections;
import java.util.List;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;
import ch.psi.num.mountaingum.sys.SysRegistry;

public class BatchBufferDialog extends Dialog {
	private int type;
	private String bufferurl;
	private Combo combi;
	
	public BatchBufferDialog(Shell parentShell, int type, String url) {
		super(parentShell);
		this.type = type;
		bufferurl = url;
	}

	protected void cancelPressed() {
		bufferurl = null;
		super.cancelPressed();
	}

	private class CombiAdd implements IFunc {
		public Object apply(Object o) {
			combi.add((String)o);
			return null;
		}
	}
	protected Control createDialogArea(Composite parent) {
		Composite main = (Composite)super.createDialogArea(parent);
		main.setLayout(new GridLayout());
		Label l = new Label(main,SWT.NONE);
		if(type == SWT.OPEN){
			l.setText("Select buffer to load");
		}  else {
			l.setText("Select buffer to save");
		}
		combi = new Combo(main,SWT.DROP_DOWN);
		List buli = SysRegistry.getBatchAdapter().getAvailableBatchBuffers();
		Collections.sort(buli);
		FuncUtil.map(new IteratorStream(buli), new CombiAdd());
		if(bufferurl != null){
			combi.setText(bufferurl);
		}
		
		return main;
	}

	protected void okPressed() {
		bufferurl = combi.getText();
		super.okPressed();
	}

	public String getBufferurl() {
		return bufferurl;
	}

}
