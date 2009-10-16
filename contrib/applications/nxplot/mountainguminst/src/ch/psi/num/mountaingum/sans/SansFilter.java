/**
 * This class filters the SANS data received from SICS 
 * so that everything is oriented nicely. 
 * 
 *  Mark Koennecke, May 2008
 */
package ch.psi.num.mountaingum.sans;

import ch.psi.num.mountaingum.tree.DataFilter;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.util.Array2DUtil;
import ch.psi.num.mountaingum.util.ArrayTools;

public class SansFilter implements DataFilter {
	private Array2DUtil ute;
	
	public SansFilter(){
		ute = new Array2DUtil();
	}
	
	public void process(TreeNode n) throws Exception {
		ParameterNode p = (ParameterNode)n;
		IntValue dv = (IntValue)p.getValue();
		int d[] = dv.getData();
		//ArrayTools.printStats(d);
		int res[] = new int[d.length];
		ute.rotate90(d,res,128,128);
		d = null;
		dv.setData(res);
	}
}
