/**
 * This is the main driver class for loading  NeXus file data into 
 * a tree. I first build a list of all paths to consider, then 
 * iterate through this list. The idea is that the mapper may choose 
 * to step through the tree at its own pleasure in order to find axis 
 * data or whatever. 
 *
 * This also supports saving of NeXus trees.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.Hashtable;

import org.nexusformat.AttributeEntry;
import org.nexusformat.NexusException;
import org.nexusformat.NexusFile;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.nexus.Perspective.NameView;
import ch.psi.num.mountaingum.nexus.Perspective.NexusTree;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.GraphicsNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.LongValue;
import ch.psi.num.mountaingum.tree.NodeValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class NexusLoader {

	private HashSet<String> checkedPaths = new HashSet<String>();
	private TreeNode root;
	private FlatNexusFile nf;

	public NexusLoader() {
	}

	public TreeNode loadNexusIntoTree(String filename) throws IOException {
		TreeNode oldTree = root;
		root = null;
		
		nf = null;
		boolean canWrite = false;

		File f = new File(filename);
		if (!f.canRead()) {
			throw new IOException("File not readable");
		}
		canWrite = f.canWrite();

		try {
			if (canWrite) {
				nf = new FlatNexusFile(filename, NexusFile.NXACC_RDWR);
			} else {
				nf = new FlatNexusFile(filename, NexusFile.NXACC_READ);
			}
		} catch (NexusException ne) {
			throw new IOException(ne.getMessage());
		}

		buildTree(filename);

		setPrivilege(root, canWrite);

		try {
			nf.close();
		} catch (NexusException ne) {
			throw new IOException(ne.getMessage());
		}

		//If all went well, destroy old tree
		//TODO
		//oldTree.dispose();
		return root;
	}

	private void recurseFile(TreeNode parent, String path)
			throws NexusException {
		String name, type, newPath;
		StringBuffer stb;

		Hashtable dir = nf.groupdir();
		Enumeration d = dir.keys();
		while (d.hasMoreElements()) {
			name = (String) d.nextElement();
			type = (String) dir.get(name);
			if (type.equalsIgnoreCase("CDF0.0")) {
				continue;
			}
			stb = new StringBuffer();
			stb.append(path);
			stb.append('/');
			stb.append(name);
			newPath = stb.toString();
			TreeNode nextLevel = makeNode(parent, type, newPath);
			if (!type.equalsIgnoreCase("SDS") && nextLevel != null) {
				nf.opengroup(name, type);
				recurseFile(nextLevel, newPath);
				nf.closegroup();
			}
		}
	}

	private class PrivFunc implements IFunc {
		private boolean canWrite;

		PrivFunc(boolean canWrite) {
			this.canWrite = canWrite;
		}

		public Object apply(Object o) {
			if (o instanceof ParameterNode) {
				ParameterNode pn = (ParameterNode) o;
				pn.setReadOnly(!canWrite);
			}
			return null;
		}
	}

	private void setPrivilege(TreeNode root, boolean canWrite) {
		FuncUtil.map(root.getTreeStream(), new PrivFunc(canWrite));
	}

	private void buildTree(String filename) throws IOException {

		root = new TreeNode(null, "");
		root.setProperty("filename", filename);
		locateGraphics();

		try {
			recurseFile(root, "");
		} catch (NexusException e) {
			throw new IOException(e);
		}
	}

	// private NeXusMapper findMapper(FlatNexusFile nf) {
	// NeXusMapper o;
	// IConfigurationElement[] decl =
	// Platform.getExtensionRegistry().getConfigurationElementsFor("ch.psi.num.mountaingum.nexus.loader.NexusMapper");
	// for(int i = 0; i < decl.length; i++){
	// IConfigurationElement e = decl[i];
	// try{
	// o = (NeXusMapper)e.createExecutableExtension("className");
	// if(o.isApplicable(nf)){
	// return o;
	// }
	// }catch(CoreException eva){
	// UIRegistry.getAdapter().displayError("Failed to load class for " +
	// e.getAttribute("className"));
	// }
	// }
	// return new DefaultNexusMapper();
	// }

	public TreeNode getTree() {
		NexusTree btv = (NexusTree) RCPUtil.findView(NexusTree.ID);
		if (btv == null) {
			// This means that the Nexus perspective has not yet been opened
			return null;
		}
		return btv.getTree();
	}

	private class SaveNXPar implements IFunc {
		private FlatNexusFile nf;

		SaveNXPar(FlatNexusFile nf) {
			this.nf = nf;
		}

		public Object apply(Object o) {
			NexusParameter nxpar;
			String path;
			int dim[], info[], i;

			if (o instanceof NexusParameter) {
				nxpar = (NexusParameter) o;
				if (nxpar.getProperty("mogrified") != null) {
					path = nxpar.getProperty("nxpath");
					try {
						nf.openpath(path);
						dim = new int[32];
						info = new int[2];
						nf.getinfo(dim, info);
						switch (info[1]) {
						case NexusFile.NX_CHAR:
						case NexusFile.NX_INT8:
						case NexusFile.NX_UINT8:
							String data = nxpar.getValue().toString();
							nf.putdata(data.getBytes());
							break;
						case NexusFile.NX_INT16:
						case NexusFile.NX_UINT16:
							IntValue iv = (IntValue) nxpar.getValue();
							short sdata[] = new short[iv.getLength()];
							int idata[] = iv.getData();
							for (i = 0; i < sdata.length; i++) {
								sdata[i] = (short) idata[i];
							}
							nf.putdata(sdata);
							sdata = null;
							idata = null;
							break;
						case NexusFile.NX_INT32:
						case NexusFile.NX_UINT32:
							IntValue iiv = (IntValue) nxpar.getValue();
							nf.putdata(iiv.getData());
							break;
						case NexusFile.NX_INT64:
						case NexusFile.NX_UINT64:
							LongValue lv = (LongValue) nxpar.getValue();
							nf.putdata(lv.getData());
							break;
						case NexusFile.NX_FLOAT32:
							DoubleValue dv = (DoubleValue) nxpar.getValue();
							double ddata[] = dv.getData();
							float fdata[] = new float[ddata.length];
							for (i = 0; i < ddata.length; i++) {
								fdata[i] = (float) ddata[i];
							}
							nf.putdata(fdata);
							fdata = null;
							ddata = null;
							break;
						case NexusFile.NX_FLOAT64:
							DoubleValue ddv = (DoubleValue) nxpar.getValue();
							nf.putdata(ddv.getData());
							break;
						}
					} catch (NexusException ne) {
						SysRegistry.getLogger()
								.error("Failed to write " + path);
					}
				}
			}
			return null;
		}
	}

	/**
	 * the main mapping function, entry to all the work
	 */
	public TreeNode makeNode(TreeNode parent, String nxclass, String nxpath) {

		if (alreadySeen(nxpath)) {
			return null;
		}

		TreeNode node;

		if (nxclass.equalsIgnoreCase("SDS")) {
			node = makeParameterNode(parent, nxpath, nxclass);
		} else {
			node = makeGroupNode(parent, nxpath, nxclass);
		}

		addPath(nxpath);

		return node;
	}

	protected void addPath(String nxpath) {
		checkedPaths.add(nxpath);
		//TODO
//		try {
//			nf.openpath(nxpath);
//			/**
//			 * This section is a workaround for a problem with attribute reading
//			 * through the Java-API. This can be removed once nxinitattrdir()
//			 * has been included and is used by nf.attrdir()
//			 */
//			String pathel[] = nxpath.substring(1).split("/");
//			String name = pathel[pathel.length - 1];
//			nf.closedata();
//			nf.opendata(name);
//			String link = getAttr("target");
//			if (link != null) {
//				checkedPaths.add(link);
//			}
//		} catch (NexusException ne) {
//		}
	}

	/*
	 * This creates all the group hierarchy components till nxpath.
	 */
	protected TreeNode makeGroupNode(TreeNode parent, String nxpath, String type) {
		String pathElement[] = nxpath.substring(1).split("/");
		String name = pathElement[pathElement.length - 1];
		TreeNode current;

		current = new TreeNode(parent, name);
		if (type == null) {
			System.out.println("could not get type for " + nxpath);
			type = "NXwidget";
		}
		current.setProperty("type", type);
		parent.insertNode(TreeNode.APPEND, current);

		return current;
	}

	/**
	 * make any parameter node
	 * 
	 * @param root
	 * @param nf
	 * @param parent
	 * @param nxpath
	 * @param nxclass
	 * @return
	 */
	protected TreeNode makeParameterNode(TreeNode parent, String nxpath,
			String nxclass) {
		int dim[], info[], totalLength = 1;
		TreeNode node = null;

		dim = new int[32];
		info = new int[2];
		String pathel[] = nxpath.substring(1).split("/");
		String name = pathel[pathel.length - 1];

		try {

			nf.opendata(name);

			nf.getinfo(dim, info);
			for (int i = 0; i < info[0]; i++) {
				totalLength *= dim[i];
			}

			Hashtable attr = nf.attrdir();
			if (attr.get("axis") != null) {
				// ignore: will be picked up when building graph data
			} else if (attr.get("signal") != null || totalLength > 100) {
				System.out.println("Making graphnode for " + nxpath);
				node = makeGraphNode(parent, nxpath);
			} else {
				node = makeSimpleParNode(parent, nxpath, dim, info);
			}
		} catch (NexusException ne) {
			ne.printStackTrace();
		} 
		return node;
	}

	/**
	 * make a simple parameter node
	 * 
	 * @param root
	 * @param nf
	 * @param nxpath
	 * @param dim
	 * @param info
	 */
	protected TreeNode makeSimpleParNode(TreeNode parent, String nxpath,
			int[] dim, int[] info) {
		String name, pathel[];
		NexusParameter par;

		pathel = nxpath.substring(1).split("/");
		name = pathel[pathel.length - 1];

		par = new NexusParameter(parent, name);
		parent.insertNode(TreeNode.APPEND, par);

		NodeValue v = makeValue(dim, info);
		par.updateValue(v);
		par.setProperty("nxpath", nxpath);
		return par;
	}

	/**
	 * convert the current SDS to a GTSE NodeValue
	 * 
	 * @param nf
	 * @param dim
	 * @param info
	 * @return
	 */
	private NodeValue makeValue(int[] dim, int[] info) {
		NodeValue v;
		IntValue iv;
		DoubleValue dv;
		LongValue lv;
		int i;

		switch (info[1]) {
		case NexusFile.NX_CHAR:
		case NexusFile.NX_INT8:
		case NexusFile.NX_UINT8:
			v = new NodeValue();
			v.setFromString(nf.gettextdata());
			return v;
		case NexusFile.NX_INT16:
		case NexusFile.NX_UINT16:
			short sdata[] = nf.getshortdata();
			iv = new IntValue(info[0], dim);
			for (i = 0; i < sdata.length; i++) {
				iv.append(sdata[i]);
			}
			return iv;
		case NexusFile.NX_INT32:
		case NexusFile.NX_UINT32:
			int idata[] = nf.getintdata();
			iv = new IntValue(info[0], dim);
			iv.setData(idata);
			return iv;
		case NexusFile.NX_INT64:
		case NexusFile.NX_UINT64:
			long ldata[] = nf.getlongdata();
			lv = new LongValue(info[0], dim);
			lv.setData(ldata);
			return lv;
		case NexusFile.NX_FLOAT32:
			float fdata[] = nf.getfloatdata();
			dv = new DoubleValue(info[0], dim);
			for (i = 0; i < fdata.length; i++) {
				dv.append(fdata[i]);
			}
			return dv;
		case NexusFile.NX_FLOAT64:
			double ddata[] = nf.getdoubledata();
			dv = new DoubleValue(info[0], dim);
			dv.setData(ddata);
			return dv;
		}
		return null;
	}

	/**
	 * make a graphics node
	 * 
	 * @param root
	 * @param nf
	 * @param parent
	 * @param nxpath
	 * @return
	 */
	protected TreeNode makeGraphNode(TreeNode parent, String nxpath) {
		TreeNode graphics, mygraph = null;
		InternalParameter tmp;
		int dim[], info[], helpdim[], i;
		IntValue iv;

		// ensure graphics node
		graphics = locateGraphics();

		// get data info
		dim = new int[32];
		info = new int[2];
		try {
			nf.getinfo(dim, info);
		} catch (NexusException ne) {
			return null;
		}

		// create graph data node and the rank and dim nodes
		String pathel[] = nxpath.substring(1).split("/");
		mygraph = new GraphicsNode(graphics, pathel[pathel.length - 2]);
		mygraph.setProperty("type", "graphdata");
		testNameAndInsert(graphics, mygraph);

		tmp = new InternalParameter(mygraph, "rank");
		mygraph.insertNode(TreeNode.APPEND, tmp);
		helpdim = new int[1];
		helpdim[0] = 1;
		iv = new IntValue(1, helpdim);
		iv.append(info[0]);
		tmp.updateValue(iv);

		tmp = new InternalParameter(mygraph, "dim");
		mygraph.insertNode(TreeNode.APPEND, tmp);
		helpdim[0] = info[0];
		iv = new IntValue(1, helpdim);
		for (i = 0; i < info[0]; i++) {
			iv.append(dim[i]);
		}
		tmp.updateValue(iv);

		// add our data
		if (info[0] == 3) {
			makeFrameSeriesViewer(mygraph, nxpath, info, dim);
			return mygraph;
		} else {
			tmp = new InternalParameter(mygraph, "counts");
			mygraph.insertNode(TreeNode.APPEND, tmp);
			tmp.updateValue(makeValue(dim, info));
			tmp.setProperty("type", "data");
		}

		try {
			nf.closedata();
		} catch (NexusException ne) {
		}

		findAxis(nxpath, mygraph);

		checkAndDefaultAxis(mygraph, info[0], dim);

		return mygraph;
	}

	protected TreeNode locateGraphics() {
		TreeNode graphics = TreeUtil.findChild(root, "graphics");
		if (graphics == null) {
			graphics = new TreeNode(root, "graphics");
			root.insertNode(TreeNode.APPEND, graphics);
			graphics.setProperty("type", "graphset");
		}
		return graphics;
	}

	/**
	 * this test if the name for mygraph is already a child of graphics If so a
	 * new name is invented.
	 * 
	 * @param graphics
	 *            The node to insert mygraph to
	 * @param mygraph
	 *            The node to insert.
	 */
	protected void testNameAndInsert(TreeNode graphics, TreeNode mygraph) {
		int i = 1, idx;
		StringBuffer name;
		String oldName;

		while (TreeUtil.findChild(graphics, mygraph.getName()) != null) {
			oldName = mygraph.getName();
			if ((idx = oldName.indexOf('_')) > 0) {
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

	protected void makeFrameSeriesViewer(TreeNode mygraph, String nxpath,
			int info[], int dim[]) {
		int helpdim[] = new int[1], i, idim;
		IntValue iv;
		InternalParameter work;
		String axis;
		TreeNode node;

		mygraph.setProperty("viewer", "mountaingumui.frameseries");

		work = (InternalParameter) TreeUtil.findChild(mygraph, "dim");
		work
				.setValue(Integer.toString(dim[1]) + " "
						+ Integer.toString(dim[2]));

		work = (InternalParameter) TreeUtil.findChild(mygraph, "rank");
		work.setValue("2");

		work = new InternalParameter(mygraph, "currentFrame");
		helpdim[0] = dim[0];
		iv = new IntValue(1, helpdim);
		iv.append(dim[0] - 1);
		work.updateValue(iv);
		mygraph.insertNode(TreeNode.APPEND, work);

		work = new InternalParameter(mygraph, "counts");
		helpdim[0] = dim[1] * dim[2];
		iv = new IntValue(1, helpdim);
		work.updateValue(iv);
		work.setProperty("type", "data");
		mygraph.insertNode(TreeNode.APPEND, work);
		String countpath = TreeUtil.pathForNode(work);

		InternalParameter cur = new InternalParameter(mygraph, "currentData");
		iv = new IntValue(1, helpdim);
		cur.updateValue(iv);
		mygraph.insertNode(TreeNode.APPEND, cur);

		CommandNode oldframe = new OldFrameCommand(mygraph, "getoldframe");
		oldframe.setProperty("countpath", countpath);
		oldframe.setProperty("nxpath", nxpath);
		oldframe.setProperty("type", "command");
		mygraph.insertNode(TreeNode.APPEND, oldframe);
		InternalParameter num = (InternalParameter) TreeUtil.findChild(
				oldframe, "framenumber");
		num.setValue(Integer.toString(dim[0] - 1));
		oldframe.start();

		cur.updateValue(work.getValue());

		try {
			nf.closedata();
		} catch (NexusException ne) {
		}

		findAxis(nxpath, mygraph);

		/**
		 * Axis numbers in the NeXus file refer to a 3D array. This has to be
		 * fixed up here.
		 */
		for (i = 0; i < mygraph.countChildren(); i++) {
			node = mygraph.getChild(i);
			if (node instanceof InternalParameter) {
				work = (InternalParameter) node;
			} else {
				continue;
			}
			axis = work.getProperty("type");
			if (axis != null && axis.equalsIgnoreCase("axis")) {
				axis = work.getProperty("dim");
				idim = Integer.parseInt(axis);
				idim--;
				work.setProperty("dim", Integer.toString(idim));
			}
		}

		checkAndDefaultAxis(mygraph, info[0], dim);
	}

	/**
	 * check if each axis has been found; if not add a suitable default axis.
	 * 
	 * @param mygraph
	 *            The graphics node to check
	 * @param rank
	 *            the rank of the data
	 * @param dim
	 *            The dimensions of the data
	 */
	protected void checkAndDefaultAxis(TreeNode mygraph, int rank, int[] dim) {
		int i;

		for (i = 0; i < rank; i++) {
			TreeNode ax = locateAxis(mygraph, i);
			if (ax == null) {
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
		for (int i = 0; i < size; i++) {
			dv.append(i);
		}
		ax.updateValue(dv);
		mygraph.insertNode(TreeNode.APPEND, ax);
	}

	/**
	 * try to find a suitable axis for dimension i
	 * 
	 * @param mygraph
	 *            The node to search
	 * @param i
	 *            The dim to check
	 * @return A suitable TreeNode or null
	 */
	protected TreeNode locateAxis(TreeNode mygraph, int i) {
		ChildStream cs = new ChildStream(mygraph);
		TreeNode ax;
		String dim;

		while ((ax = (TreeNode) cs.next()) != null) {
			dim = ax.getProperty("dim");
			if (dim != null) {
				if (Integer.parseInt(dim) == i) {
					return ax;
				}
			}
		}
		return null;
	}

	/**
	 * Try to locate axis nodes
	 * 
	 * @param nf
	 * @param nxpath
	 * @param mygraph
	 */
	protected void findAxis(String nxpath, TreeNode mygraph) {
		String name, type, axAttr;
		try {
			Hashtable dir = nf.groupdir();
			Enumeration it = dir.keys();
			while (it.hasMoreElements()) {
				name = (String) it.nextElement();
				type = (String) dir.get(name);
				if (type.equalsIgnoreCase("SDS")) {
					nf.opendata(name);
					axAttr = getAttr("axis");
					if (axAttr != null) {
						makeAxis(name, axAttr, mygraph);
					}
					nf.closedata();
				}
			}
		} catch (NexusException ne) {

		}
	}

	/**
	 * create an axis node
	 * 
	 * @param nf
	 * @param name
	 * @param axAttr
	 * @param mygraph
	 * @throws NexusException
	 */
	protected void makeAxis(String name, String axAttr, TreeNode mygraph)
			throws NexusException {
		InternalParameter ax = new InternalParameter(mygraph, name);
		mygraph.insertNode(TreeNode.APPEND, ax);
		ax.setProperty("type", "axis");
		int axno = Integer.parseInt(axAttr);
		ax.setProperty("dim", Integer.toString(axno - 1));
		int dim[] = new int[32];
		int info[] = new int[2];
		nf.getinfo(dim, info);
		ax.updateValue(makeValue(dim, info));
	}

	/**
	 * test if this node has already been seen.
	 * 
	 * @param nf
	 * @param nxpath
	 * @return
	 */
	protected boolean alreadySeen(String nxpath) {
		if (checkedPaths.contains(nxpath)) {
			return true;
		}
		String link = getAttr("target");
		if (link != null) {
			if (checkedPaths.contains(link)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * helper function to get an attribute
	 * 
	 * @param nf
	 * @param name
	 * @return
	 */
	protected String getAttr(String name) {
		try {
			Hashtable h = nf.attrdir();
			AttributeEntry e = (AttributeEntry) h.get(name);
			if (e == null) {
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
				return Long.toString(myLong);
			}
			return new String(byteArr);
		} catch (NexusException ne) {
			return null;
		}
	}

	/**
	 * This removes trailing zeros from bdata. Trailing zero bytes confuse the
	 * conversion to string.
	 * 
	 * @param bdata
	 * @return
	 */
	private byte[] fixTrailingZero(byte[] bdata) {
		int i, length = 0;
		for (i = 0; i < bdata.length; i++) {
			if (bdata[i] != 0) {
				length++;
			}
		}
		byte result[] = new byte[length];
		for (i = 0; i < length; i++) {
			result[i] = bdata[i];
		}
		return result;
	}

	/**
	 * reset ourselves...
	 */
	public void reset() {
		checkedPaths.clear();
	}

	public void saveTree(TreeNode root, String filename) throws IOException {

		String oldfile = root.getProperty("filename");
		if (!oldfile.equals(filename)) {
			/**
			 * To fix this I need to copy the old file on the new file and then
			 * apply the changes. For this I would need the apache commons-io
			 * library, or include it into the source or whatever. As of now
			 * (08/2009) I cannot yet bring myself to add the additional
			 * dependency.
			 */
			throw new IOException("Filename mismatch");
		}

		try {
			FlatNexusFile nf = new FlatNexusFile(filename, NexusFile.NXACC_RDWR);
			FuncUtil.map(root.getTreeStream(), new SaveNXPar(nf));
			nf.close();
		} catch (NexusException ne) {
			throw new IOException(ne.getMessage());
		}
	}

	public void viewNode(TreeNode root) {
		if (root == null) {
			return;
		}

		NexusTree btv = (NexusTree) RCPUtil.findView(NexusTree.ID);
		if (btv == null) {
			// This means that the Nexus perspective has not yet been opened
			return;
		}

		btv.setTree(root);
		EditorView ed = (EditorView) RCPUtil.findView(TreeEditorView.ID, "1");
		if (ed != null) {
			ed.disconnect();
		}

		NameView nv = (NameView) RCPUtil.findView(NameView.ID);
		if (nv != null) {
			nv.setName(root.getProperty("filename"));
		}
	}
}