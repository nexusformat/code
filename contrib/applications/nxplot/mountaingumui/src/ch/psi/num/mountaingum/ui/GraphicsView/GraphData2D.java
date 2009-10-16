/**
 * The SGT grid plot for 2D data has some pecularities. The most notable of which 
 * is that the x and y are swapped in comparison with the data I get from
 * SICS. This class takes care of this and is also responsible for finding
 * the value of individual points.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, June 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.util.Array2DUtil;

public class GraphData2D extends GraphData {
	private Array2DUtil ute;
	private double tmp[];
	
	public GraphData2D(String name, ParameterNode p) {
		super(name, p);
		ute = new Array2DUtil();
		tmp = null;
	}

	public double[] getData() {
		int xdim, ydim;
		super.getData();
		if(tmp == null || tmp.length != data.length){
			tmp = null;
			tmp = new double[data.length];
		}
		xdim = graph.getXAxis().getDataLength();
		ydim = graph.getYAxis().getDataLength();
		try{
			ute.rotate90(data, tmp, xdim, ydim);
		}catch(Exception eva){}
		System.arraycopy(tmp, 0, data, 0, tmp.length);
		return data;
	}
	
	public double getValue(int pos[]){
		int xdim = graph.getXAxis().getDataLength();
		int address = pos[0]*xdim + pos[1];
		if(data != null && address < data.length){
			return data[address];
		} else {
			return 0;
		}
	}
	public double[] getOriData(){
		return super.getData();
	}
}
