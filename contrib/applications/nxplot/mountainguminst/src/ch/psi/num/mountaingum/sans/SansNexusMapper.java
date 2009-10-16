/**
 * This is the NeXus file mapper for SANS and SANSLI. It is here to intercept 
 * and specially treat certain nodes in the SANS hierarchy.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.sans;

import java.util.HashSet;

import org.nexusformat.NexusException;

import ch.psi.num.mountaingum.nexus.loader.DefaultNexusMapper;
import ch.psi.num.mountaingum.nexus.loader.FlatNexusFile;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class SansNexusMapper extends DefaultNexusMapper {
	private boolean sans2;
	private HashSet sansOmit;
	
	public SansNexusMapper(){
		sansOmit = new HashSet();
		sansOmit.add("/entry1/SANS-II/detector/counts_anode");
		sansOmit.add("/entry1/SANS-II/detector/counts_back");
		sansOmit.add("/entry1/SANS-II/detector/counts_front");
		sansOmit.add("/entry1/SANS/detector/counts");
	}
	public boolean isApplicable(FlatNexusFile nf) {
		try{
			nf.openpath("/entry1/SANS");
			sans2 = false;
			return true;
		}catch(NexusException ne){}
		
		try{
			nf.openpath("/entry1/SANS-II");
			sans2 = true;
			return true;
		}catch(NexusException ne){}
		
		return false;
	}

	public void map(TreeNode root, FlatNexusFile nf, String nxclass,
			String nxpath) {
		
		/**
		 * This means I wish to suppress the additional 512x512 raw images 
		 * from the view. Only confuses loosers. 
		 */
		if(sansOmit.contains(nxpath)){
			addPath(nxpath,nf);
			return;
		}		
		
		if(nxpath.equals("/entry1/SANS-II/detector/counts")){
			fixSANS2(root, nf, nxpath);
			makeRadialGraph(root);
			return;
		}
		
		if(!sans2 && nxpath.equals("/entry1/data1/counts")){
			makeGraphNode(root,nf,nxpath);
			makeRadialGraph(root);
			addPath(nxpath,nf);
			return;
		}
		if(nxpath.equals("/entry1/SANS-II/detector/z_position")){
			makeParameterNode(root,nf,nxpath);
			TreeNode n = TreeUtil.searchNode(root, nxpath);
			n.setProperty("multiply", "100");
			return;
		}
		super.map(root, nf, nxclass, nxpath);
	}
	/**
	 * Make the TreeNode for the radial_average 
	 * @param root The root node
	 * @param int the instrument name
	 */
	private void makeRadialGraph(TreeNode root) {
		InternalParameter work;
		IntValue iv;
		DoubleValue dv;
		
		TreeNode graphics = TreeUtil.findChild(root, "graphics");
		if(graphics == null){
			System.out.println("Programming error in SansNexusMapper, This needs to be called after generating the main graph node");
			return;
		}

		TreeNode radial = new TreeNode(graphics,"radial_average");
		radial.setProperty("type", "graphdata");
		radial.setProperty("viewer", "mountaingumui.SansRadialViewer");
		graphics.insertNode(TreeNode.APPEND, radial);
		
		int helpdim[] = new int[1];
		work = new InternalParameter(radial,"rank");
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.setFromString("1");
		work.updateValue(iv);
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"dim");
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.setFromString("256");
		work.updateValue(iv);
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"2ddata");
		if(sans2){
			work.setValue("/graphics/detector/counts");
		} else {
			work.setValue("/graphics/data1/counts");
		}
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"2ddim");
		helpdim[0] = 2;
		iv = new IntValue(1,helpdim);
		iv.setFromString("128 128");
		work.updateValue(iv);
		radial.insertNode(TreeNode.APPEND, work);

		work = new InternalParameter(radial,"2dpix");
		helpdim[0] = 2;
		dv = new DoubleValue(1,helpdim);
		if(sans2){
			dv.setFromString("4.34 4.34");
		} else {
			dv.setFromString("7.5 7.5");
		}
		work.updateValue(dv);
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"lambdapath");
		if(sans2){
			work.setValue("/entry1/SANS-II/Dornier-VS/lambda");
		} else {
			work.setValue("/entry1/SANS/Dornier-VS/lambda");
		}
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"distancepath");
		if(sans2){
			work.setValue("/entry1/SANS-II/detector/z_position");
		} else {
			work.setValue("/entry1/SANS/detector/x_position");
		}
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"counts");
		helpdim[0] = 256;
		dv = new DoubleValue(1,helpdim);
		work.updateValue(dv);
		work.setProperty("type", "data");
		radial.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(radial,"q");
		helpdim[0] = 256;
		dv = new DoubleValue(1,helpdim);
		work.updateValue(dv);
		work.setProperty("type", "axis");
		work.setProperty("dim", "0");
		radial.insertNode(TreeNode.APPEND, work);

	}
	/**
	 * This fixes the x and y axis for SANS2. By accident it picks those up which are vaild 
	 * for the 512x512 raw data arrays.
	 * @param root 
	 * @param nf The NeXusFile
	 * @param nxpath The path
	 */
	private void fixSANS2(TreeNode root, FlatNexusFile nf, String nxpath) {
		InternalParameter axis;
		
		TreeNode mygraph = makeGraphNode(root, nf,nxpath);
		addPath(nxpath,nf);
		
		int helpdim[] = new int[1];
		DoubleValue iv = new DoubleValue(1,helpdim);
		for(int i = 0; i < 128; i++){
			iv.append((double)i);
		}
		axis = (InternalParameter)TreeUtil.findChild(mygraph, "detector_x");
		if(axis != null){
			axis.updateValue(iv);
		}
		axis = (InternalParameter)TreeUtil.findChild(mygraph, "detector_y");
		if(axis != null){
			axis.updateValue(iv);
		}
	}
}
