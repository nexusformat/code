/**
 * This class is responsible for loading a SINQ ASCII file into a
 * GTSE NeXus viewer tree for viewing.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.nexus.loader.sinqascii;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Enumeration;
import java.util.HashSet;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;

public class ASCIILoader {
	private HashSet exclude;

	public ASCIILoader(){
		exclude = new HashSet();
		exclude.add("ScanVariables");
	}
	public TreeNode loadASCIIFile(String filename){
		TreeNode root = null, par;
		GraphicsNode graph;
		InternalParameter tmp;
		ASCIIReader ar;
		int i;
		String parname, parvalue;
		
		try{
			File f = new File(filename);
			FileInputStream fin = new FileInputStream(f);
			ar = new ASCIIReader(fin);
			ar.read();
			fin.close();
		}catch(IOException eva){
			SysRegistry.getLogger().error(eva.getMessage());
			return null;
		}
		
		root = new TreeNode(null,"");
		par = new TreeNode(root,"parameter");
		root.insertNode(TreeNode.APPEND, par);
		graph = new GraphicsNode(root,"scan");
		root.insertNode(TreeNode.APPEND, graph);

		/**
		 * exclude arrays of scan vars
		 */
		String scanvars = (String)ar.getValue("names");
		String scanp[] = scanvars.split(",");
		for(i = 0; i < scanp.length; i++){
			exclude.add(scanp[i]);
		}
		
		
		/**
		 * add parameters
		 */
		Enumeration e = ar.getKeys();
		while(e.hasMoreElements()){
			parname = (String)e.nextElement();
			if(! exclude.contains(parname)){
				parvalue = (String)ar.getValue(parname);
				tmp = new InternalParameter(par,parname);
				par.insertNode(TreeNode.APPEND, tmp);
				tmp.setValue(parvalue);
			}
		}
		
		makeGraph(graph, ar);
		root.setProperty("filename", filename);
		
		return root;
	}
	private void makeGraph(GraphicsNode graph, ASCIIReader ar) {
		int helpdim[], i, j;
		InternalParameter tmp;
		IntValue iv;
		DoubleValue dv;
		float counts[], par[];
		
		graph.setProperty("type", "graphset");
		counts = (float[])ar.getValue("Counts");
		
		helpdim = new int[1];
		tmp = new InternalParameter(graph,"rank");
		graph.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(1);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(graph,"dim");
		graph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(counts.length);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(graph,"counts");
		tmp.setProperty("type", "data");
		graph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = counts.length;
		iv = new IntValue(1,helpdim);
		for(i = 0; i < counts.length; i++){
			iv.append(counts[i]);
		}
		tmp.updateValue(iv);

		String scanvars = (String)ar.getValue("ScanVariables");
		String scanp[] = scanvars.split(",");
		for(i = 0; i < scanp.length; i++){
			par = (float[])ar.getValue(scanp[i].trim());
			if(par != null && par.length > 1 && Math.abs(par[1] - par[0]) > 0.0001) {
				tmp = new InternalParameter(graph,scanp[i]);
				graph.insertNode(TreeNode.APPEND, tmp);
				helpdim[0] = par.length;
				dv = new DoubleValue(1,helpdim);
				for(j = 0; j < par.length; j++){
					dv.append(par[j]);
				}
				tmp.updateValue(dv);
				tmp.setProperty("type", "axis");
				tmp.setProperty("dim", "0");
				return;
			}
		}
	}
}
