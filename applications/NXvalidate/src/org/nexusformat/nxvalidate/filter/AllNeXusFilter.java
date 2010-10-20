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
 * AllNeXusFilter.java
 *
 */
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
