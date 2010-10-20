/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Nexus Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information, see <http://www.nexusformat.org/>
 *
 * Logger.java
 *
 */
package org.nexusformat.nxvalidate;

import java.util.Vector;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

final class Logger {
	private static final Logger logger = new Logger();

	private static int loggingLevel;
	private static Vector<ChangeListener> listeners
	 							= new Vector<ChangeListener>();
	private static boolean hasListeners = false;
	
	public static final int TRACE = 0;
	public static final int DEBUG = 1;
	public static final int INFO = 2;
	public static final int WARN = 3;
	public static final int ERROR = 4;

	private Logger() {
		loggingLevel = INFO;
	}

	public static void addListener(final ChangeListener listener) {
		if (listener == null)
			return;
		listeners.add(listener);
		hasListeners = true;
	}

	public static void setLevel(final int level) {
		loggingLevel = level;
	}

	static Logger getInstance() {
		return logger;
	}

	private void println(final String message, final int level) {
		if (level >= loggingLevel) {
			if (hasListeners) {
				ChangeEvent event = new ChangeEvent(message);
				for (ChangeListener listener : listeners) {
					listener.stateChanged(event);
				}
			} else {
				System.out.println(message);
			}
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
