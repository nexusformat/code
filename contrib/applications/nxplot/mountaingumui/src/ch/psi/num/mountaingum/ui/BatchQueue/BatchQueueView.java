/**
 * This is the view for the Mountaingum batch queue editor.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, January 2008
 *  
 *  added drag and drop, Mark Koennecke, October 2008
 *  
 *  Modified to accomodate the TableEditor, Mark Koennecke, July 2009
 *  The TableEditor communicates changes by sending a 
 *  treeStructureChangeEvent to this class. A cleanup would be to modify the 
 *  BatchContainer to act in a similar way. 
 *   
 */
package ch.psi.num.mountaingum.ui.BatchQueue;

import java.util.Iterator;
import java.util.LinkedList;

import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.custom.TableEditor;
import org.eclipse.swt.dnd.DND;
import org.eclipse.swt.dnd.DragSource;
import org.eclipse.swt.dnd.DragSourceEvent;
import org.eclipse.swt.dnd.DragSourceListener;
import org.eclipse.swt.dnd.DropTarget;
import org.eclipse.swt.dnd.DropTargetAdapter;
import org.eclipse.swt.dnd.DropTargetEvent;
import org.eclipse.swt.dnd.TextTransfer;
import org.eclipse.swt.dnd.Transfer;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Widget;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.batch.BatchListener;
import ch.psi.num.mountaingum.batch.BatchNode;
import ch.psi.num.mountaingum.batch.BatchParseException;
import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.CommandLineTerminal.OutputStyle;
import ch.psi.num.mountaingum.ui.CommandLineTerminal.TerminalText;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;
import ch.psi.num.mountaingum.util.ListSequence;
import ch.psi.num.mountaingum.util.Sequence;
import ch.psi.num.mountaingum.util.SequenceStream;

