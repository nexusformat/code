/**
 * This is specialization of a viewer for POLDI which allows to switch 
 * the correlation diagram between Q and D display.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2008
 */
package ch.psi.num.mountaingum.poldi;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.GraphicsView.CommandGraph1D;

public class PoldiViewer extends CommandGraph1D {
	private Button qMode, dMode;
	
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
	
		Group axGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		axGroup.setLayout(new FillLayout(SWT.VERTICAL));
		axGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		axGroup.setText("X-Axis Type");
		qMode = new Button(axGroup, SWT.RADIO);
		qMode.setText("Q");
		qMode.setSelection(false);
		qMode.addSelectionListener(this);
		dMode = new Button(axGroup,SWT.RADIO);
		dMode.setText("D");
		dMode.setSelection(true);
		dMode.addSelectionListener(this);
		
		reset = new Button(controlPanel,SWT.PUSH);
		reset.setText("Reset Zoom");
		reset.addSelectionListener(this);
		reset.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		refresh = new Button(controlPanel,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);
		refresh.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		lpl.linekeyVisible(false);
	}
	private class CollectPoldi implements IFunc {

		public Object apply(Object o) { 
			if(!(o instanceof ParameterNode)){
				return null;
			}
			ParameterNode n = (ParameterNode)o;
			String type = n.getProperty("type");
			if(type != null){
				if(type.equals("axis")){
					setXAxis(new PoldiAxis(n.getName(),n));
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
		FuncUtil.map(new ChildStream(graphNode), new CollectPoldi());
	}
	public void widgetSelected(SelectionEvent e) {
		Button c = (Button)e.getSource();
		double data[];
		
		if(c == qMode){
			if(c.getSelection() == true){
				setAxisMode(PoldiAxis.DMODE);
			}
			return;
		}
		if(c == dMode){
			if(c.getSelection() == true){
				setAxisMode(PoldiAxis.QMODE);
			}
			return;
		}
		super.widgetSelected(e);
	}
	private void setAxisMode(int tof2) {
		PoldiAxis f = (PoldiAxis)getXAxis();
		f.setAxisMode(tof2);
		lpl.resetZoom();
		makePlot();
	}
}
