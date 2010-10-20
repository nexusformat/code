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
 * HdfFilter.java
 *
 */
package org.nexusformat.nxvalidate.filter;

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
