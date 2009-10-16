/**
 * This is a NeXus file mapper for AMOR. It basically calculates the 
 * various projections from the 3D TOF data set.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.amor;

import org.nexusformat.NexusException;

import ch.psi.num.mountaingum.nexus.loader.DefaultNexusMapper;
import ch.psi.num.mountaingum.nexus.loader.FlatNexusFile;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

import shared.array.IntegerArray;
import shared.array.RealArray;

public class AmorNexusMapper extends DefaultNexusMapper {


	public AmorNexusMapper() {
		super();
	}

	public boolean isApplicable(FlatNexusFile nf) {
		try{
			nf.openpath("/entry1/AMOR");
			return true;
		}catch(NexusException ne){}
		return false;
	}

	public void map(TreeNode root, FlatNexusFile nf, String nxclass,
			String nxpath) {
		if(nxpath.equals("/entry1/AMOR/area_detector/data")){
			makeAmorPlots(root, nf, nxpath);
			addPath(nxpath,nf);
			return;
		}
		if(nxpath.equals("/entry1/AMOR/single_detector_1/data")){
			makeGraphNode(root,nf,nxpath);
			fixAxis(root,"graphics/single_detector_1");
			addPath(nxpath,nf);
			return;
		}
		if(nxpath.equals("/entry1/AMOR/single_detector_2/data")){
			makeGraphNode(root,nf,nxpath);
			fixAxis(root,"graphics/single_detector_2");
			addPath(nxpath,nf);
			return;
		}
		super.map(root, nf, nxclass, nxpath);
	}

	private void fixAxis(TreeNode root, String path) {
		TreeNode graph = TreeUtil.searchNode(root, path);
		TreeNode ax = TreeUtil.findChild(graph, "axis0");
		graph.deleteChild(ax);
		ax = TreeUtil.findChild(graph, "time_binning");
		ax.setProperty("dim", "0");
	}

	private void makeAmorPlots(TreeNode root, FlatNexusFile nf, String nxpath) {
		int info[], dim[], data[];

		info = new int[2];
		dim = new int[3];
		try{
			nf.getinfo(dim,info);
			data = nf.getintdata();
			IntegerArray rawdata = new IntegerArray(data,dim[0],dim[1],dim[2]);
			RealArray rawreal = rawdata.tor();
			rawdata = null;
			
			makeDetImage(rawreal, root);
			
			makeYTOF(rawreal, root, nf);
			
		}catch(NexusException naomi){
			SysRegistry.getLogger().error(naomi.getMessage());
		}
	}

	private void printDim(int dim[]){
		int i;
		for(i = 0; i < dim.length; i++){
			System.out.print(dim[i]);
			System.out.print(' ');
		}
		System.out.print('\n');
	}
	private void makeYTOF(RealArray rawreal, TreeNode root, FlatNexusFile nf) {
		RealArray ra;
		int helpdim[], i;
		InternalParameter tmp;
		IntValue iv;
		
		ra = rawreal.rSum(0);
		printDim(ra.dimensions());
		ra = ra.rSum(1);
		printDim(ra.dimensions());

		TreeNode graphics = locateGraphics(root);
		GraphicsNode imGraph = new GraphicsNode(graphics,"YTOF");
		graphics.insertNode(TreeNode.APPEND, imGraph);
		imGraph.setProperty("type", "graphdata");

		helpdim = new int[1];
		tmp = new InternalParameter(imGraph,"rank");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(1);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(imGraph,"dim");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(ra.size(2));
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(imGraph,"counts");
		tmp.setProperty("type", "data");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = ra.size(0);
		iv = new IntValue(1,helpdim);
		double d[] = ra.values();
		for(i = 0; i < d.length; i++){
			iv.append(d[i]);
		}
		tmp.updateValue(iv);
		
		tmp = (InternalParameter)TreeUtil.searchNode(root,"/graphics/single_detector_1/time_binning");
		InternalParameter tof = new InternalParameter(imGraph,tmp.getName());
		imGraph.insertNode(TreeNode.APPEND, tof);
		tof.updateValue(tmp.getValue());
		tmp.copyProperties(tof);
		tof.setProperty("dim", "0");
		
	}

	private void makeDetImage(RealArray rawreal, TreeNode root) {
		int helpdim[], i;
		InternalParameter tmp;
		IntValue iv;
		
		RealArray im = rawreal.rSum(2);
		im = im.reshape(im.size(1), im.size(0), im.size(2));
		
		TreeNode graphics = locateGraphics(root);
		GraphicsNode imGraph = new GraphicsNode(graphics,"image");
		graphics.insertNode(TreeNode.APPEND, imGraph);
		imGraph.setProperty("type", "graphdata");

		helpdim = new int[1];
		tmp = new InternalParameter(imGraph,"rank");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(2);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(imGraph,"dim");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = 2;
		iv = new IntValue(1,helpdim);
		iv.append(im.size(0));
		iv.append(im.size(1));
		tmp.updateValue(iv);

		addDefaultAxis(imGraph,0,im.size(0));
		addDefaultAxis(imGraph,1,im.size(1));
		
		tmp = new InternalParameter(imGraph,"counts");
		tmp.setProperty("type", "data");
		imGraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = im.size(0) * im.size(1);
		iv = new IntValue(1,helpdim);
		double d[] = im.values();
		for(i = 0; i < d.length; i++){
			iv.append(d[i]);
		}
		tmp.updateValue(iv);
		
	}

}
