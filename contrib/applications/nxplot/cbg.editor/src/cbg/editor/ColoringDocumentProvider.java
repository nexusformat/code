package cbg.editor;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IDocumentPartitioner;
import org.eclipse.jface.text.source.IAnnotationModel;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IPathEditorInput;
import org.eclipse.ui.IStorageEditorInput;
import org.eclipse.ui.editors.text.FileDocumentProvider;
import org.eclipse.ui.editors.text.TextFileDocumentProvider;
import org.eclipse.ui.texteditor.IElementStateListener;

public class ColoringDocumentProvider extends FileDocumentProvider {

    private TextFileDocumentProvider textFileDocumentProvider;

    public ColoringDocumentProvider() {
        super();
    }

    protected IDocument createDocument(Object element) throws CoreException {
        IDocument document = super.createDocument(element);
        String filename = getFileName(element);
        if (document == null) {
            textFileDocumentProvider = new TextFileDocumentProvider();
            textFileDocumentProvider.connect(element);
            document = textFileDocumentProvider.getDocument(element);
        }
        initializeDocument(document, filename);
        return document;
    }

    public boolean isReadOnly(Object element) {
        return textFileDocumentProvider == null ? super.isReadOnly(element) : textFileDocumentProvider.isReadOnly(element);
    }

    public boolean isModifiable(Object element) {
        return textFileDocumentProvider == null ? super.isModifiable(element) : textFileDocumentProvider.isModifiable(element);
    }

    public IDocument getDocument(Object element) {
        return textFileDocumentProvider == null ? super.getDocument(element) : textFileDocumentProvider.getDocument(element);
    }

    protected void doResetDocument(Object element, IProgressMonitor monitor) throws CoreException {
        if (textFileDocumentProvider == null) super.doResetDocument(element, monitor);
        else
            textFileDocumentProvider.resetDocument(element);
    }

    protected void doSaveDocument(IProgressMonitor monitor, Object element, IDocument document, boolean overwrite) throws CoreException {
        if (textFileDocumentProvider == null) super.doSaveDocument(monitor, element, document, overwrite);
        else
            textFileDocumentProvider.saveDocument(monitor, element, document, overwrite);
    }

    public long getModificationStamp(Object element) {
        return textFileDocumentProvider == null ? super.getModificationStamp(element) : textFileDocumentProvider.getModificationStamp(element);
    }

    public long getSynchronizationStamp(Object element) {
        return textFileDocumentProvider == null ? super.getSynchronizationStamp(element) : textFileDocumentProvider.getSynchronizationStamp(element);
    }

    public boolean isDeleted(Object element) {
        return textFileDocumentProvider == null ? super.isDeleted(element) : textFileDocumentProvider.isDeleted(element);
    }

    public boolean mustSaveDocument(Object element) {
        return textFileDocumentProvider == null ? super.mustSaveDocument(element) : textFileDocumentProvider.mustSaveDocument(element);
    }

    public boolean canSaveDocument(Object element) {
        return textFileDocumentProvider == null ? super.canSaveDocument(element) : textFileDocumentProvider.canSaveDocument(element);
    }

    public IAnnotationModel getAnnotationModel(Object element) {
        return textFileDocumentProvider == null ? super.getAnnotationModel(element) : textFileDocumentProvider.getAnnotationModel(element);
    }

    public void aboutToChange(Object element) {
        if (textFileDocumentProvider == null) super.aboutToChange(element);
        else
            textFileDocumentProvider.aboutToChange(element);
    }

    public void changed(Object element) {
        if (textFileDocumentProvider == null) super.changed(element);
        else
            textFileDocumentProvider.changed(element);
    }

    public void addElementStateListener(IElementStateListener listener) {
        if (textFileDocumentProvider == null) super.addElementStateListener(listener);
        else
            textFileDocumentProvider.addElementStateListener(listener);
    }

    public void removeElementStateListener(IElementStateListener listener) {
        if (textFileDocumentProvider == null) super.removeElementStateListener(listener);
        else
            textFileDocumentProvider.removeElementStateListener(listener);
    }

    //	protected IDocument createDocument(Object element) throws CoreException {
    //		IDocument doc = super.createDocument(element);
    //		String filename = null;
    //		if(element instanceof IPathEditorInput) {
    //			filename = ((IPathEditorInput)element).getName();
    //            IDocument document= createEmptyDocument();
    //            setDocumentContent(document, (IPathEditorInput)element, null);
    //            doc = document;
    //		} else if(element instanceof IEditorInput) {
    //            filename = ((IEditorInput)element).getName();
    //            IDocument document= createEmptyDocument();
    //            setDocumentContent(document, (IEditorInput)element, null);
    //            doc = document;
    //        }
    //        Assert.isNotNull(doc);
    //		initializeDocument(doc, filename);
    //		return doc;
    //	}

    private String getFileName(Object element) {
        if (element instanceof IPathEditorInput) {
            return ((IPathEditorInput) element).getName();
        } else if (element instanceof IStorageEditorInput) {
            try {
                return ((IStorageEditorInput) element).getStorage().getName();
            } catch (CoreException e) {
                e.printStackTrace();
            }
        } else if (element instanceof IEditorInput) { return ((IEditorInput) element).getName(); }
        return null;
    }

    protected void disposeElementInfo(Object element, ElementInfo info) {
        super.disposeElementInfo(element, info);
        if (textFileDocumentProvider != null) {
            textFileDocumentProvider.disconnect(element);
        }
    }

    protected void setupDocument(Object element, IDocument document) {
        super.setupDocument(element, document);
        String filename = null;
        if (element instanceof IPathEditorInput) {
            filename = ((IPathEditorInput) element).getName();
        } else if (element instanceof IStorageEditorInput) {
            try {
                filename = ((IStorageEditorInput) element).getStorage().getName();
            } catch (CoreException e) {
                e.printStackTrace();
            }
        } else if (element instanceof IEditorInput) {
            filename = ((IEditorInput) element).getName();
        }
        initializeDocument(document, filename);
    }

    private void initializeDocument(IDocument document, String filename) {
        if (document != null) {
            IDocumentPartitioner partitioner = createPartitioner(filename);
            document.setDocumentPartitioner(partitioner);
            partitioner.connect(document);
        }
    }

    private IDocumentPartitioner createPartitioner(String filename) {
        ColoringPartitionScanner scanner = new ColoringPartitionScanner(filename);
        IDocumentPartitioner partitioner = new ColoringPartitioner(scanner, scanner.getContentTypes());
        return partitioner;
    }

}