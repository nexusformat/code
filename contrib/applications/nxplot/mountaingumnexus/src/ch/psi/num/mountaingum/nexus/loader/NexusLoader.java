/**
 * This is the main driver class for loading  NeXus file data into 
 * a GTSE tree. I first build a list of all paths to consider, then 
 * iterate through this list. The idea is that the mapper may choose 
 * to step through the file at its own pleasure in order to find axis 
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
import java.util.Hashtable;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.Platform;
import org.nexusformat.NexusException;
import org.nexusformat.NexusFile;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.nexus.Perspective.NameView;
import ch.psi.num.mountaingum.nexus.Perspective.NexusTree;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.DoubleValue;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.LongValue;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class NexusLoader {

	private Hashtable pathMap;

	public NexusLoader(){
		pathMap = new Hashtable();
	}
	
	public TreeNode loadNexusIntoTree(String filename) throws IOException{
		FlatNexusFile nf = null;
		boolean canWrite = false;
		
		File f = new File(filename);
		if(!f.canRead()){
			throw new IOException("File not readable");
		}
		canWrite = f.canWrite();
		
		try{
			if(canWrite){
				nf = new FlatNexusFile(filename,NexusFile.NXACC_RDWR );
			} else {
				nf = new FlatNexusFile(filename,NexusFile.NXACC_READ );
			}
		}catch(NexusException ne){
			throw new IOException(ne.getMessage());
		}
		
		buildPathMap(nf);
	
		TreeNode root = buildTree(nf,filename);
		
		setPrivilege(root,canWrite);
		try{
			nf.close();
		}catch(NexusException ne){
			throw new IOException(ne.getMessage());
		}
		return root;
	}

	public void viewNode(TreeNode root){
		
		if(root == null){
			return;
		}
		NexusTree btv = (NexusTree)RCPUtil.findView(NexusTree.ID);
		if(btv == null){
			// This means that the Nexus perspective has not yet been opened
			return;
		}
		btv.setTree(root);
		EditorView ed = (EditorView)RCPUtil.findView(TreeEditorView.ID, "1");
		if(ed != null){
			ed.disconnect();
		}
		NameView nv = (NameView)RCPUtil.findView(NameView.ID);
		if(nv != null){
			nv.setName(root.getProperty("filename"));
		}
	}
	private class PrivFunc implements  IFunc {
		private boolean canWrite;
		
		PrivFunc(boolean canWrite){
			this.canWrite = canWrite;
		}
		
		public Object apply(Object o) {
			if(o instanceof ParameterNode){
				ParameterNode pn = (ParameterNode)o;
				pn.setReadOnly(!canWrite);
			}
			return null;
		}
		
	}
	private void setPrivilege(TreeNode root, boolean canWrite) {
		FuncUtil.map(root.getTreeStream(), new PrivFunc(canWrite));
	}

	private TreeNode buildTree(FlatNexusFile nf, String filename) throws IOException {
		String path, type;
		
		TreeNode root = new TreeNode(null,"");
		root.setProperty("filename", filename);
		
		NeXusMapper mapper = findMapper(nf);
		
		Enumeration pathList = pathMap.keys();
		while(pathList.hasMoreElements()){
			path = (String)pathList.nextElement();
			type = (String)pathMap.get(path);
			try{
				nf.openpath(path);
				mapper.map(root, nf, type, path);
			}catch(NexusException ne){
				throw new IOException(ne.getMessage());
			}
		}
		return root;
	}

	private NeXusMapper findMapper(FlatNexusFile nf) {
		NeXusMapper o;
		IConfigurationElement[] decl = Platform.getExtensionRegistry().getConfigurationElementsFor("ch.psi.num.mountaingum.nexus.loader.NexusMapper");
		for(int i = 0; i < decl.length; i++){
			IConfigurationElement e = decl[i];
			try{
					o = (NeXusMapper)e.createExecutableExtension("className");
					if(o.isApplicable(nf)){
						return o;
					}
			}catch(CoreException eva){
				UIRegistry.getAdapter().displayError("Failed to load class for " + e.getAttribute("className"));
			}
		}
		return new DefaultNexusMapper();
	}

	private void buildPathMap(FlatNexusFile nf) throws IOException {
		pathMap.clear();
		try{
			recursePath(nf, "");
		}catch(NexusException ne){
			throw new IOException(ne.getMessage());
		}
	}

	private void recursePath(FlatNexusFile nf, String path) throws NexusException {
		String name, type, newPath;
		StringBuffer stb;
		
		Hashtable dir = nf.groupdir();
		Enumeration d = dir.keys();
		while(d.hasMoreElements()){
			name = (String)d.nextElement();
			type = (String)dir.get(name);
			if(type.equalsIgnoreCase("CDF0.0")){
				continue;
			}
			stb = new StringBuffer();
			stb.append(path);
			stb.append('/');
			stb.append(name);
			newPath = stb.toString();
			pathMap.put(newPath, type);
			if(!type.equalsIgnoreCase("SDS")){
				nf.opengroup(name, type);
				recursePath(nf,newPath);
				nf.closegroup();
			}
		}
	}
	public TreeNode getTree(){
		NexusTree btv = (NexusTree)RCPUtil.findView(NexusTree.ID);
		if(btv == null){
			// This means that the Nexus perspective has not yet been opened
			return null;
		}
		return btv.getTree();
	}
	private class SaveNXPar implements IFunc {
		private FlatNexusFile nf;
		
		SaveNXPar(FlatNexusFile nf){
			this.nf = nf;
		}
		
		public Object apply(Object o) {
			NexusParameter nxpar;
			String path;
			int dim[], info[], i;
			
			if(o instanceof NexusParameter){
				nxpar = (NexusParameter)o;
				if(nxpar.getProperty("mogrified") != null){
					path = nxpar.getProperty("nxpath");
					try{
						nf.openpath(path);
						dim = new int[32];
						info = new int[2];
						nf.getinfo(dim,info);
						switch(info[1]){
						case NexusFile.NX_CHAR:
						case NexusFile.NX_INT8:
						case NexusFile.NX_UINT8:
							String data = nxpar.getValue().toString();
							nf.putdata(data.getBytes());
							break;
						case NexusFile.NX_INT16:
						case NexusFile.NX_UINT16:
							IntValue iv = (IntValue)nxpar.getValue();
							short sdata[] = new short[iv.getLength()];
							int idata[] = iv.getData();
							for(i = 0; i < sdata.length; i++){
								sdata[i] = (short)idata[i];
							}
							nf.putdata(sdata);
							sdata = null;
							idata = null;
							break;
						case NexusFile.NX_INT32:
						case NexusFile.NX_UINT32:
							IntValue iiv = (IntValue)nxpar.getValue();
							nf.putdata(iiv.getData());
							break;
						case NexusFile.NX_INT64:
						case NexusFile.NX_UINT64:
							LongValue lv = (LongValue)nxpar.getValue();
							nf.putdata(lv.getData());
							break;
						case NexusFile.NX_FLOAT32:
							DoubleValue dv = (DoubleValue)nxpar.getValue();
							double ddata[] = dv.getData();
							float fdata[] = new float[ddata.length];
							for(i = 0; i < ddata.length; i++){
								fdata[i] = (float)ddata[i];
							}
							nf.putdata(fdata);
							fdata = null;
							ddata = null;
							break;
						case NexusFile.NX_FLOAT64:
							DoubleValue ddv = (DoubleValue)nxpar.getValue();
							nf.putdata(ddv.getData());
							break;
						}
					}catch(NexusException ne){
						SysRegistry.getLogger().error("Failed to write " + path);
					}
				}
			}
			return null;
		}
	}
	public void saveTree(TreeNode root, String filename) throws IOException{
		
		File f = new File(filename);
		String oldfile = root.getProperty("filename");
		if(!oldfile.equals(filename)){
			/**
			 * To fix this I need to copy the old file on the 
			 * new file and then apply the changes. For this I would need 
			 * the apache commons-io library, or include it into the source or 
			 * whatever. As of now (08/2009) I cannot yet bring myself to add 
			 * the additional dependency. 
			 */
			throw new IOException("Filename mismatch");
		}
		
		try{
			FlatNexusFile nf = new FlatNexusFile(filename,NexusFile.NXACC_RDWR);
			FuncUtil.map(root.getTreeStream(), new SaveNXPar(nf));
			nf.close();
		}catch(NexusException ne){
			throw new IOException(ne.getMessage());
		}		
	}
}
