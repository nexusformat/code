/**
 * This class enables logiing of errors and exceptions
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.ui.actions;

import ch.psi.num.mountaingum.sys.StdoutLogger;

public class HalfDebug extends DebugLevelAction {

	public HalfDebug() {
		debugLevel = StdoutLogger.DEBUG;
	}
	
}
