/**
 * This is a little view which packages the TerminalText widget into 
 * a view to be used for the TreeViewer or elsewhere.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.ui.CommandLineTerminal.OutputStyle;
import ch.psi.num.mountaingum.ui.CommandLineTerminal.TerminalText;

public class TerminalLog extends ViewPart {

	public static final String ID = "mountaingumui.TerminalLog";
	
	private TerminalText text;
	public TerminalLog() {
	}

	public void createPartControl(Composite parent) {
		Composite cont = new Composite(parent,SWT.NONE);
		cont.setLayout(new FillLayout());
		text = new TerminalText(cont, SWT.MULTI | SWT.V_SCROLL
	                | SWT.H_SCROLL | SWT.FULL_SELECTION | SWT.READ_ONLY);
		text.setWordWrap(false);
		cont.layout(true);
	}
	public void logCommand(String command){
		final String com = command;
		text.getDisplay().asyncExec(new Runnable() {

			public void run() {
				text.appendInputText(com);
			}});
	}
	public void log(String data){
		if(data.indexOf("ERROR") >= 0){
			appendOutput(data,OutputStyle.ERROR);
		} else if(data.indexOf("WARNING") >= 0){
			appendOutput(data,OutputStyle.WARNING);
		} else {
			appendOutput(data,OutputStyle.NORMAL);
		}
	}
	private void appendOutput(String txt, int style){
		final int st = style;
		final String t = txt;
		text.getDisplay().asyncExec(new Runnable(){
			public void run() {
				text.appendOutputText(t, st);
			}});
	}
	public void setFocus() {
		// nothing to do
	}
}
