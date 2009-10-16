/**
 * This is a specialized viewer for the FOCUS instrument @ PSI.
 * It allows to change the TOF axis to energy etc. This adds 
 * controls which allow to switch the axis mode. The actual 
 * calculation is relegated to a special FocusAxis. An 
 * overridden collectData takes care that this special 
 * axis is indeed created on initialisation. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2008
 */
package ch.psi.num.mountaingum.focus;

import gov.noaa.pmel.util.Domain;
import gov.noaa.pmel.util.Range2D;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphicsView.Graph2DProj;
import ch.psi.num.mountaingum.ui.GraphicsView.GraphData2D;

public class FocusViewer extends Graph2DProj implements UpdateListener {
	final static int TOF = 1;
	static final int ENERGY = 2;
	private int tofMode;
	private Label dataNum;
	
	private Button tofSelected, energySelected;
	
	public FocusViewer(){
		super();
		tofMode = TOF; 
	}
	
	protected Composite makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);

        make2DControls(controlPanel);
	
        makeProjectionControl(controlPanel);

		Group axisMode = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		axisMode.setLayout(new FillLayout(SWT.VERTICAL));
		axisMode.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		axisMode.setText("TOF Axis Mode");
		tofSelected = new Button(axisMode, SWT.RADIO);
		tofSelected.setText("time-of-flight");
		tofSelected.setSelection(true);
		tofSelected.addSelectionListener(this);
		energySelected = new Button(axisMode,SWT.RADIO);
		energySelected.setText("energy");
		energySelected.addSelectionListener(this);
		
		Group numour = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		numour.setLayout(new FillLayout(SWT.VERTICAL));
		numour.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		numour.setText("Numor");
		dataNum = new Label(numour,SWT.NONE);
		dataNum.setText("Unknown");
		ParameterNode pn = (ParameterNode)TreeUtil.nodeForPath("/instrument/experiment/datafilenumber");
		if(pn != null){
			pn.addUpdateListener(this);
			nodeUpdated(pn);
		}
		
		reset = new Button(controlPanel,SWT.PUSH);
		reset.setText("Reset Zoom");
		reset.addSelectionListener(this);
		reset.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		refresh = new Button(controlPanel,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);
		refresh.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		return controlPanel;
	}

	public void widgetSelected(SelectionEvent e) {
		double data[];
		Object c = e.getSource();
		Button cb = null;

		if(c == tofSelected){
			cb = (Button)e.getSource();
			if(cb.getSelection() == true){
				setAxisMode(TOF);
			}
			return;
		}
		if(c == energySelected){
			cb = (Button)e.getSource();
			if(cb.getSelection() == true){
				setAxisMode(ENERGY);
			}
			return;
		}
		if(c == reset){
			cb = (Button)e.getSource();
			if(tofMode == ENERGY){
				/*
				 * this is a workaround for a problem
				 * with resetZoom in SGT.
				 */
				data = getYAxis().getData();
				Range2D yr = new Range2D();
				yr.start = data[0];
				yr.end = data[data.length -1];
				data = null;
				data = getXAxis().getData();
				Range2D xr = new Range2D();
				xr.start = data[0];
				xr.end = data[data.length - 1];
				data = null;
				zoomDomain = new Domain(xr,yr);
				makePlot();
				return;
			} else {
				super.widgetSelected(e);
			}
		}
		super.widgetSelected(e);
	}
	private void setAxisMode(int tof2) {
		double data[];
		
		tofMode = tof2;
		FocusAxis f = (FocusAxis)getXAxis();
		f.setAxisMode(tof2);
		Range2D yr = zoomDomain.getYRange();
		data = getXAxis().getData();
		Range2D xr = new Range2D();
		xr.start = data[0];
		xr.end = data[data.length - 1];
		data = null;
		zoomDomain = new Domain(xr,yr);
		makePlot();
	}
	class CollectFocus implements IFunc {

		public Object apply(Object o) {
			if( !(o instanceof ParameterNode)){
				return null;
			}
			ParameterNode n = (ParameterNode)o;
			String type = n.getProperty("type");
			if(type != null){
				if(type.equals("axis")){
					String dim = n.getProperty("dim");
					if(dim != null){
						if(dim.indexOf("0") >= 0){
							setXAxis(new FocusAxis(n.getName(),n));
						} else {
							setYAxis(new GraphAxis(n.getName(),n));
						}
					}
				} else if(type.equals("data")){
					addData(new GraphData2D(n.getName(),n));
					dataName = n.getName();
				}
			}
			return null;
		}
		
	}

	protected void collectData() {
		clearAll();
		FuncUtil.map(new ChildStream(graphNode), new CollectFocus());
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}

	public void nodeUpdated(TreeNode n) {
		ParameterNode pn = (ParameterNode)n;
		double val = TreeUtil.getNodeValue(pn, 0);
		int iv = (int)val;
		final String t = Integer.toString(iv);
		dataNum.getDisplay().asyncExec(new Runnable(){
			public void run() {
				dataNum.setText(t);
			}});
	}
}
