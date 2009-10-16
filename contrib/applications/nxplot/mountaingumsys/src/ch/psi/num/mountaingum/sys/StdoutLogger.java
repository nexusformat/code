/**
 * This is another logger which outputs to stdout.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.sys;

public class StdoutLogger implements LoggerInterface {

	public final static int IO    = 0;
	public final static int TRACE = 1;
	public final static int INFO  = 2;
	public final static int DEBUG = 3;
	public final static int WARN  = 4;
	public final static int ERROR = 5;
	public final static int OFF   = 6;
	
	protected int logLevel;
	
	public void debug(String txt) {
		log(DEBUG,txt);
	}

	public void error(String txt) {
		log(ERROR,txt);
	}

	public void info(String txt) {
		log(INFO,txt);
	}

	public void received(String txt) {
		log(IO,txt);
	}

	public void sent(String txt) {
		log(IO,txt);
	}

	public void trace(String txt) {
		log(TRACE,txt);
	}

	public void warn(String txt) {
		log(WARN,txt);
	}
	protected  void log(int level, String txt){
		if(level >= logLevel){
			System.out.println(txt);
		}
	}

	protected String levelToString(int level) {
		switch(level){
		case IO:
			return "IO";
		case TRACE:
			return "TRACE";
		case INFO:
			return "INFO";
		case DEBUG:
			return "DEBUG";
		case WARN:
			return "WARNING";
		case ERROR:
			return "ERROR";
		}
		return "BADLEVEL";
	}
	public int getLogLevel() {
		return logLevel;
	}

	public void setLogLevel(int logLevel) {
		this.logLevel = logLevel;
	}

	public void exception(Exception eva) {
		if(logLevel > ERROR){
			return;
		}
		/*
		 * This keeps popping up as excessive 
		 * object allocations in hprof-files. So we stop
		 * it with the code above. 
		 */
		log(ERROR,"Exception " + eva.getMessage());
		StackTraceElement stack[] = eva.getStackTrace();
		for(int i = 0; i < stack.length; i++){
			log(ERROR,stack[i].toString());
		}
	}
}
