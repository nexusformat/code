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
	@Override
	public void map(TreeNode root, FlatNexusFile nf, String nxclass,
			String nxpath) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void reset() {
		// TODO Auto-generated method stub
		
	}


<<<<<<< HEAD:contrib/applications/nxplot/mountaingumnexus/src/ch/psi/num/mountaingum/nexus/loader/DefaultNexusMapper.java
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
=======
>>>>>>> refs #195:contrib/applications/nxplot/mountaingumnexus/src/ch/psi/num/mountaingum/nexus/loader/DefaultNexusMapper.java
}
