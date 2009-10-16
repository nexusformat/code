/**
 * This is the central access point for all system services.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 * 
 * added TerminalInterface support
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.sys;

import ch.psi.num.mountaingum.batch.BatchAdapter;

public class SysRegistry {
	/**
	 * logging stuff
	 */
	private static LoggerInterface logger = null;

	/**
	 * an adapter which can do tree creation
	 */
	private static ISystemAdapter sys = null;
	
	/**
	 * and adpater for managing batch processing
	 */
	private static BatchAdapter batch = null;
	
	static public LoggerInterface getLogger(){
		if(logger == null){
			logger = new DefaultLogger();
		}
		return logger;
	}
	static synchronized public void setLogger(LoggerInterface li){
		logger = li;
	}
	public static BatchAdapter getBatchAdapter() {
		return batch;
	}
	public static void setBatchAdapter(BatchAdapter batch) {
		SysRegistry.batch = batch;
	}
	public static ISystemAdapter getSystemAdapter() {
		return sys;
	}
	public static void setSystemAdapter(ISystemAdapter sys) {
		SysRegistry.sys = sys;
	}
}
