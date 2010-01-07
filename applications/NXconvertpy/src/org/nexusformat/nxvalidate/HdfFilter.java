package org.nexusformat.nxvalidate;

import java.io.File;
import javax.swing.filechooser.FileFilter;

public class HdfFilter extends FileFilter implements java.io.FileFilter {
	public HdfFilter() {
	}

	@Override
	public boolean accept(File pathname) {
		if (pathname.isDirectory()) {
			return true;
		}
		String name = pathname.getName();
		if (name.endsWith(".hdf")) {
			return true;
		} else if (name.endsWith(".hdf5")) {
			return true;
		} else if (name.endsWith(".h5")) {
			return true;
		} else if (name.endsWith(".hdf4")) {
			return true;
		} else if (name.endsWith(".hd4")) {
			return true;
		}
		return false;
	}

    @Override
    public String getDescription() {
        return "*.hdf, *.hdf4, *.hdf5, *.h4, *.h5";
    }
	
}
