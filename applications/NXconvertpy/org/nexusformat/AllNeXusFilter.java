package org.nexusformat;

import java.io.File;
import java.io.FileFilter;
import java.util.Vector;

public class AllNeXusFilter implements FileFilter {
	private Vector<FileFilter> filters;

	public AllNeXusFilter() {
		this.filters = new Vector<FileFilter>();
		this.filters.add(new NeXusFilter());
		this.filters.add(new HdfFilter());
		this.filters.add(new XmlFilter());
	}

	@Override
	public boolean accept(File pathname) {
		for (FileFilter filter : this.filters) {
			if (filter.accept(pathname)) {
				return true;
			}
		}
		return false;
	}

}
