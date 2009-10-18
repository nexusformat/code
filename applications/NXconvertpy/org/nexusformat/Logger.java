package org.nexusformat;

final class Logger {
	private static final Logger logger = new Logger();

	private static int loggingLevel;
	
	public static final int TRACE = 0;
	public static final int DEBUG = 1;
	public static final int INFO = 2;
	public static final int WARN = 3;
	public static final int ERROR = 4;

	private Logger() {
		loggingLevel = INFO;
	}

	public static void setLevel(final int level) {
		loggingLevel = level;
	}

	static Logger getInstance() {
		return logger;
	}

	private void println(final String message, final int level) {
		if (level >= loggingLevel) {
			System.out.println(message);
		}
	}

	public void trace(final String message) {
		this.println(message, TRACE);
	}
	public void debug(final String message) {
		this.println(message, DEBUG);
	}
	public void info(final String message) {
		this.println(message, INFO);
	}
	public void warn(final String message) {
		this.println(message, WARN);
	}
	public void error(final String message) {
		this.println(message, ERROR);
	}
}