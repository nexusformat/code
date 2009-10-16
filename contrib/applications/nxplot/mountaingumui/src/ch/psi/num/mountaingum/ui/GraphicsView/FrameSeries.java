/**
 * This is a viewer for a series of frames as produced by a scan across 
 * an area detector. Or as a general viewer for 3D data. This viewer 
 * requires a special tree structure as described below. Please note that
 * node names are not always important but the types are.
 *  
 *  -- counts, type=data, viewer=frameseries
 *  -- rank = 2
 *  -- x, axis, as usual
 *  -- y  "      "
 *  -- currentData
 *  -- getoldframe type=command
 *  ------ framenumber 
 *  -- currentFrame 
 *  
 *  This is how this ought to work: There are basically two modes of operation:
 *  * We are viewing the current detector data
 *  * We are skipping through old frames
 *  This is handled like this:
 *  
 *  - counts, or the node with the type graphdata is actually being displayed.
 *  - currentFrame is the current number of frames available. This may vary during a
 *    scan
 *  - currentData is current actual histogram memory data. When viewing current 
 *    detector data an registered updateListener on this will copy the data into 
 *    the node with the graphdata. Which is the one to be displayed.
 *  - getoldframe, together with its parameter framenumber are called when an old 
 *    frame is requested. The commands task is to copy the appropriate data onto 
 *    the node which is actually being displayed.
 *    
 *  Copyright: GPL
 *  
 *  Mark Koennecke, October 2007
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class FrameSeries extends Graph2DSGT {
	protected boolean showingCurrentFrame;
	protected int frameNumber, maxFrameNum;
	protected Button start, down10, down, current, up, up10, end; 
	protected Text frameNum;
	
	public FrameSeries(){
		super();
		showingCurrentFrame = true;
	}
	private class AnimationControl implements SelectionListener {
		public void widgetDefaultSelected(SelectionEvent e) {
			// Nothing to do here
		}
		public void widgetSelected(SelectionEvent e) {
			Control c = (Control)e.getSource();
			if(c == frameNum){
				String fn = frameNum.getText();
				int num = Integer.parseInt(fn);
				if(num < 0 ){
					num = 0;
				} 
				if(num > maxFrameNum){
					num = maxFrameNum;
				}
				setFrameNum(num);
				return;
			}
			if(c == start){
				setFrameNum(0);
				return;
			}
			if(c == end){
				setFrameNum(maxFrameNum);
				return;
			}
			if(c == down10){
				increment(-10);
				return;
			}
			if(c == down){
				increment(-1);
				return;
			}
			if(c == up10){
				increment(10);
				return;
			}
			if(c == up){
				increment(1);
				return;
			}
		}
		private void increment(int num){
			frameNumber += num;
			if(frameNumber < 0){
				frameNumber = 0;
			}
			if(frameNumber >= maxFrameNum){
				frameNumber = maxFrameNum;
			}
			setFrameNum(frameNumber);
		}
	};
	private class MaxNumUpdater implements UpdateListener  {
		public void nodeStatusChanged(TreeNode node) {
			// Nothing to do here
		}
		public void nodeUpdated(TreeNode n) {
			ParameterNode pn = (ParameterNode)n;
			IntValue nv = (IntValue)pn.getValue();
			final int num = nv.getData()[0];
			maxFrameNum = num;
			if(showingCurrentFrame){
				Display d = frameNum.getDisplay();
				d.asyncExec(new Runnable(){
					public void run() {
						frameNum.setText(Integer.toString(num));
						frameNum.getParent().layout(true);
					}
				});
				ParameterNode pd = (ParameterNode)
					TreeUtil.findChild(n.getParent(), "currentData");
				pd.queryValue();
			}
		}
	}
	private class CurrentFrameUpdater implements UpdateListener{
		public void nodeStatusChanged(TreeNode node) {
			// Nothing to do
		}
		public void nodeUpdated(TreeNode n) {
			if(showingCurrentFrame){
				ParameterNode pn = (ParameterNode)n;
				ParameterNode target = (ParameterNode)
					TreeUtil.findChild(pn.getParent(), dataName);
				TreeUtil.copyNodeValue(pn, target);
			}
		}
	}
	public Object makeEditor(Composite uiData, TreeNode n) throws Exception{
		Composite viewer = new Composite((Composite)uiData,SWT.NONE);
		viewer.setLayout(new GridLayout());
		Composite view2d = (Composite)super.makeEditor(viewer, n);
		view2d.setLayoutData(new GridData(GridData.FILL_BOTH));
		
		AnimationControl ac = new AnimationControl();
		Composite butRow = new Composite(viewer,SWT.NONE);
		RowLayout butLay = new RowLayout(SWT.HORIZONTAL);
		butLay.fill = true;
		butRow.setLayout(butLay);
		start = new Button(butRow,SWT.PUSH);
		start.setText("0");
		start.addSelectionListener(ac);

		down10 = new Button(butRow,SWT.PUSH);
		down10.setText("<<");
		down10.addSelectionListener(ac);

		down = new Button(butRow,SWT.PUSH);
		down.setText("<");
		down.addSelectionListener(ac);
		
		frameNum = new Text(butRow,SWT.CENTER);
		frameNum.setText("00000");
		frameNum.addSelectionListener(ac);
		
		up = new Button(butRow,SWT.PUSH);
		up.setText(">");
		up.addSelectionListener(ac);

		up10 = new Button(butRow,SWT.PUSH);
		up10.setText(">>");
		up10.addSelectionListener(ac);

		end = new Button(butRow,SWT.PUSH);
		end.setText("max");
		end.addSelectionListener(ac);

		MaxNumUpdater mnu = new MaxNumUpdater();
		ParameterNode num = (ParameterNode)TreeUtil.findChild(n, "currentFrame");
		if(num == null){
			throw new Exception("Malformed tree, missing currentFrame");
		}
		num.addUpdateListener(mnu);
		
		CurrentFrameUpdater cfu = new CurrentFrameUpdater();
		ParameterNode fn = (ParameterNode)TreeUtil.findChild(n, "currentData");
		if(fn == null){
			throw new Exception("Malformed tree, missing currentData");
		}
		fn.addUpdateListener(cfu);
		
		IntValue iv = (IntValue)num.getValue();
		try{
			maxFrameNum = iv.getData()[0];
		}catch(Exception eva){
			maxFrameNum = 0;
		}
		setFrameNum(maxFrameNum);
		
		return viewer;
	}
	private void setFrameNum(int num) {
		if(num != maxFrameNum){
			showingCurrentFrame = false;
		} else {
			showingCurrentFrame = true;
		}
		/*
		System.out.println("setFrameNum(" + num +"), max = " + maxFrameNum +
				" state = " + showingCurrentFrame);
		*/
		ParameterNode pp = (ParameterNode)TreeUtil.searchNode(graphNode,
				"getoldframe/framenumber");
		pp.setProperty("commandValue",Integer.toString(num));
		pp.setValue(Integer.toString(num));
		CommandNode pc = (CommandNode)TreeUtil.findChild(graphNode, 
					"getoldframe");
		pc.start();
		frameNum.setText(Integer.toString(num));
		frameNumber = num;
	}
}
