package org.nexusformat.nxvalidate.filter;

import org.nexusformat.nxvalidate.filter.ExtensionFilter;

public class SchematronFilter extends ExtensionFilter {
	public static final String EXTENSION = ".sch";

	public SchematronFilter() {
		super(EXTENSION);
	}
}
