/**
 * This is a NeXis file mapper for rita. Mainly it tries to 
 * create the graphics for the n windows at RITA
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.tas;

import java.util.Enumeration;
import java.util.HashSet;
import java.util.Hashtable;

import org.nexusformat.NexusException;

import ch.psi.num.mountaingum.nexus.loader.DefaultNexusMapper;
import ch.psi.num.mountaingum.nexus.loader.FlatNexusFile;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class RitaNexusMapper extends DefaultNexusMapper {
	private double axisdata[];
	private String axisname;
	private HashSet killList;
	
	public RitaNexusMapper(){
		killList = new HashSet();
		killList.add("/entry1/RITA-2/analyzer/blade_rotation_angles");
		killList.add("/entry1/RITA-2/analyzer/polar_angle");
		killList.add("/entry1/RITA-2/analyzer/energy");
		killList.add("/entry1/RITA-2/analyzer/blade_angles");
		killList.add("/entry1/RITA-2/detectorwindows/polar_angle");
		killList.add("/entry1/sample/Qm");
		axisdata = null;
		axisname = null;
	}
	
	public boolean isApplicable(FlatNexusFile nf) {
		try{
			nf.openpath("/entry1/RITA-2");
			return true;
		}catch(NexusException ne){}
		return false;
	}

	public void map(TreeNode root, FlatNexusFile nf, String nxclass,
			String nxpath) {

		if(nxpath.indexOf("/entry1/data") >= 0){
			addPath(nxpath,nf);
			return;
		}
		if(killList.contains(nxpath)){
			addPath(nxpath,nf);
			return;
		}
		
		if(nxpath.equals("/entry1/RITA-2/detectorwindows/counts")){
			locateAxisData(nf);
			if(axisdata != null){
				buildWindows(root, nf,nxpath);
			}
			addPath(nxpath,nf);
			return;
		}
		
		if(nxpath.equals("/entry1/RITA-2/detector/counts")){
			makeGraphNode(root,nf,nxpath);
			TreeNode com = TreeUtil.searchNode(root,"/graphics/detector/getoldframe");
			/**
			 * arrange the default axis settings to become useful
			 */
			com = TreeUtil.searchNode(root, "graphics/detector/axis0");
			com.setProperty("type", "hugo");
			com = TreeUtil.searchNode(root, "graphics/detector/axis1");
			com.setProperty("dim", "0");
			com = TreeUtil.searchNode(root, "graphics/detector/axis2");
			com.setProperty("dim", "1");
			
			addPath(nxpath,nf);
			return;
		}
		
		super.map(root, nf, nxclass, nxpath);
	}

	protected void buildWindows(TreeNode root, FlatNexusFile nf, String nxpath) {
		int allcounts[][], counts[], dim[], info[], i, j;
		counts = new int[axisdata.length];
		try{
			nf.openpath(nxpath);
			dim = new int[2];
			info = new int[2];
			nf.getinfo(dim,info);
			allcounts = new int[dim[0]][dim[1]];
			nf.getdata(allcounts);
			for(i = 0; i < 9; i++){
				for(j = 0; j < dim[0]; j++){
					counts[j] = allcounts[j][i];
				}
				makeWindowNode(root, i, counts);
			}
		}catch(NexusException ne){
			SysRegistry.getLogger().error("Failed to build windows for RITA");
		}
	}

	protected  void makeWindowNode(TreeNode root, int i, int[] counts) {
		InternalParameter tmp;
		int helpdim[];
		DoubleValue dv;
		IntValue iv;
		
		TreeNode graphics = locateGraphics(root);
		String name = "w" + Integer.toString(i+1);
		GraphicsNode w = new GraphicsNode(graphics,name);
		graphics.insertNode(TreeNode.APPEND, w);
		w.setProperty("type","graphdata");
	
		helpdim = new int[1];
		tmp = new InternalParameter(w,"rank");
		w.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(1);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(w,"dim");
		w.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(counts.length);
		tmp.updateValue(iv);

		tmp = new InternalParameter(w,axisname);
		w.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = axisdata.length;
		dv = new DoubleValue(1,helpdim);
		dv.setData(axisdata);
		tmp.updateValue(dv);
		tmp.setProperty("type", "axis");
		tmp.setProperty("dim", "0");
		
		tmp = new InternalParameter(w,"counts");
		w.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = counts.length;
		iv = new IntValue(1,helpdim);
		iv.setData(counts);
		tmp.updateValue(iv);
		tmp.setProperty("type", "data");
	}

	protected  void locateAxisData(FlatNexusFile nf) {
		String name, type, axis;
		int i, dim[], info[];
		float fdata[];
		
		try{
			nf.openpath("/entry1/data");
			Hashtable ht = nf.groupdir();
			Enumeration e = ht.keys();
			while(e.hasMoreElements()){
				name = (String)e.nextElement();
				type = (String)ht.get(name);
				if(type.equals("SDS")){
					nf.opendata(name);
					axis = getAttr(nf,"axis");
					if(axis != null && axis.equals("1")){
						axisname = name;
						fdata = nf.getfloatdata();
						dim = new int[3];
						info = new int[2];
						nf.getinfo(dim, info);
						/**
						 * sometimes the axis data is 2D as well, then we have to get the fifth 
						 * as scan axis. 
						 */
						if(info[0] == 2){
							float tmpdata[][] = new float[dim[0]][dim[1]];
							nf.getdata(tmpdata);
							fdata = null;
							fdata = new float[dim[0]];
							for(i = 0; i < dim[0]; i++){
								fdata[i] = tmpdata[i][4];
							}
							tmpdata = null;
							
						}
						axisdata = new double[fdata.length];
						for(i = 0; i < fdata.length; i++){
							axisdata[i] = (double)fdata[i];
						}
						nf.closedata();
						return;
					}
					nf.closedata();
				}
			}
		}catch(NexusException ne){
			SysRegistry.getLogger().error("ERROR: failed to process /entry1/data in RITA file");
		}
	} 

	public void reset() {
		axisdata = null;
		axisname = null;
		super.reset();
	}
	
	
}
