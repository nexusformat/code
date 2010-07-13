package org.nexusformat.nxvalidate.filter;

import org.nexusformat.nxvalidate.filter.ExtensionFilter;
import java.io.FileFilter;
import java.util.Vector;

public class NeXusFilter extends ExtensionFilter {
	public static final String EXTENSION = ".nxs";
	
	public NeXusFilter() {
		super(EXTENSION);
	}

}
