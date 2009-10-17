/**
 * This is the default implementation for a NeXus file 
 * mapper. It will copy all the normal data elements 
 * into something looking like the file hierarchy. 
 * Larger SDS's get a special treatment: the default 
 * mapper tries to map them into the graphics hierarchy.
 * In order to avoid duplication, the default mapper keeps 
 * track of what it has mapped already and ignores items 
 * which have already been mapped. Or which are linked to 
 * something already mapped. 
 * 
 * Other mappers will probably override this mapper to treat 
 * some special paths differently. 
 * 
 *   copyright: GPL
 *   
 *   Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import java.util.Enumeration;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;

import org.nexusformat.AttributeEntry;
import org.nexusformat.NeXusFileInterface;
import org.nexusformat.NexusException;
import org.nexusformat.NexusFile;

import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.LongValue;
import ch.psi.num.mountaingum.tree.NodeValue;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class DefaultNexusMapper implements NeXusMapper {
	protected HashSet<String> mapset;
	
	public DefaultNexusMapper(){
		mapset = new HashSet<String>();
	}
	/**
	 * Well, the default mapper applies to any file....
	 */
	public boolean isApplicable(FlatNexusFile nf) {
		return true;
	}

	/**
	 * the main mapping function, entry to all the work
	 */
	public void map(TreeNode root, FlatNexusFile nf, String nxclass, String nxpath) {
		
		if(alreadySeen(nf,nxpath)){
			return;
		}

		if(nxclass.equalsIgnoreCase("SDS")){
			makeParameterNode(root, nf, nxpath);
		} else {
			makeGroupNode(root, nf, nxpath);
		}

		addPath(nxpath,nf);
	}

	protected void addPath(String nxpath, FlatNexusFile nf) {
		mapset.add(nxpath);
		try{
			nf.openpath(nxpath);
			/**
			 * This section is a workaround for a problem
			 * with attribute reading through the Java-API.
			 * This can be removed once  nxinitattrdir() has 
			 * been included and is used by nf.attrdir()
			 */
			String pathel[] = nxpath.substring(1).split("/");
			String name = pathel[pathel.length -1];
			nf.closedata();
			nf.opendata(name);
			String link = getAttr(nf,"target");
			if(link != null){
				mapset.add(link);
			}
		}catch(NexusException ne){}
	}
	/*
	 * This creates all the group hierarchy components till nxpath. 
	 */
	protected  void makeGroupNode(TreeNode root, FlatNexusFile nf,
			String nxpath) {
		String pathel[] = nxpath.substring(1).split("/");
		TreeNode current;
		
		TreeNode parent = root;
		for(int i = 0; i < pathel.length; i++){
			current = TreeUtil.findChild(parent, pathel[i]);
			if(current == null){
				current = new TreeNode(parent, pathel[i]);
				parent.insertNode(TreeNode.APPEND, current);
			}
			parent = current;
		}
	}
	
	/**
	 * make any parameter node
	 * @param root
	 * @param nf
	 * @param nxpath
	 */
	protected  void makeParameterNode(TreeNode root, FlatNexusFile nf,
			String nxpath) {
		int dim[], info[], totalLength = 1;;
		
		
		dim = new int[32];
		info = new int[2];
		try{
			nf.getinfo(dim, info);
			for(int i = 0; i < info[0]; i++){
				totalLength *= dim[i];
			}
			
			/**
			 * This section is a workaround for a problem
			 * with attribute reading through the Java-API.
			 * This can be removed once  nxinitattrdir() has 
			 * been included and is used by nf.attrdir()
			 */
			String pathel[] = nxpath.substring(1).split("/");
			String name = pathel[pathel.length -1];
			nf.closedata();
			nf.opendata(name);

			Hashtable attr = nf.attrdir();
			if(attr.get("axis") != null){
				// ignore: will be picked up when building graph data
				return;
			} else if(attr.get("signal") != null || totalLength > 100){
				System.out.println("Making graphnode for " + nxpath);
				makeGraphNode(root, nf, nxpath);
				return;
			} else {
				makeSimpleParNode(root,nf,nxpath,dim,info);
			}
		}catch(NexusException ne){
			
		}
	}
	/**
	 * make a simple parameter node
	 * @param root
	 * @param nf
	 * @param nxpath
	 * @param dim
	 * @param info
	 */
	protected  TreeNode makeSimpleParNode(TreeNode root, FlatNexusFile nf,
			String nxpath, int[] dim, int[] info) {
		String name, pathel[];
		StringBuffer pathToGroup;
		TreeNode group;
		NexusParameter par;
		
		/*
		 * split off the name and ensure that the desired group exists.
		 */
		pathel = nxpath.substring(1).split("/");
		name = pathel[pathel.length-1];
		pathToGroup = new StringBuffer();
		for(int i = 0; i < pathel.length-1; i++){
			pathToGroup.append('/');
			pathToGroup.append(pathel[i]);
		}
		String groupath = pathToGroup.toString();
		makeGroupNode(root,nf,groupath);
		group = TreeUtil.searchNode(root, groupath.substring(1));
		
		par = new NexusParameter(group,name);
		group.insertNode(TreeNode.APPEND, par);
		
		NodeValue v = makeValue(nf,dim, info);
		par.updateValue(v);
		par.setProperty("nxpath", nxpath);
		return par;
	}
	/**
	 * convert the current SDS to a GTSE NodeValue
	 * @param nf
	 * @param dim
	 * @param info
	 * @return
	 */
	private NodeValue makeValue(FlatNexusFile nf, int[] dim, int[] info) {
		NodeValue v;
		IntValue iv;
		DoubleValue dv;
		LongValue lv;
		int i;
		
		switch(info[1]){
		case NexusFile.NX_CHAR:
		case NexusFile.NX_INT8:
		case NexusFile.NX_UINT8:
			v = new NodeValue();
			v.setFromString(nf.gettextdata());
			return v;
		case NexusFile.NX_INT16:
		case NexusFile.NX_UINT16:
			short sdata[] = nf.getshortdata();
			iv = new IntValue(info[0],dim);
			for(i = 0; i < sdata.length; i++){
				iv.append(sdata[i]);
			}
			return iv;
		case NexusFile.NX_INT32:
		case NexusFile.NX_UINT32:
			int idata[] = nf.getintdata();
			iv = new IntValue(info[0],dim);
			iv.setData(idata);
			return iv;
		case NexusFile.NX_INT64:
		case NexusFile.NX_UINT64:
			long ldata[] = nf.getlongdata();
			lv = new LongValue(info[0],dim);
			lv.setData(ldata);
			return lv;
		case NexusFile.NX_FLOAT32:
			float fdata[] = nf.getfloatdata();
			dv = new DoubleValue(info[0],dim);
			for(i = 0; i < fdata.length; i++){
				dv.append((double)fdata[i]);
			}
			return dv;
		case NexusFile.NX_FLOAT64:
			double ddata[] = nf.getdoubledata();
			dv = new DoubleValue(info[0],dim);
			dv.setData(ddata);
			return dv;
		}
		return null;
	}
	/**
	 * make a graphics node 
	 * @param root
	 * @param nf
	 * @param nxpath
	 * @return
	 */
	protected  TreeNode makeGraphNode(TreeNode root, FlatNexusFile nf,
			String nxpath) {
		TreeNode graphics, mygraph = null;
		InternalParameter tmp;
		int dim[], info[], helpdim[], i;
		IntValue iv;
		
		// ensure graphics node
		graphics = locateGraphics(root);
		
		// get data info
		dim = new int[32];
		info = new int[2];
		try{
			nf.getinfo(dim, info);
		}catch(NexusException ne){
			return null;
		}

		// create graph data node and the rank and dim nodes
		String pathel[] = nxpath.substring(1).split("/");
		mygraph = new GraphicsNode(graphics, pathel[pathel.length-2]);
		mygraph.setProperty("type", "graphdata");
		testNameAndInsert(graphics, mygraph);

		tmp = new InternalParameter(mygraph,"rank");
		mygraph.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1,helpdim);
		iv.append(info[0]);
		tmp.updateValue(iv);

		
		tmp = new InternalParameter(mygraph,"dim");
		mygraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = info[0];
		iv = new IntValue(1,helpdim);
		for(i = 0; i < info[0]; i++){
			iv.append(dim[i]);
		}
		tmp.updateValue(iv);
		
		// add our data
		if(info[0] == 3){
			makeFrameSeriesViewer(mygraph, nxpath, info, dim, nf);
			return mygraph;
		} else {
			tmp = new InternalParameter(mygraph,"counts");
			mygraph.insertNode(TreeNode.APPEND, tmp);
			tmp.updateValue(makeValue(nf,dim,info));
			tmp.setProperty("type", "data");
		}
		
		try{	
			nf.closedata();
		}catch(NexusException ne){}
		
		findAxis(nf,nxpath, mygraph);
		
		checkAndDefaultAxis(mygraph, info[0],  dim);

		return mygraph;
	}

	protected TreeNode locateGraphics(TreeNode  root){
		TreeNode graphics = TreeUtil.findChild(root, "graphics");
		if(graphics == null){
			graphics = new TreeNode(root,"graphics");
			root.insertNode(TreeNode.APPEND, graphics);
			graphics.setProperty("type", "graphset");
		}
		return graphics;
	}
	/**
	 * this test if the name for mygraph is already a child of graphics
	 * If so a new name is invented. 
	 * @param graphics The node to insert mygraph to
	 * @param mygraph The node to insert.
	 */
	protected void testNameAndInsert(TreeNode graphics, TreeNode mygraph) {
		int i = 1, idx;
		StringBuffer name;	
		String oldName;
		
		while(TreeUtil.findChild(graphics,mygraph.getName()) != null){
			oldName = mygraph.getName();
			if((idx = oldName.indexOf('_')) > 0 ){
				oldName = oldName.substring(0, idx);
			}
			name = new StringBuffer(oldName);
			name.append('_');
			name.append(Integer.toString(i));
			mygraph.setProperty("name", name.toString());
			i++;
		}
		graphics.insertNode(TreeNode.APPEND, mygraph);
	}
	protected void makeFrameSeriesViewer(TreeNode mygraph, String nxpath, int  info[], 
			int dim[], FlatNexusFile nf) {
		int helpdim[] = new int[1], i, idim;
		IntValue iv;
		InternalParameter work;
		String axis;
		TreeNode node;
		
		mygraph.setProperty("viewer", "mountaingumui.frameseries");
		
		work = (InternalParameter)TreeUtil.findChild(mygraph,"dim");
		work.setValue(Integer.toString(dim[1]) + " " + Integer.toString(dim[2]));
		
	    work = (InternalParameter)TreeUtil.findChild(mygraph, "rank");
		work.setValue("2");

		work = new InternalParameter(mygraph,"currentFrame");
		helpdim[0] = dim[0];
		iv = new IntValue(1,helpdim);
		iv.append(dim[0]-1);
		work.updateValue(iv);
		mygraph.insertNode(TreeNode.APPEND, work);
		
		work = new InternalParameter(mygraph,"counts");
		helpdim[0] = dim[1]*dim[2];
		iv = new IntValue(1,helpdim);
		work.updateValue(iv);
		work.setProperty("type", "data");
		mygraph.insertNode(TreeNode.APPEND, work);
		String countpath = TreeUtil.pathForNode(work);
		
		InternalParameter cur = new InternalParameter(mygraph,"currentData");
		iv = new IntValue(1,helpdim);
		cur.updateValue(iv);
		mygraph.insertNode(TreeNode.APPEND, cur);

		CommandNode oldframe = new OldFrameCommand(mygraph,"getoldframe");
		oldframe.setProperty("countpath", countpath);
		oldframe.setProperty("nxpath", nxpath);
		oldframe.setProperty("type", "command");
		mygraph.insertNode(TreeNode.APPEND, oldframe);
		InternalParameter num = (InternalParameter) TreeUtil.findChild(oldframe, "framenumber");
		num.setValue(Integer.toString(dim[0]-1));
		oldframe.start();
		
		cur.updateValue(work.getValue());

		try{	
			nf.closedata();
		}catch(NexusException ne){}
		
		findAxis(nf,nxpath, mygraph);
		
		/**
		 * Axis numbers in the NeXus file refer to a 3D array. This has to be 
		 * fixed up here.
		 */
		for(i = 0; i < mygraph.countChildren(); i++){
			node = mygraph.getChild(i);
			if(node instanceof InternalParameter){
				work = (InternalParameter)node;
			} else {
				continue;
			}
			axis = work.getProperty("type");
			if(axis != null && axis.equalsIgnoreCase("axis")){
				axis = work.getProperty("dim");
				idim = Integer.parseInt(axis);
				idim--;
				work.setProperty("dim", Integer.toString(idim));
			}
		}
		
		checkAndDefaultAxis(mygraph, info[0],  dim);
	
		
	}
	/**
	 * check if each axis has been found; if not add a suitable 
	 * default axis.
	 * @param mygraph The graphics node to check
	 * @param rank the rank of the data
	 * @param dim The dimensions of the data
	 */
	protected  void checkAndDefaultAxis(TreeNode mygraph, int rank, int[] dim) {
		int i;
		
		for(i = 0; i < rank; i++){
			TreeNode ax = locateAxis(mygraph,i);
			if(ax == null){
				addDefaultAxis(mygraph, i, dim[i]);
			}
		}
	}
	protected void addDefaultAxis(TreeNode mygraph, int dim, int size) {
		String name = "axis" + dim;
		InternalParameter ax = new InternalParameter(mygraph, name);
		ax.setProperty("type", "axis");
		ax.setProperty("dim", Integer.toString(dim));
		int tmp[] = new int[1];
		tmp[0] = size;
		DoubleValue dv = new DoubleValue(1, tmp);
		for(int i = 0; i < size; i++){
			dv.append(i);
		}
		ax.updateValue(dv);
		mygraph.insertNode(TreeNode.APPEND, ax);
	}
	/**
	 * try to find a suitable axis for dimension i 
	 * @param mygraph The node to search
	 * @param i The dim to check
	 * @return A suitable TreeNode or null
	 */
	protected TreeNode  locateAxis(TreeNode mygraph, int i) {
		ChildStream cs = new ChildStream(mygraph);
		TreeNode ax;
		String dim;
		
		while((ax = (TreeNode)cs.next()) != null){
			dim = ax.getProperty("dim");
			if(dim != null){
				if(Integer.parseInt(dim) == i){
					return ax;
				}
			}
		}
		return null;
	}
	/**
	 * Try to locate axis nodes 
	 * @param nf
	 * @param nxpath
	 * @param mygraph
	 */
	protected void findAxis(FlatNexusFile nf, String nxpath, TreeNode mygraph) {
		String name, type, axAttr;
		try{
			Hashtable dir = nf.groupdir();
			Enumeration it = dir.keys();
			while(it.hasMoreElements()){
				name = (String)it.nextElement();
				type = (String)dir.get(name);
				if(type.equalsIgnoreCase("SDS")){
					nf.opendata(name);
					axAttr = getAttr(nf,"axis");
					if(axAttr != null){
						makeAxis(nf, name, axAttr, mygraph);
					}
					nf.closedata();
				}
			}
		}catch(NexusException ne){
			
		}
	}
	/**
	 * create an axis node
	 * @param nf
	 * @param name
	 * @param axAttr
	 * @param mygraph
	 * @throws NexusException
	 */
	protected  void makeAxis(FlatNexusFile nf, String name, String axAttr, TreeNode mygraph)
		throws NexusException {
		InternalParameter ax = new InternalParameter(mygraph, name);
		mygraph.insertNode(TreeNode.APPEND, ax);
		ax.setProperty("type", "axis");
		int axno =  Integer.parseInt(axAttr);
		ax.setProperty("dim", Integer.toString(axno-1));
		int dim[] = new int[32];
		int info[] = new int[2];
		nf.getinfo(dim,info);
		ax.updateValue(makeValue(nf,dim,info));
	}
	/**
	 * test if this node has already been seen.
	 * @param nf
	 * @param nxpath
	 * @return
	 */
	protected boolean alreadySeen(NeXusFileInterface nf, String nxpath){
		if(mapset.contains(nxpath)){
			return true;
		}
		String link = getAttr(nf,"target");
		if(link != null){
			if(mapset.contains(link)){
				return true;
			}
		}
		return false;
	}
	/**
	 * helper function to get an attribute
	 * @param nf
	 * @param name
	 * @return
	 */
	protected String getAttr(NeXusFileInterface nf, String name){
		try{
			Hashtable h = nf.attrdir();
			AttributeEntry e = (AttributeEntry)h.get(name);
			if(e == null){
				return null;
			}
			int dim[] = new int[2];
			dim[0] = e.length;
			dim[1] = e.type;
			byte bdata[] = new byte[e.length];
			nf.getattr(name, bdata, dim);
			byte byteArr[] = fixTrailingZero(bdata);
			// Check to see if the attribute is some sort of integer
			if (e.type > NexusFile.NX_BINARY) {
				Byte myByte = new Byte(byteArr[0]);
				long myLong = myByte.longValue();
				//System.out.println(name + " : " + Long.toString(myLong));
				return Long.toString(myLong);
			}
			return new String(byteArr);
		}catch(NexusException ne){
			return null;
		}
	}
	/**
	 * This removes trailing zeros from bdata. Trailing zero bytes confuse
	 * the conversion to string. 
	 * @param bdata
	 * @return
	 */
	private byte[] fixTrailingZero(byte[] bdata) {
		int i, length = 0;
		for(i = 0; i < bdata.length; i++){
			if(bdata[i] != 0){
				length++;
			}
		}
		byte result[] = new byte[length];
		for(i = 0; i < length; i++){
			result[i] = bdata[i];
		}
		return result;
	}
	/**
	 * reset ourselves...
	 */
	public void reset() {
		mapset.clear();
	}
}
