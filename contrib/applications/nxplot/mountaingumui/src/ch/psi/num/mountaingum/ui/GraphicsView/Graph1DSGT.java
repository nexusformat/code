/**
 * This is the default  class for displaying 1D histograms. It is based on the 
 * SGT visualisation package.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import java.awt.Color;
import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

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
import ch.psi.num.mountaingum.sys.SysRegistry;
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

import gov.noaa.pmel.sgt.LineAttribute;
import gov.noaa.pmel.sgt.dm.*;
import gov.noaa.pmel.util.Domain;

public class Graph1DSGT extends Graph implements NodeViewer, TreeListener, SelectionListener, 
	PropertyChangeListener{
	private static final Color[] colorList =
	  { Color.blue, Color.cyan.darker(), Color.green, Color.orange.darker(),
	    Color.red, Color.magenta, Color.black, Color.gray};
	private static final int[] markList =
	  {1, 2, 9, 15, 10, 24, 11, 44};

	protected TreeNode graphNode;
	protected SGTLineLayout lpl;
	protected Button line, scatter, log, normal, reset, refresh;
	protected boolean linePlot;
	protected Frame embedFrame;
	protected Domain zoomDomain;
	
	public Graph1DSGT(){
		linePlot = true;
		zoomDomain = null;
	}
	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		graphNode = n;
		lpl = new SGTLineLayout("MountaingumPlot",null,false);
		lpl.addPropertyChangeListener(this);
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
		lpl.setTitles(graphNode.getName(), "", "");
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
		Group style = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		style.setLayout(new FillLayout(SWT.VERTICAL));
		style.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		style.setText("Plot Style");
		line = new Button(style,SWT.RADIO);
		line.setText("Lines");
		line.setSelection(true);
		line.addSelectionListener(this);
		scatter = new Button(style,SWT.RADIO);
		scatter.setText("Scatter");
		scatter.addSelectionListener(this);
		
		Group logGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		logGroup.setLayout(new FillLayout(SWT.VERTICAL));
		logGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		logGroup.setText("Axis Type");
		normal = new Button(logGroup, SWT.RADIO);
		normal.setText("Normal");
		normal.setSelection(true);
		normal.addSelectionListener(this);
		log = new Button(logGroup,SWT.RADIO);
		log.setText("log");
		log.addSelectionListener(this);
		
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
	protected void collectData() {
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
			LineAttribute latt;
			SimpleLine sl = null;
			
			if(linePlot){
				latt = new LineAttribute(LineAttribute.SOLID, colorList[count%8]);
			} else {
				latt = new LineAttribute(LineAttribute.MARK, markList[count%8], 
						colorList[count%8]);
			}
			GraphData gd = (GraphData)o;
			GraphAxis gx = getXAxis();
			if(gx == null){
				return null;
			}
			if(gx.getData().length != gd.getData().length){
				return null;
			}
			xMeta = new SGTMetaData(gx.getAxisName(),"");
			yMeta = new SGTMetaData(gd.getAxisName(),"");
			if(lpl.isYlog()){
				sl = new SimpleLine(gx.getData(), gd.getDataLogWashed(),
						gd.getName());
			} else {
				sl = new SimpleLine(gx.getData(), gd.getData(), gd.getName());
			}
			sl.setXMetaData(xMeta);
			sl.setYMetaData(yMeta);
			lpl.addData(sl, latt, gd.getName());
			lpl.setTitles(gd.getTitle(), "", "");
			count++;
			return null;
		}
	}
	public void makePlot() {
		if(!isValid()) {
			return;
		}
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try{
					lpl.clear();
					lpl.setBatch(true);
					FuncUtil.map(new IteratorStream(data.values()), new SGTPlot());
					lpl.setBatch(false);
					lpl.repaint();
				}catch(Exception eva){
					SysRegistry.getLogger().exception(eva);
				}
			}
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
		if(c == line){
			linePlot = true;
			makePlot();
			return;
		} 
		if(c == scatter ){
			linePlot = false;
			makePlot();
			return;
		}
		if(c == normal){
			lpl.setYlog(false);
			makePlot();
			return;
		}
		if(c == log){
			lpl.setYlog(true);
			makePlot();
			return;
		}
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
	public void propertyChange(PropertyChangeEvent evt) {
		String name = evt.getPropertyName();
		if(name.equals("zoomRectangle")){
			Rectangle zm = (Rectangle)evt.getNewValue();
			if(zm.width <= 1 || zm.height <= 1 ){
				return;
			}
			zoomDomain = lpl.getRange();
		}
		if(name.equals("domainRange")){
			zoomDomain = lpl.getRange();
		}
	}
}
