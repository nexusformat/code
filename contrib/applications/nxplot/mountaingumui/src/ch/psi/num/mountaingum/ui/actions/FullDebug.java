/**
 * This action creates full debug output
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.ui.actions;

import ch.psi.num.mountaingum.sys.StdoutLogger;

public class FullDebug extends DebugLevelAction {

	public FullDebug() {
		debugLevel = StdoutLogger.IO;
	}
	
}
