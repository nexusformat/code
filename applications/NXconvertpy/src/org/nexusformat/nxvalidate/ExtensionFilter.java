package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.FileFilter;

public class ExtensionFilter extends javax.swing.filechooser.FileFilter
                                     implements FileFilter {
	private String extension;
	private boolean withDirectories;

	ExtensionFilter(final String extension) {
		this(extension, true);
	}

	ExtensionFilter(final String extension, final boolean withDirectories) {
		if (extension == null) {
			throw new Error("Cannot filter null extension");
		}
		if (extension.length() <= 0) {
			throw new Error("Cannot filter empty extension");
		}
		this.extension = extension;
		this.withDirectories = withDirectories;
	}

	/** {@inheritDoc} */
	public final boolean accept(File pathname) {
		if (pathname.isDirectory()) {
			return this.withDirectories;
		}
		return (pathname.getName().endsWith(this.extension));
	}

    @Override
    public String getDescription() {
        return "*" + this.extension;
    }
}
