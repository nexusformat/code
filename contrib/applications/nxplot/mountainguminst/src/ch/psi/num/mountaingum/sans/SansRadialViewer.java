/**
 * This is a special viewer for SANS which shows the radial average over 
 * the 2D detector. It tries to find the 2D detector data, registers itself 
 * as an update listener and recalculates the radial average each time 
 * new 2D detector data arrives.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.sans;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.GraphicsView.LogLog1DSGT;

public class SansRadialViewer extends LogLog1DSGT implements UpdateListener {
	public final static String ID = "mountaingumui.SansRadialViewer";
	protected RadialAverage radi;
	protected COG2D cogi;
	protected ParameterNode countNode, lambdaNode, distanceNode;
	protected int xdim, ydim;
	protected GraphAxis xaxis;
	protected GraphData data;
	protected Button cog, manual, apply;
	protected Text xcenter, ycenter;
	protected int manualXCenter, manualYCenter;
	protected boolean cogCenter; 
	
	public SansRadialViewer(){
		super();
		radi = new RadialAverage();
		cogi = new COG2D();
		xaxis = null;
		data = null;
		cogCenter = true;
	}

	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		try{
			ParameterNode link = (ParameterNode)TreeUtil.findChild(n,"2ddata");
			String path = link.getValue().toString();
			countNode = (ParameterNode)TreeUtil.searchNode(TreeUtil.findRoot(n),path);
			countNode.addUpdateListener(this);
			ParameterNode dim = (ParameterNode)TreeUtil.findChild(n,"2ddim");
			double ddim[] = TreeUtil.getNodeValueAsDouble(dim);
			xdim = (int)ddim[0];
			ydim = (int)ddim[1];
			dim = (ParameterNode)TreeUtil.findChild(n,"2dpix");
			ddim = TreeUtil.getNodeValueAsDouble(dim);
			radi.setPixelSize(ddim[0], ddim[1]);
			lambdaNode = (ParameterNode)TreeUtil.findChild(n,"lambdapath");
			path = lambdaNode.getValue().toString();
			lambdaNode = (ParameterNode)TreeUtil.searchNode(TreeUtil.findRoot(n),path);
			distanceNode = (ParameterNode)TreeUtil.findChild(n,"distancepath");
			path = distanceNode.getValue().toString();
			distanceNode = (ParameterNode)TreeUtil.searchNode(TreeUtil.findRoot(n),path);
		}catch(Exception eva){
			SysRegistry.getLogger().exception(eva);
			return null;
		}
		Object o = super.makeEditor(uiData, n);
		nodeUpdated(countNode);
		return o;
	}


	public void nodeUpdated(TreeNode n) {
		double q[] = null, c[];
		
		double d[] = TreeUtil.getNodeValueAsDouble(countNode);

		if(cogCenter){
			cogi.calculate(d, xdim, ydim);
			radi.setCenter((int)cogi.x, (int)cogi.y);
			xcenter.getDisplay().asyncExec(new Runnable() {
				public void run() {
					xcenter.setText(Integer.toString((int)cogi.x));
					ycenter.setText(Integer.toString((int)cogi.y));
				}});
		} else {
			radi.setCenter(manualXCenter, manualYCenter);
		}
		try{
			radi.calculate(d, xdim, ydim);
		}catch(Exception eva){
			SysRegistry.getLogger().exception(eva);
		}
		if(xaxis == null){
			xaxis = getXAxis();
			data = getData("counts");
		}
		DoubleValue dv = (DoubleValue)xaxis.getNode().getValue();
		double lambda = TreeUtil.getNodeValue(lambdaNode, 0);
		double distance = TreeUtil.getNodeValue(distanceNode, 0);
		if(lambda < .1){
			lambda = 1.0;
		}
		if(distance < .1){
			distance = 1000;
		}
		try{
			/*
			 * This is a special feature for SANS2 where they have the distance in m 
			 * instead of normal mm
			 */
			if(distanceNode.getProperty("multiply") != null){
				distance *= 100.;
			}
			q = radi.getQ(lambda, distance);
		}catch(Exception eva){}
		/*
		 * we shorten the arrays by 3 in order to cut off the uninteresting 
		 * bins just close to the beam position.
		 */
		dv.setData(shorten(q,3));
		dv = (DoubleValue)data.getNode().getValue();
		dv.setData(shorten(radi.getCounts(),3));
		makePlot();
	}
	private double[] shorten(double[] q, int i) {
		double result[] = new double[q.length - i];
		System.arraycopy(q, i, result, 0, q.length  -i);
		return result;
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}

	protected void makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);
		
		Group center = new Group(controlPanel, SWT.NONE);
		center.setLayout(new FillLayout(SWT.VERTICAL));
		center.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		center.setText("Center");
		cog = new Button(center, SWT.RADIO);
		cog.setText("COG");
		cog.setSelection(true);
		cog.addSelectionListener(this);
		manual = new Button(center,SWT.RADIO);
		manual.setText("Manual");
		manual.addSelectionListener(this);
		Label l  = new Label(center,SWT.NONE);
		l.setText("Center Input: x y");
		Composite xy = new Composite(center, SWT.NONE);
		xy.setLayout(new FillLayout(SWT.HORIZONTAL));
		xcenter = new Text(xy, SWT.SINGLE);
		ycenter = new Text(xy, SWT.SINGLE);
		apply = new Button(center,SWT.PUSH);
		apply.setText("Apply Center");
		apply.addSelectionListener(this);
		
		reset = new Button(controlPanel,SWT.PUSH);
		reset.setText("Reset Zoom");
		reset.addSelectionListener(this);
		reset.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		refresh = new Button(controlPanel,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);
		refresh.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));


	}

	public void widgetSelected(SelectionEvent e) {
		Object o = e.getSource();
		if(o == cog){
			cogCenter = true;
			nodeUpdated(countNode);
			return;
		} else if(o == manual){
			cogCenter = false;
			readManualCenter();
			nodeUpdated(countNode);
			return;
		} else if(o == apply){
			if(cogCenter){
				cog.setSelection(false);
				manual.setSelection(true);
			}
			cogCenter = false;
			readManualCenter();
			nodeUpdated(countNode);
			return;
		} else if(o == refresh){
			countNode.queryValue();
			return;
		}
		super.widgetSelected(e);
	}

	private void readManualCenter() {
		int x = -1, y = -1;
		try{
			x = (int)Double.parseDouble(xcenter.getText());
			y = (int)Double.parseDouble(ycenter.getText());
		}catch(Exception eva){
			SysRegistry.getLogger().exception(eva);
		}
		if(x < 0 || x > xdim){
			x = xdim/2;
		}
		if(y < 0 || y > xdim){
			y = ydim/2;
		}
		manualXCenter = x;
		manualYCenter = y;
	}
}
