/**
 * This is a derived class of GraphAxis which is able to 
 * represent the axis both as time-of-flight and as 
 * energy
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2008
 */
package ch.psi.num.mountaingum.focus;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.GraphAxis;

public class FocusAxis extends GraphAxis {
	private ParameterNode lambdaNode, elasticNode;
	private double sample2Detector, totalPath;
	
	private int axisMode;
	public FocusAxis(String name, ParameterNode p) {
		super(name, p);
		lambdaNode = null;
		elasticNode = null;
	}
	public void setAxisMode(int axisMode) {
		if(axisMode == FocusViewer.TOF ||  axisMode == FocusViewer.ENERGY){
			this.axisMode = axisMode;
		} else {
			SysRegistry.getLogger().error("Invalid axisMode in FocusAxis: " 
					+ axisMode);
		}
	}
	public double[] getData() {
		super.getData();
		if(axisMode == FocusViewer.ENERGY){
			convertToEnergy();
		}
		return data;
	}
	private void convertToEnergy() {
		double lambda, elasticPos, ee1, div2, divisor;
		double theopos;
		int i;
		
		if(lambdaNode == null){
			initNodes();
		}
		
		lambda = TreeUtil.getNodeValue(lambdaNode, 0);
		if(lambda < 1.){
			SysRegistry.getLogger().error("Lambda below 1. in FocusAxis");
			return;
		}
		if(elasticNode != null){
			elasticPos = TreeUtil.getNodeValue(elasticNode, 0);
		} else {
			elasticPos = .0;
		}
		theopos = lambda * totalPath * 252.78;
		if(Math.abs(theopos - elasticPos) > 100){
			SysRegistry.getLogger().error("Dubious elasticPos " + 
					elasticPos + " coerced to " + theopos);
			elasticPos = theopos;
		}
		ee1 = (double)81.799/(lambda*lambda);
		div2 = (double)252.78 * sample2Detector;
		for(i = 0; i < data.length; i++){
			divisor = ((data[i] - elasticPos)/div2) + lambda;
			data[i] = ee1 - ((double)81.799/(divisor*divisor));
		}
	}
	private void initNodes() {
		lambdaNode = (ParameterNode)TreeUtil.nodeForPath("/instrument/monochromator/tofwavelength");
		if(lambdaNode == null){
			lambdaNode = (ParameterNode)TreeUtil.searchNode(TreeUtil.findRoot(node), "/entry1/FOCUS/monochromator/lambda");
		}
		elasticNode = (ParameterNode)TreeUtil.nodeForPath("/instrument/monochromator/tofelastic");
		sample2Detector = 2.5;
		totalPath = 3.0;
	}
	public String getAxisName() {
		if(axisMode == FocusViewer.ENERGY){
			return "energy-transfer";
		}
		return super.getAxisName();
	}
}
