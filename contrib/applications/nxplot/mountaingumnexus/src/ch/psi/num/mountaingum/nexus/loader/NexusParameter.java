/**
 * This is a very simple derived class of InternalParameter which makes a 
 * property by setting the property mogified if it has been changed. 
 * The intent is that, when saving files, this property will be examined and 
 * only those fields changed on file which have this property. The other 
 * operation implemented by this class is deferred loading, i.e. data is 
 * only read from file when it is actually required for the calculation of a 
 * plot.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import org.nexusformat.NexusException;

import ch.psi.num.mountaingum.tree.NodeValue;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;


public class NexusParameter extends ch.psi.num.mountaingum.tree.InternalParameter {
	private boolean loaded;
	
	public NexusParameter(TreeNode p, String name) {
		super(p, name);
		loaded = false;
	}

	public void setValue(String val) {
		super.setValue(val);
		setProperty("mogrified", "true");
		loaded = true;
	}
	public NodeValue getValue() {
		if(!loaded){
			loadData();
		}
		return v;
	}

	private void loadData() {
		int dim[], info[];
		TreeNode root = TreeUtil.findRoot(this);
		
		String filename = root.getProperty("filename");
		String nxpath = getProperty("nxpath");

		//System.out.println("Doing deferred load of " + nxpath);
		
		try{
			FlatNexusFile nf = new FlatNexusFile(filename,FlatNexusFile.NXACC_READ);
			nf.openpath(nxpath);
			dim = new int[32];
			info = new int[2];
			nf.getinfo(dim,info);
			NodeValue v = NexusLoader.makeValue(nf, dim, info);
			updateValue(v);
			nf.close();
		}catch(NexusException ne){
			UIRegistry.getAdapter().displayError("Exception " + ne.getMessage() + " while loading " + nxpath);
		}
	}

	public void updateValue(NodeValue v) {
		loaded = true;
		super.updateValue(v);
	}
}