public class BatchQueueView extends ViewPart implements SelectionListener,
	BatchListener, TreeListener{
	public static final String ID = "mountaingumui.BatchQueueView";
	protected static final int COLSIZE = 150;
	
	private BatchAdapter batchi;
	private Sequence buffer;
	private Sequence clipboard;
	private Button cut, copy, paste, neu, load, save, start, interrupt;
	protected  ScrolledComposite queuePane;
	protected Table queue;
	private ScrolledComposite editorPane;
	private TerminalText log;
	protected  TableEditor eddy;
	private String bufferUrl;
	private LinkedList dragSelection;
	protected SashForm mainPane;
	private int currentIndex;
	
	/* drag and drop*/
	private Transfer types[] = {TextTransfer.getInstance()};
	private int dragOps = DND.DROP_COPY | DND.DROP_MOVE;
	private DragSource dragSource;
	private DropTarget dragTarget, paneTarget;
	
	public BatchQueueView() {
		batchi = SysRegistry.getBatchAdapter();
		buffer = batchi.getSequence();
		clipboard = new ListSequence();
		batchi.addBatchListener(this);
		bufferUrl = "Mountaingum.job";
		dragSelection = new LinkedList();
	}

	public void createPartControl(Composite parent) {
		Composite pane = new Composite(parent,SWT.NONE);
		GridLayout gd = new GridLayout();
		gd.numColumns = 1;
		pane.setLayout(gd);
		
		mainPane = new SashForm(pane,SWT.HORIZONTAL);
		mainPane.setLayoutData(new GridData(GridData.FILL_BOTH));
		configureMainPane(mainPane);
		configureDnD();
		int w[] = new int[2];
		w[0] = 30;
		w[1] = 60;
		mainPane.setWeights(w);
		
		Composite buttonPane = new Composite(pane,SWT.NONE);
		configureButtonPane(buttonPane);
		
		currentIndex = -1;
		//SWTUtil.printHierarchy(parent, 2);
	}

	private void configureButtonPane(Composite buttonPane) {
		RowLayout rolli = new RowLayout(SWT.HORIZONTAL);
		rolli.fill = true;
		buttonPane.setLayout(rolli);
		
		cut = new Button(buttonPane,SWT.PUSH);
		cut.setText("Cut");
		cut.addSelectionListener(this);
	
		copy = new Button(buttonPane,SWT.PUSH);
		copy.setText("Copy");
		copy.addSelectionListener(this);
		
		paste = new Button(buttonPane,SWT.PUSH);
		paste.setText("Paste");
		paste.addSelectionListener(this);

		neu = new Button(buttonPane,SWT.PUSH);
		neu.setText("Clear");
		neu.addSelectionListener(this);
		
		load = new Button(buttonPane,SWT.PUSH);
		load.setText("Load");
		load.addSelectionListener(this);

		save = new Button(buttonPane,SWT.PUSH);
		save.setText("Save");
		save.addSelectionListener(this);

		start = new Button(buttonPane,SWT.PUSH);
		start.setText("Start");
		start.addSelectionListener(this);
		
		interrupt = new Button(buttonPane,SWT.PUSH);
		interrupt.setText("Interrupt");
		interrupt.addSelectionListener(this);
		interrupt.setBackground(interrupt.getDisplay().getSystemColor(SWT.COLOR_RED));
	}
	protected  class QueueMouse extends MouseAdapter {
		public void mouseDoubleClick(MouseEvent e) {
			if(e.stateMask != 0){
				int idx = queue.getSelectionIndex();
				if(idx >= 0){
					doEdit(queue.getItem(idx));
				}
			} else {
				int idx = queue.getSelectionIndex();
				if(idx >= 0){
					editGraphically();
				}
			}
		}
	}
	protected void configureMainPane(Composite mainPane) {
		GridLayout gl = new GridLayout();
		gl.numColumns = 2;
		mainPane.setLayout(gl);
		
		Group gugu = new Group(mainPane,SWT.NONE);
		gugu.setText("Batch Queue");
		gugu.setLayout(new FillLayout());
		gugu.setLayoutData(new GridData(GridData.FILL_BOTH));
		queuePane = new ScrolledComposite(gugu,SWT.H_SCROLL | SWT.V_SCROLL);
		queuePane.setBackground(queuePane.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		queue = new Table(queuePane,SWT.MULTI);
		queuePane.setContent(queue);
		TableColumn colli = new TableColumn(queue, SWT.NONE);
		colli.setWidth(COLSIZE);
		
		eddy = new TableEditor(queue);
		eddy.grabHorizontal = true;
		eddy.horizontalAlignment = SWT.LEFT;
		eddy.minimumWidth = COLSIZE;
		queue.addMouseListener(new QueueMouse());
		fillQueue();	
		
		SashForm ediLog = new SashForm(mainPane,SWT.VERTICAL);
		ediLog.setLayoutData(new GridData(GridData.FILL_BOTH));
		configureEdiLog(ediLog);
		int w[] = new int[2];
		w[0] = 60;
		w[1] = 30;
		ediLog.setWeights(w);
	}

	private void configureDnD(){
		dragSource = new DragSource(queue, dragOps);
		dragSource.setTransfer(types);
		dragSource.addDragListener(new DragSourceListener(){
			public void dragFinished(DragSourceEvent event) {
				Integer ii;
				int i;
				if(event.detail == DND.DROP_MOVE){
					//System.out.println("Killing fields");
					Object o[] = new Object[buffer.length()];
					boolean flag[] = new boolean[buffer.length()];
					for(i = 0; i < buffer.length(); i++){
						o[i] = buffer.itemAt(i);
						flag[i] = true;
					}
					Iterator it = dragSelection.iterator();
					while(it.hasNext()){
						Object oo = it.next();
						for(i = 0; i < o.length; i++){
							if(o[i] == oo){
								flag[i] = false;
							}
						}
					}
					buffer.delete(0, buffer.length());
					for(i = 0; i < o.length; i++){
						if(flag[i]){
							buffer.insert(buffer.length(), o[i]);
						}
					}
					fillQueue();
					//printBuffer("Buffer after Drop Finished");
				}
			}
			public void dragSetData(DragSourceEvent event) {
				event.data = "Hugo";
			}
			public void dragStart(DragSourceEvent event) {
				int i;
				dragSelection.clear();
				int selection[] = queue.getSelectionIndices();
				if(selection != null && selection.length > 0){
					for(i = 0; i < selection.length; i++ ){
						dragSelection.add(buffer.itemAt(selection[i]));
						//System.out.println("DragStart on " + ((BatchNode)buffer.itemAt(selection[i])).getDescription());
					}
				}
			}});
		
		dragTarget = new DropTarget(queue,dragOps);
		dragTarget.setTransfer(types);
		dragTarget.addDropListener(new DropTargetAdapter(){
			public void drop(DropTargetEvent event) {
				if(event.data == null){
					event.detail = DND.DROP_NONE;
					return;
				}
				//System.out.println("Table drop invoked");
				if(event.item == null){
					// append at end
					Iterator it = dragSelection.iterator();
					while(it.hasNext()){
						TreeNode nn = (TreeNode)it.next();
						buffer.insert(buffer.length(), batchi.cloneBatchNode(nn));
					}
					fillQueue();
				} else {
					// append somewhere
					TableItem ti = (TableItem)event.item;
					int idx = queue.indexOf(ti);
					Iterator it = dragSelection.iterator();
					while(it.hasNext()){
						TreeNode n = (TreeNode)it.next();
						idx++;
						buffer.insert(idx, batchi.cloneBatchNode(n));
					}
					fillQueue();
				}
				//printBuffer("Buffer after Drop");
			}
		});
		
		paneTarget = new DropTarget(queuePane,dragOps);
		paneTarget.setTransfer(types);
		paneTarget.addDropListener(new DropTargetAdapter(){
			public void drop(DropTargetEvent event) {
				if(event.data == null){
					event.detail = DND.DROP_NONE;
					return;
				}
				//System.out.println("Pane drop invoked");
				Iterator it = dragSelection.iterator();
				while(it.hasNext()){
					TreeNode n = (TreeNode)it.next();
					buffer.insert(buffer.length(), batchi.cloneBatchNode(n));
				}
				fillQueue();
				//printBuffer("Buffer after Drop on pane");
			}
		});
	}

	/**
	 * a private function to help debugging dreck and drop
	 * @param prefix
	 */
	private void printBuffer(String prefix){
		int i;
		BatchNode batchi;
		
		System.out.println("Buffer after " + prefix);
		for(i = 0; i < buffer.length(); i++){
			batchi = (BatchNode)buffer.itemAt(i);
			System.out.println("pos : " + i + " data = " + batchi.getDescription());
		}
	}
	protected void configureEdiLog(Composite ediLog) {

		Group gugu = new Group(ediLog,SWT.NONE);
		gugu.setText("Editor");
		gugu.setSize(new Point(200,200));
		gugu.setLayout(new FillLayout());
		editorPane = new ScrolledComposite(gugu,SWT.V_SCROLL | SWT.H_SCROLL);
		editorPane.setExpandHorizontal(true);
		editorPane.setExpandVertical(true);
		
		Group gaga = new Group(ediLog,SWT.NONE);
		gaga.setText("Log");
		gaga.setLayout(new FillLayout());
		log = new TerminalText(gaga,
				SWT.MULTI| SWT.V_SCROLL | SWT.H_SCROLL | SWT.READ_ONLY ); 
		gaga.layout(true);
	}

	private class TableFill implements IFunc{
		public Object apply(Object o) {
			BatchNode bn = (BatchNode)o;
			TableItem ti = new TableItem(queue, SWT.NONE);
			ti.setText(0, bn.getDescription());
			//System.out.println("Adding item: " + bn.getDescription());
			return null;
		}
	}
	
	protected void fillQueue() {
		queue.removeAll();
		FuncUtil.map(new SequenceStream(buffer), new TableFill());
		queue.getColumn(0).pack();
		queue.setSize(queue.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		queuePane.setMinSize(queue.computeSize(SWT.DEFAULT,SWT.DEFAULT));
	}

	public void widgetSelected(SelectionEvent e) {
		Control c = (Control)e.getSource();
		if(c == cut){
			doCut();
			return;
		} else if(c == copy){
			doCopy();
			return;
		} else if(c == paste){
			doPaste();
			return;
		} else if(c == neu){
			buffer.delete(0, buffer.length());
			fillQueue();
			Control cc = editorPane.getContent();
			if(cc != null){
				cc.dispose();
			}
			return;
		} else if( c == save){
			doSave();
			return;
		} else if(c == load){
			doLoad();
			return;
		} else if(c == start){
			doStart();
			return;
		} else if(c == interrupt){
			SysRegistry.getSystemAdapter().interrupt();
		}
	}
	private void doStart() {
		BatchBufferDialog bbd = new BatchBufferDialog(queue.getShell(),SWT.SAVE,bufferUrl);
		int status = bbd.open();
		if(status == Window.OK){
			String url = bbd.getBufferurl();
			if(url != null){
				batchi.start(url);
				bufferUrl = url;
			}
		}
	}

	private void doLoad() {
		BatchBufferDialog bbd = new BatchBufferDialog(queue.getShell(),SWT.OPEN, bufferUrl);
		int status = bbd.open();
		if(status == Window.OK){
			String url = bbd.getBufferurl();
			if(url != null){
				batchi.load(url);
				bufferUrl = url;
			}
			Control c = editorPane.getContent();
			if(c != null){
				c.dispose();
			}
		}
	}

	private void doSave() {
		BatchBufferDialog bbd = new BatchBufferDialog(queue.getShell(),SWT.SAVE, bufferUrl);
		int status = bbd.open();
		if(status == Window.OK){
			String url = bbd.getBufferurl();
			if(url != null){
				batchi.save(url);
				bufferUrl = url;
			}
		}
	}

	private void saveEditData() {
		Text tx = (Text)eddy.getEditor();
		BatchNode bn = (BatchNode)buffer.itemAt(queue.getSelectionIndex());
		try{
			bn.parse(tx.getText());
			TableItem it = queue.getItem(queue.getSelectionIndex());
			it.setText(bn.getDescription());
		}catch(BatchParseException eva){
			log.appendOutputText(eva.getMessage(),OutputStyle.ERROR );
		}
	}

	
	private void editGraphically(){
		try{
			TreeNode n = (TreeNode)buffer.itemAt(currentIndex);
			n.removeTreeListener(this);
		}catch(Exception e){}
		
		TreeNode node = (TreeNode)buffer.itemAt(queue.getSelectionIndex());
		if(node == null){
			return;
		}
		editNodeGraphically(node);
	}
	protected void editNodeGraphically(TreeNode node){
		NodeEditor ne  = null;
		Composite ed;
		
		// locate node editor 
		if(node instanceof CommandNode){
			CommandNode pn = (CommandNode)node;
			String editor = pn.getProperty("viewer");
			ne = RCPUtil.getNodeEditor(editor,"mountaingumui.commandeditor");
		} else {
			TreeNode pn = (TreeNode)node;
			// supress commandset nodes....
			String type = pn.getProperty("type");
			if(type != null && type.endsWith("commandset")){
				return;
			}
			String editor = pn.getProperty("viewer");
			ne = RCPUtil.getNodeEditor(editor,"mountaingumui.SingleParEditor");
		}
		// show node editor
		if(ne != null){
			currentIndex = queue.getSelectionIndex();
			Control c = editorPane.getContent();
			if(c != null){
				c.dispose();
			}
			try{
				if(ne instanceof CommandNodeEditor){
					ed = new BatchContainer(editorPane,ne,(TreeNode)node,  
					currentIndex, this);
				} else {
					ed = (Composite)ne.makeEditor(editorPane, node);
					node.addTreeListener(this);
				}
				editorPane.setContent(ed);
				editorPane.setMinSize(ed.computeSize(SWT.DEFAULT, SWT.DEFAULT));
			}catch(Exception eva){
				UIRegistry.getAdapter().displayError(eva.getMessage());
				eva.printStackTrace();
			}
		}
	}
	
	void updateTableNode(int idx){
		try{
			BatchNode bn = (BatchNode)buffer.itemAt(idx);
			queue.getItem(idx).setText(0, bn.getDescription());
		}catch(Exception e){}
	}
	
	private void doEdit(Widget item) {
		final TableItem it = (TableItem)item;
		Control c = eddy.getEditor();
		if(c != null){
			c.dispose();
		}
		final Text edit = new Text(queue,SWT.NONE);
		edit.setText(it.getText(0));
		Listener tl = new Listener(){
			public void handleEvent(Event event) {
				switch(event.type){
				case SWT.FocusOut:
					saveEditData();
					edit.dispose();
					break;
				case SWT.Traverse:
					switch(event.detail){
					case SWT.TRAVERSE_RETURN:
						saveEditData();
					case SWT.TRAVERSE_ESCAPE:
						edit.dispose();
						event.doit = false;
						break;
					}
				}
			}
		};
		edit.addListener(SWT.FocusOut, tl);
		edit.addListener(SWT.Traverse, tl);
		edit.selectAll();
		edit.setFocus();
		eddy.setEditor(edit, it, 0);
	}

	private void doPaste() {
		int sel = queue.getSelectionIndex() + 1;
		if(sel <= 0){
			sel = buffer.length();
		}
		if(sel > buffer.length()){
			sel = buffer.length();
		}
		//System.out.println("Pasting at " + sel);
		buffer.insert(sel, clipboard);
		fillQueue();
	}

	private void doCopy() {
		TreeNode bn;
		int selection [] = queue.getSelectionIndices();
		if(selection != null){
			clipboard.delete(0, clipboard.length());
			for(int i = 0; i < selection.length; i++){
				bn = batchi.cloneBatchNode((TreeNode)buffer.itemAt(selection[i]));
				clipboard.insert(clipboard.length(), bn);
			}
		}
	}
	private void doCut() {
		int i, length;
		int selection [] = queue.getSelectionIndices();
		if(selection != null && selection.length > 0){
			clipboard.delete(0, clipboard.length());
			/**
			 * copy everything and create a flag array
			 */
			length = buffer.length();
			Object obj[] = new Object[length];
			boolean flag[] = new boolean[length];
			for(i = 0; i < length; i++){
				obj[i] = buffer.itemAt(i);
				flag[i] = true;
			}
			/**
			 * all those to be deleted get their flag set to false
			 */
			for(i = 0; i < selection.length; i++){
				flag[selection[i]] = false;
			}
			buffer.delete(0,length);
			/**
			 * now I copy those which have a flag of true to the buffer, the 
			 * others to the clipboard.
			 */
			for(i = 0; i < length; i++){
				if(flag[i]){
					buffer.insert(buffer.length(), obj[i]);
				} else {
					clipboard.insert(clipboard.length(), obj[i]);
				}
			}
		}
		fillQueue();
	}

	public void logMessage(String txt) {
		final String t = txt;
		log.getDisplay().asyncExec(new Runnable(){
			public void run() {
				log.appendLine(t);
			}});
	}

	public void sequenceChanged(Sequence s) {
		fillQueue();
	}

	public void startingEntry(int id) {
		final int idx = id - 1;
		final BatchNode bn = (BatchNode)buffer.itemAt(idx);
		queue.getDisplay().asyncExec(new Runnable(){
			public void run() {
				if(idx < queue.getItemCount() && idx >= 0){
					queue.setSelection(idx);
					log.appendInputText(bn.getDescription());
				}
			}});
	}
	public void dispose(){
		batchi.removeBatchListener(this);
	}
	public void treeStructureChanged(TreeNode parent) {
		updateTableNode(currentIndex);
	}

	public void setFocus() {
		// Nothing to do
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

	public void nodeEnqueued(BatchNode n) {
		// Nothing to do
	}
}
