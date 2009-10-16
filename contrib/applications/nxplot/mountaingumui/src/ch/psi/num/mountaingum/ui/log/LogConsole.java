/**
 * This class displays the output of the internal log in a eclipse 
 * console window.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.log;

import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;

import ch.psi.num.mountaingum.sys.StdoutLogger;

public class LogConsole extends StdoutLogger {
	private MessageConsoleStream out;
	public LogConsole(){
		super();
		MessageConsole logConsole = new MessageConsole("LogConsole", null);
		out = logConsole.newMessageStream();
		ConsolePlugin.getDefault().getConsoleManager().addConsoles(new IConsole[]  {logConsole});
	}

	protected void log(int level, String txt) {
		if(level < logLevel){
			return;
		}
		final String mess = levelToString(level) + ":" + txt;
		Display d = ConsolePlugin.getStandardDisplay();
		if(d != null){
			d.asyncExec(new Runnable() {
				public void run(){
					out.println(mess);
				}
			});
		}
	}
}
