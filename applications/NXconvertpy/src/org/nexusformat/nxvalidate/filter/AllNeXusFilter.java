package org.nexusformat.nxvalidate.filter;

import org.nexusformat.nxvalidate.*;
import java.io.File;
import java.util.Vector;
import javax.swing.filechooser.FileFilter;

public class AllNeXusFilter extends FileFilter implements java.io.FileFilter {
	private Vector<FileFilter> filters;
	private String description;

	public AllNeXusFilter() {
		this.filters = new Vector<FileFilter>();
		this.filters.add(new NeXusFilter());
		this.filters.add(new HdfFilter());
		this.filters.add(new XmlFilter());
		
		this.description = "";
		int numFilters = this.filters.size();
		for (int i = 0; i < numFilters; i++) {
		    this.description += this.filters.get(i).getDescription();
		    if ((i + 1) < numFilters) {
		        this.description += ", ";
		    }
		}
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

    @Override
    public String getDescription() {
        return this.description;
    }

}
