/**
 * This is the controller for the Tcl Editor in Gumtree SE.
 * It connects the UI elements and implements the link 
 * between the UI and the batch system. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September-October 2009
 */
package ch.psi.num.mountaingum.ui.Tcl;

import java.net.URL;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.actions.ActionFactory;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.batch.BatchScriptListener;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.BatchQueue.BatchBufferDialog;
import ch.psi.num.mountaingum.ui.TreeViewer.TerminalLog;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;


public class TclController extends ViewPart implements 
	SelectionListener, BatchScriptListener {
	private Button clear, load, save, saveas, execute, interrupt;
	private boolean executing;
	private TerminalLog logger;
	private BatchEditor eddy;
	private ImageDescriptor logo;
	private Image logoImage;
	
	public TclController(){
		executing = false;
		logger = null;
		URL ulli = Platform.getBundle("mountaingumui").getEntry("icons/tcllogo.gif");
		logo = ImageDescriptor.createFromURL(ulli);
		logoImage = null;
	}
	public void createPartControl(Composite parent) {
		Composite c = new Composite(parent, SWT.NONE);
		c.setLayout(new GridLayout());
		
		clear = new Button(c, SWT.PUSH);
		clear.setText("New");
		clear.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		clear.addSelectionListener(this);

		load = new Button(c, SWT.PUSH);
		load.setText("Load");
		load.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		load.addSelectionListener(this);
		
		save = new Button(c, SWT.PUSH);
		save.setText("Save");
		save.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		save.addSelectionListener(this);

		saveas = new Button(c, SWT.PUSH);
		saveas.setText("Save as");
		saveas.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		saveas.addSelectionListener(this);
		
		execute = new Button(c, SWT.PUSH);
		execute.setText("Execute");
		execute.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		execute.addSelectionListener(this);

		interrupt = new Button(c, SWT.PUSH);
		interrupt.setText("Interrupt");
		interrupt.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		interrupt.addSelectionListener(this);
		interrupt.setBackground(interrupt.getDisplay().getSystemColor(SWT.COLOR_RED));
		
		Label l = new Label(c,SWT.NONE);
		l.setLayoutData(new GridData(GridData.FILL_BOTH));
		if(logoImage == null){
			logoImage = logo.createImage();
		}
		if(logoImage != null){
			l.setImage(logoImage);
		}
	}

	public void dispose() {
		super.dispose();
		if(logoImage != null){
			logoImage.dispose();
		}
	}
	public void widgetSelected(SelectionEvent e) {
		Object o = e.getSource();
		
		if(o == interrupt){
			SysRegistry.getSystemAdapter().interrupt();
			return;
		}
		if(o == clear){
			StringEditorInput ini = new StringEditorInput("NewBatch.tcl", null);
			IWorkbenchPage page = getViewSite().getPage();
			try{
				page.openEditor(ini,"mountaingumui.TclEditor");
			}catch(Exception eva){
				SysRegistry.getLogger().error(eva.getMessage());
				eva.printStackTrace();
			}
			return;
		}
		if(o == load){
			BatchBufferDialog bbd = new BatchBufferDialog(load.getShell(),SWT.OPEN, "NewBatch.tcl");
			int status = bbd.open();
			if(status == Window.OK){
				String url = bbd.getBufferurl();
				if(url != null){
					BatchAdapter ba = SysRegistry.getBatchAdapter();
					String data = ba.loadRaw(url);
					StringEditorInput ini = new StringEditorInput(url,data);
					IWorkbenchPage page = getViewSite().getPage();
					try{
						page.openEditor(ini,"mountaingumui.TclEditor");
					}catch(Exception eva){
						SysRegistry.getLogger().error(eva.getMessage());
						eva.printStackTrace();
					}
					
				}
			}
			return;
		}
		if(o == save){
			save();
		}
		if(o == saveas){
			IWorkbenchPage p = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();
			IEditorPart ede = p.getActiveEditor();
			if(ede == null){
				return; // No open editor, nothing to save.....
			}
			StringEditorInput tei = (StringEditorInput)ede.getEditorInput();
			BatchBufferDialog bbd = new BatchBufferDialog(load.getShell(),SWT.SAVE, tei.getName());
			int status = bbd.open();
			if(status == Window.OK){
				String url = bbd.getBufferurl();
				if(url != null){
					try{
						tei.setFilename(url);
						BatchEditor be = (BatchEditor)ede;
						tei.setData(be.getDocumentProvider().getDocument(tei).get());
						save();
						be.setInput(tei);
					}catch(Exception eva){
						SysRegistry.getLogger().error(eva.getMessage());
						eva.printStackTrace();
					}
					
				}
			}
			return;
		}
		if(o == execute){
			if(executing){
				UIRegistry.getAdapter().displayError("ERROR: cannot execute while already running a batch file");
				return;
			}
			IWorkbenchPage pp = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();
			IEditorPart ie = pp.getActiveEditor();
			if(ie == null){
				log("ERROR: no editor active, cannot execute");
				return;
			}
			eddy = (BatchEditor)ie;
			eddy.saved();
			eddy.setEditable(false);
			executing = true;
			logger = getLogger();
			IEditorInput iee = eddy.getEditorInput();
			String data = eddy.getDocumentProvider().getDocument(iee).get();
			BatchAdapter ba = SysRegistry.getBatchAdapter();
			ba.exeScript(iee.getName(),data,this);
			return;
		}
	}

	private void save(){
		IWorkbenchAction x = ActionFactory.SAVE_ALL.create(PlatformUI.getWorkbench().getActiveWorkbenchWindow());
		x.run();
	}

	public void setFocus() {
		// Nothing to do
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

	public void log(String txt) {
		final String txttxt = txt;
		save.getDisplay().asyncExec(new Runnable(){
			public void run() {
				logger.log(txttxt);
			}});
	}

	private TerminalLog getLogger() {
		if(logger == null){
			logger = (TerminalLog)RCPUtil.findView(TerminalLog.ID);
		}
		return logger;
	}
	public void range(int start, int end) {
		final int s = start;
		final int e = end;
		save.getDisplay().asyncExec(new Runnable(){
			public void run() {
				eddy.selectAndReveal(s, e-s);
				//eddy.setHighlightRange(s, e-s, true);
			}});
	}

	public void terminated() {
		executing = false;
		eddy.setEditable(true);
		eddy.setHighlightRange(0,0,false);
	}
}
