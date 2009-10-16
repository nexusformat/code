/**
 * This is derived from cbg.editor.ColoringEditor and implements 
 * some code to initialize properly and do specialized 
 * saving.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.ui.Tcl;

import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.jface.text.IDocument;
import org.eclipse.ui.IEditorInput;

import cbg.editor.ColoringEditor;
import cbg.editor.ColoringSourceViewerConfiguration;
import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.sys.SysRegistry;

public class BatchEditor extends ColoringEditor {
	public static final String ID = "mountaingumui.TclEditor"; 
	private boolean editable;
	
	public void setEditable(boolean editable) {
		this.editable = editable;
	}

	public boolean isEditable() {
		return editable;
	}

	public BatchEditor(){
		super();
        ((ColoringSourceViewerConfiguration) getSourceViewerConfiguration()).setFilename("Hugo.tcl");
        editable = true;
	}

	public void doSave(IProgressMonitor progressMonitor) {
		IEditorInput in = getEditorInput();
		if(in != null && in instanceof StringEditorInput){
			IDocument doc = this.getDocumentProvider().getDocument(in);
			String txt = doc.get();
			BatchAdapter ba = SysRegistry.getBatchAdapter();
			ba.saveRaw(in.getName(), txt);
			this.editorSaved();
		}
		super.doSave(progressMonitor);
	}
	public void saved(){
		this.editorSaved();
	}
}
