/**
 * This is a simple text editor as a command editor. This is basically a support editor
 * for unidentified text coming from converted old batch files. But may be there are other 
 * uses, too.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;

public class TextEdit implements CommandNodeEditor {
	public static final String ID = "mountaingumui.TextEdit";
	private ParameterNode editNode;
	private Text edit;
	
	public void enqueue() {
		String txt = edit.getText();
		editNode.enqueueValue(txt.replaceAll("\n", "@nl@"));
	}

	public void save() {
		String txt = edit.getText();
		editNode.setValue(txt.replaceAll("\n", "@nl@"));
	}

	public void start() {
		save();
	}

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		Composite com = new Composite(parent,SWT.NONE);
		com.setLayout(new FillLayout());
		edit = new Text(com,SWT.MULTI);
		editNode = (ParameterNode)n;
		String txt = editNode.getValue().toString();
		txt = txt.replaceAll("@nl@", "\n");
		edit.setText(txt);
		return com;
	}

}
