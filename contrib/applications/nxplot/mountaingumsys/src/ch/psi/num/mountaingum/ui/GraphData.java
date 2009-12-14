/**
 * This class manages the data associated with a graphical component: i.e,
 * axis data or the data to be plotted. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 * 
 * Copying arrays all the time introduced reference leaks. Thus this 
 * was rewritten to maintain a local copy of the node data for the 
 * graphics and copy data from the node on each request. A local 
 * copy of the data is required as the plot logic may require to 
 * modify data. And if you do not allocate memory, it cannot 
 * leak!
 * 
 * Mark Koennecke, June 2008
 */
package ch.psi.num.mountaingum.ui;


import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class GraphData implements UpdateListener {
	protected Graph graph;
	protected String name;
	protected ParameterNode node;
	protected Object style;
	protected double data[];
	
	public GraphData(String name, ParameterNode p){
		this.name = name;
		style = null;
		node = p;
		graph = null;
		node.addUpdateListener(this);
		data = null;
	}

	public void nodeStatusChanged(TreeNode node) {
		// nothing to do here
	}

	public void nodeUpdated(TreeNode n) {
		if(graph != null){
			SysRegistry.getLogger().debug("GraphData received new data");
			//System.out.println("GraphData received new data");
			graph.changeData(this);
		}
	}
	public double[] getData(){
		if(data == null || node.getValue().getLength() != data.length){
			data = new double[node.getValue().getLength()];
		}
		TreeUtil.copyNodeValueToDouble(node, data);
		return data;
	}
	public double getDataValue(int idx){
		if(data == null){
			getData();
		}
		return data[idx];
	}
	/**
	 * when plotting logarithmically, values below or equal to zero
	 * fuck up. This fixes this. 
	 * @return A spick and spank data set ready for log plotting.
	 */
	public double[] getDataLogWashed(){
		getData();
		for(int i = 0; i < data.length; i++){
			if(data[i] <= .0){
				data[i] = .1;
			}
		}
		return data;
	}
	public Graph getGraph() {
		return graph;
	}

	public DataRange getDataRange(){
		DataRange dr = new DataRange();
		double d[] = getData();
		if(d != null){
			dr.calculate(d);
		}
		d = null;
		return dr;
	}
	public int getDataLength(){
		return node.getValue().getLength();
	}
	public void setGraph(Graph graph) {
		this.graph = graph;
	}

	public Object getStyle() {
		return style;
	}

	public void setStyle(Object style) {
		this.style = style;
		if(graph != null){
			graph.makePlot();
		}
	}

	public String getName() {
		return name;
	}
	
	public String getAxisName(){
		ParameterNode pn = (ParameterNode)TreeUtil.findChild(node,"name");
		if(pn != null){
			return pn.getValue().toString();
		} else {
			return name;
		}
	}

	public int findIndex(double val){
		int res = 0;
		if(data == null){
			return 0;
		}
		while(data[res] < val  && res < data.length -1){
			res++;
		}
		return res;
	}
	public ParameterNode getNode() {
		return node;
	}
	public String getTitle(){
		ParameterNode  pn = (ParameterNode)TreeUtil.findChild(node.getParent(), "title");
		if(pn != null){
			return pn.getValue().toString();
		} else {
			return node.getParent().getName();
		}
	}
	public double getValue(int pos[]){
		if(data == null){
			getData();
		}
		return data[pos[0]];
	}
	public void finalize() throws Throwable {
		data = null;
		graph = null;
		node = null;
		//System.out.println("Killing GraphData.." );
		super.finalize();
	}
}
