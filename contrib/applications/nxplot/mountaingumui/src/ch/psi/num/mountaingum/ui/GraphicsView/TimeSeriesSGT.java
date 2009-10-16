/**
 * This is a NodeEditor for viewing a time series, for instance a log of temperature.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import gov.noaa.pmel.sgt.dm.SGTMetaData;
import gov.noaa.pmel.sgt.dm.SimpleLine;
import gov.noaa.pmel.sgt.swing.JPlotLayout;
import gov.noaa.pmel.util.GeoDateArray;

import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.image.BufferedImage;
import java.util.Calendar;

import org.eclipse.swt.SWT;
import org.eclipse.swt.awt.SWT_AWT;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Spinner;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.LogListener;
import ch.psi.num.mountaingum.tree.LongValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.Graph;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.SGTUtil;

public class TimeSeriesSGT extends Graph implements LogListener, NodeViewer,
		SelectionListener, DisposeListener {
	public final static String ID = "mountainugumui.TimeSeries";

	protected final static String rangeList[] = {
		"1m", "2m", "5m", "10m", "30m", "1h", "2h", "4h", "8h", "16h", "1d", "2d", 
		"4d", "8d"
	};
	protected final static int rangeInSec[] = {
		60, 120, 300, 600, 1800, 3600, 7200, 14400, 28800, 
		57600, 86400, 172800, 345600, 691200
	};
	
	protected TreeNode graphNode;
	protected int timeIntervall;
	protected JPlotLayout plot;
	protected Combo range;
	protected Spinner updateIntervall;
	protected int updateIntervallValue;
	protected Button zoom;
	protected long nextUpdate;
	protected Thread updater;
	protected boolean loading;
	/*
	 * normally we wish to display data starting as of now. But there is 
	 * the option to view historic data, too. These flags take care of this.
	 */
	protected boolean useCurrentDate;
	protected long startTime;
	
	protected Text feedback;
	
	private class Uppi implements Runnable {
		public void run() {
			while(true){
				if(System.currentTimeMillis()/1000 > nextUpdate && !loading){
					loadNewData();
				} else {
					try{
						// this gives 2 seconds resolution
						Thread.currentThread().sleep(2000);
					}catch(Exception eva){}
				}
			}
		}
	}
	protected void loadNewData() {
		long to, from;
		
		if(useCurrentDate){
			to = System.currentTimeMillis()/1000;
		} else {
			to = startTime;
		}
		/*
		 * This is for debugging  and demonstration only.
		 * Set to  false for live data
		 */
		if(false){
			Calendar c = Calendar.getInstance();
			/* 
			 *  month values are 0 based! But days not!
			 *  I really need a gun! A big one!
			 */ 
			c.set(2006,06,20,15,00);
			to = c.getTimeInMillis()/1000;
		}
		
		from = to - timeIntervall;
		CommandNode cn = (CommandNode)TreeUtil.findChild(graphNode, "getdata");
		ParameterNode pn = (ParameterNode)TreeUtil.findChild(cn, "starttime");
		pn.getValue().setFromString(Long.toString(from));
		pn = (ParameterNode)TreeUtil.findChild(cn, "endtime");
		pn.getValue().setFromString(Long.toString(to));
		cn.start();
		loading = true;
	}
	
	private class PlotSeries implements IFunc {
		public Object apply(Object o) {
			TreeNode tn = (TreeNode)o;
			String tst = tn.getProperty("timeseries");
			if(tst != null && tst.equalsIgnoreCase("true")){
				String name = tn.getName();
				ParameterNode time = (ParameterNode)TreeUtil.findChild(tn,"time");
				ParameterNode val = (ParameterNode)TreeUtil.findChild(tn, "values");
				if(time == null && val == null){
					SysRegistry.getLogger().error("Malformed timeseries node");
					return null;
				}
				LongValue lv = (LongValue)time.getValue();
				DoubleValue dv = (DoubleValue)val.getValue();
				/*
				 * GeoDateArray does time in milliseconds but we get it in 
				 * seconds. Sighhhhhhh :-(
				 * BTW: this may belong into the layer interpreting the replies 
				 * from the underlying system. Depends on how one defines time. 
				 * This now is unix time in seconds. 
				 */
				long timeFix[] = lv.getData();
				for(int i = 0; i < timeFix.length; i++){
					timeFix[i] *= 1000;
				}
				GeoDateArray taxis = new GeoDateArray(timeFix);
				SimpleLine sl = new SimpleLine(taxis,dv.getData(), name);
				SGTMetaData xMeta = new SGTMetaData("Time","");
				sl.setXMetaData(xMeta);
				SGTMetaData yMeta = new SGTMetaData("Values","");
				sl.setYMetaData(yMeta);
				if(dv.getLength() > 0 && lv.getLength() > 0){
					plot.addData(sl,name);
				} else {
					addLogData(tn,"No data for " + name, false);
				}
			}
			return null;
		}
		
	}
	public void makePlot() {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				plot.clear();
				plot.setBatch(true);
				FuncUtil.map(new ChildStream(graphNode), new PlotSeries());
				plot.setBatch(false);
				plot.repaint();
			}});
	}

	public void addLogData(TreeNode node, String txt, boolean success) {
		if(success){
			makePlot();
		} else {
			final String t = txt; 
			feedback.getDisplay().asyncExec(new Runnable(){
				public void run() {
					feedback.append(t);
					if(t.indexOf('\n') < 0){
						feedback.append("\n");
					}
				}});
		}
		nextUpdate = System.currentTimeMillis()/1000 + updateIntervallValue;;
		loading = false;
	}

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		graphNode = n;
		timeIntervall = 30*60; //default 30 minutes

		Composite container = new Composite(parent, SWT.NONE);
		container.addDisposeListener(this);
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 2;
		container.setLayout(gritti);
		Composite c = new Composite(container, SWT.EMBEDDED);
		c.setLayoutData(new GridData(GridData.FILL_BOTH));
		Frame embedFrame = SWT_AWT.new_Frame(c); 
		plot = new JPlotLayout(false, true, false,"MG Time", null, false);
		plot.setTitles("", "", "");
		embedFrame.add(plot);
		makeControls(container);
		nextUpdate = 0;
		loading = false;
		updater = new Thread(new Uppi());
		updater.start();
		updateIntervallValue = 20;
		CommandNode cn = (CommandNode)TreeUtil.findChild(graphNode, "getdata");
		cn.addLogListener(this);
		return container;
	}

	private void makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);
		
		Group r = new Group(controlPanel,SWT.NONE);
		r.setText("Range");
		r.setLayout(new FillLayout());
		r.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		range = new Combo(r,SWT.SIMPLE| SWT.READ_ONLY);
		for(int i = 0; i < rangeList.length; i++){
			range.add(rangeList[i]);
		}
		range.select(4);
		range.addSelectionListener(this);
		
		Group up = new Group(controlPanel,SWT.NONE);
		up.setText("Update Intervall");
		up.setLayout(new FillLayout());
		up.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		updateIntervall = new Spinner(up,SWT.NONE);
		updateIntervall.setSelection(20);
		updateIntervall.addSelectionListener(this);
		
		zoom = new Button(controlPanel,SWT.PUSH);
		zoom.setText("Reset Zoom");
		zoom.addSelectionListener(this);
		zoom.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		Group fb = new Group(controlPanel, SWT.NONE);
		fb.setText("Feedback");
		fb.setLayout(new FillLayout());
		fb.setLayoutData(new GridData(GridData.FILL_BOTH));
		feedback = new Text(fb,SWT.MULTI);
	}

	public void widgetSelected(SelectionEvent e) {
		Object o = e.getSource();
		if(o == zoom){
			EventQueue.invokeLater(new Runnable(){
				public void run() {
					plot.resetZoom();
					plot.repaint();
				}});
		} else if( o == range){
			int idx = range.getSelectionIndex();
			timeIntervall = rangeInSec[idx];
			loadNewData();
		} else if(o == updateIntervall){
			updateIntervallValue = updateIntervall.getSelection();
		}
		
	}
	public void widgetDisposed(DisposeEvent e) {
		CommandNode cn = (CommandNode)TreeUtil.findChild(graphNode, "getdata");
		cn.removeLogListener(this);
		updater.stop();		
	}
	public BufferedImage makeImage(int width, int height) {
		return SGTUtil.drawInImage(plot,width,height);
	}
	public void print(Printer p) {
		SGTUtil.print(plot, p);
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
	public void commandSent(TreeNode tn, String command) {
		// Nothing to do
	}
	public void deviceFinished(TreeNode node, boolean success) {
		// TODO Nothing to
	}
	public void deviceStarted(TreeNode node) {
		// Nothing to do
	}
	public void changeData(GraphData g) {
		makePlot();
	}
}
