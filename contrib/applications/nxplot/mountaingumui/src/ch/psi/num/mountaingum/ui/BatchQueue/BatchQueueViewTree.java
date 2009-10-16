/**
 * This is derived class of BatchQueuView which just arranges the queue on the right
 * side to make space in the perspective on the left for a tree display.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, November 2008
 */
package ch.psi.num.mountaingum.ui.BatchQueue;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.custom.TableEditor;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import ch.psi.num.mountaingum.batch.BatchNode;
import ch.psi.num.mountaingum.tree.TreeNode;


public class BatchQueueViewTree extends BatchQueueView {
	public static final String ID = "mountaingumui.BatchQueueViewTree";

	/**
	 * This does the same as the corresponding method in BatchQueueView, just that 
	 * the queue is at the right hand side.
	 */
	protected void configureMainPane(Composite mainPane) {
		int w[] = new int[2];
		GridLayout gl = new GridLayout();
		gl.numColumns = 3;
		gl.makeColumnsEqualWidth = false;
		mainPane.setLayout(gl);
		
		SashForm ediLog = new SashForm(mainPane,SWT.VERTICAL);
		GridData gd = new GridData(GridData.FILL_BOTH);
		gd.horizontalSpan = 2;
		ediLog.setLayoutData(gd);
		configureEdiLog(ediLog);
		w[0] = 60;
		w[1] = 30;
		ediLog.setWeights(w);
		
		Group gugu = new Group(mainPane,SWT.NONE);
		gugu.setText("Batch Queue");
		gugu.setLayout(new FillLayout());
		gugu.setLayoutData(new GridData(GridData.FILL_VERTICAL));
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
	}

	
	public void createPartControl(Composite parent) {
		int w[] = new int[2];
		super.createPartControl(parent);
		w[0] = 60;
		w[1] = 30;
		mainPane.setWeights(w);
	}


	public void nodeEnqueued(BatchNode n) {
		fillQueue();
		int idx = queue.getItemCount();
		queue.setSelection(idx-1);
		editNodeGraphically((TreeNode)n);
	}
}
