/**
 * This is an editor input for the Tcl Editor in GTSE. 
 * It delivers data from a String containing the data delivered 
 * by SICS
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.ui.Tcl;

import java.io.InputStream;
import java.io.StringBufferInputStream;

import org.eclipse.core.resources.IStorage;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.ui.IPersistableElement;
import org.eclipse.ui.IStorageEditorInput;

public class StringEditorInput implements IStorageEditorInput {
	private String filename, data;
	
	public void setData(String data) {
		this.data = data;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public StringEditorInput(String filename, String data) {
		this.data = data;
		this.filename = filename;
		if(data == null){
			this.data = "";
		}
	}

	private class StringStorage implements IStorage {

		public InputStream getContents() throws CoreException {
			return new StringBufferInputStream(data);
		}

		public IPath getFullPath() {
			return null;
		}

		public String getName() {
			return filename;
		}

		public boolean isReadOnly() {
			return false;
		}
		public Object getAdapter(Class adapter) {
			return null;
		}
	};
	public IStorage getStorage() throws CoreException {
		return new StringStorage();
	}

	public boolean exists() {
		return true;
	}

	public ImageDescriptor getImageDescriptor() {
		return null;
	}

	public String getName() {
		return filename;
	}

	public IPersistableElement getPersistable() {
		return null;
	}

	public String getToolTipText() {
		return filename;
	}

	public Object getAdapter(Class adapter) {
		return null;
	}
}
