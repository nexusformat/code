/**
 * This is a special  NeXus mapper for the FOCUS instrument
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.focus;

import org.nexusformat.NexusException;

import ch.psi.num.mountaingum.nexus.loader.DefaultNexusMapper;
import ch.psi.num.mountaingum.nexus.loader.FlatNexusFile;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.util.Array2DUtil;

public class FocusNexusMapper extends DefaultNexusMapper {

	public boolean isApplicable(FlatNexusFile nf) {
		try{
			nf.openpath("/entry1/FOCUS");
			return  true;
		}catch(NexusException ne){}
		return false;
	}

	public void map(TreeNode root, FlatNexusFile nf, String nxclass,
			String nxpath) {

		if(alreadySeen(nf,nxpath)){
			return;
		}
		
		if(nxpath.equals("/entry1/FOCUS/tof_monitor")){
			return;
		}
		
		if(nxpath.indexOf("summed") > 0){
			TreeNode n = makeGraphNode(root,nf,nxpath);
			addPath(nxpath,nf);
			String name = n.getName();
			int idx = name.indexOf('_');
			if(idx > 0){
				name = name.substring(0,idx);
			}
			n.setProperty("name", "sum" + name);
			return;
		}
		
		if(nxpath.indexOf("counts") > 0){
			TreeNode n = makeGraphNode(root,nf,nxpath);
			addPath(nxpath,nf);
			n.setProperty("viewer", "mountaingumui.FocusViewer");
//			swapData((InternalParameter)TreeUtil.findChild(n, "counts"), nf, nxpath);
			TreeNode ax = TreeUtil.findChild(n, "theta");
			ax.setProperty("dim", "1");
			ax = TreeUtil.findChild(n, "time_binning");
			ax.setProperty("dim", "0");
			return;
		}
		
		super.map(root, nf, nxclass, nxpath);
	}

	/**
	 * This was wrong in the end. But I leave it in for purposes of documentation. 
	 * @param cts
	 * @param nf
	 * @param nxpath
	 */
	private void swapData(InternalParameter cts, FlatNexusFile nf, String nxpath) {
		IntValue iv = (IntValue)cts.getValue();
		int srcdata[] = iv.getData();
		int targetdata[] = new int[srcdata.length];
		int dim[] = new int[2];
		int info[] = new int[2];
		try{
			nf.openpath(nxpath);
			nf.getinfo(dim, info);
			Array2DUtil au = new Array2DUtil();
			au.rotate90(srcdata, targetdata, dim[1], dim[0]);
			iv.setData(targetdata);
		}catch(Exception e){}
	}

}
