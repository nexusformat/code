/**
 * This is a derived class of GraphAxis which is able to 
 * represent the axis both as Q (the normal) and D. This is 
 * a special for POLDI
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2008
 */
package ch.psi.num.mountaingum.poldi;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.ui.GraphAxis;

public class PoldiAxis extends GraphAxis {
	final static int QMODE = 0;	
	final static int DMODE = 1;
	
	private int axisMode;
	private double convertedData[];
	
	public PoldiAxis(String name, ParameterNode p) {
		super(name, p);
		convertedData = null;
	}
	public void setAxisMode(int axisMode) {
		if(axisMode == QMODE ||  axisMode == DMODE){
			this.axisMode = axisMode;
		} else {
			SysRegistry.getLogger().error("Invalid axisMode in PoldiAxis: " 
					+ axisMode);
		}
	}
	public double[] getData() {
		super.getData();
		if(axisMode == DMODE){
			convertToD();
			return convertedData;
		}
		return data;
	}
	private void convertToD() {
		int i;
		
		if(convertedData == null || convertedData.length != data.length){
			convertedData = null;
			convertedData = new double[data.length];
		}
		double divisor = 2. * Math.PI;
		for(i = 0; i < data.length; i++){
			if(data[i] > .0001){
				convertedData[i] = divisor/data[i];
			} else {
				convertedData[i] = .0;
			}
		}
	}
	public String getAxisName() {
		if(axisMode == QMODE){
			return "d";
		} else {
			return "q";
		}
	}
}
