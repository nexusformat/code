/**
 * This is a class for displaying a 1D histograms in double logarithmic
 * dissplay. Using SGT.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.image.BufferedImage;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.Graph;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.SGTUtil;

import org.eclipse.swt.awt.SWT_AWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;

import gov.noaa.pmel.sgt.dm.*;

public class LogLog1DSGT extends Graph implements NodeViewer, TreeListener, SelectionListener{
	protected TreeNode graphNode;
	protected LogLogSGT lpl;
	protected Button reset, refresh;
	protected Frame embedFrame;
	
	
	public LogLog1DSGT(){
	}
	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		graphNode = n;
		lpl = new LogLogSGT();
		collectData();
		makePlot();
		graphNode.addTreeListener(this);
		Composite container = new Composite((Composite)uiData, SWT.NONE);
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 2;
		container.setLayout(gritti);
		Composite c = new Composite(container, SWT.EMBEDDED);
		c.setLayoutData(new GridData(GridData.FILL_BOTH));
		embedFrame = SWT_AWT.new_Frame(c); 
		embedFrame.add(lpl);
		makeControls(container);
		return container;
	}
	protected void makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);
				
		reset = new Button(controlPanel,SWT.PUSH);
		reset.setText("Reset Zoom");
		reset.addSelectionListener(this);
		reset.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		refresh = new Button(controlPanel,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);
		refresh.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

	}
	private class CollectFunc implements IFunc {

		public Object apply(Object o) {
			if(!(o instanceof ParameterNode)){
				return null;
			}
			ParameterNode n = (ParameterNode)o;
			String type = n.getProperty("type");
			if(type != null){
				if(type.equals("axis")){
					setXAxis(new GraphAxis(n.getName(),n));
					//System.out.println("Added x-axis: " + n.getName());
				} else if(type.equals("data")){
					addData(new GraphData(n.getName(),n));
					//System.out.println("Added data: " + n.getName());
				}
			}
			return null;
		}
	}
	private void collectData() {
		clearAll();
		FuncUtil.map(new ChildStream(graphNode), new CollectFunc());
	}

	public void changeData(GraphData g) {
		makePlot();
	}
	private class SGTPlot implements IFunc {
		private int count;
		
		SGTPlot(){
			count = 0;
		}
		public Object apply(Object o) {
			SGTMetaData xMeta, yMeta;
			SimpleLine sl = null;
			
			GraphData gd = (GraphData)o;
			GraphAxis gx = getXAxis();
			if(gx == null){
				return null;
			}
			if(gx.getData().length != gd.getData().length){
				return null;
			}
			xMeta = new SGTMetaData(gx.getAxisName(),"");
			yMeta = new SGTMetaData(gd.getName(),"");
			sl = new SimpleLine(gx.getDataLogWashed(), gd.getDataLogWashed(),
						gd.getName());
			sl.setXMetaData(xMeta);
			sl.setYMetaData(yMeta);
			lpl.addData(sl, gd.getTitle(),gx.getAxisName(), gd.getName());
			return null;
		}
	}
	public void makePlot() {
		if(!isValid()) {
			return;
		}
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				lpl.setBatch(true);
				FuncUtil.map(new IteratorStream(data.values()), new SGTPlot());
				lpl.setBatch(false);
				lpl.repaint();			}
		});
	}
	public void processNode(TreeNode n) {
		// defunct
	}
	public void treeStructureChanged(TreeNode parent) {
		collectData();
		makePlot();
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
	public void widgetSelected(SelectionEvent e) {
		Button c = (Button)e.getSource();
		if(c == refresh){
			refresh();
			return;
		}
		if(c == reset){
			lpl.resetZoom();
		}
	}
	public BufferedImage makeImage(int width, int height) {
		return SGTUtil.drawInImage(lpl,width,height);
	}
	public void print(Printer p) {
		SGTUtil.print(lpl, p);
	}
}
