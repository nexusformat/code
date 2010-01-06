package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.FileFilter;

public class HdfFilter implements FileFilter {
	public HdfFilter() {
	}

	@Override
	public boolean accept(File pathname) {
		if (pathname.isDirectory()) {
			return true;
		}
		String name = pathname.getName();
		if (name.endsWith("hdf")) {
			return true;
		} else if (name.endsWith("hdf5")) {
			return true;
		} else if (name.endsWith("h5")) {
			return true;
		} else if (name.endsWith("hdf4")) {
			return true;
		} else if (name.endsWith("hd4")) {
			return true;
		}
		return false;
	}

	
}
