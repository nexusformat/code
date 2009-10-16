/**
 * This is a node which can load old frames from a NeXus data file.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.loader;

import ncsa.hdf.hdflib.HDFNativeData;

import org.nexusformat.NexusException;
import org.nexusformat.NexusFile;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.IntValue;
import ch.psi.num.mountaingum.tree.InternalParameter;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.util.Array2DUtil;

public class OldFrameCommand extends CommandNode {

	public OldFrameCommand(TreeNode parent, String name) {
		super(parent, name);
		InternalParameter no = new InternalParameter(this,"framenumber");
		int helpdim[] = new int[1];
		helpdim[0] = 1;
		IntValue iv = new IntValue(1,helpdim);
		iv.append(0);
		no.updateValue(iv);
		insertNode(TreeNode.APPEND,no);
	}

	public void start() {
		int start[], size[];
		int dim[], info[];
		TreeNode root = TreeUtil.findRoot(this);
		
		String filename = root.getProperty("filename");
		String nxpath = getProperty("nxpath");
		int frameNo = getFrameNo();
		start = new int[3];
		size = new int[3];
		
		InternalParameter dn = (InternalParameter) TreeUtil.searchNode(root, getProperty("countpath"));
		
		if(getProperty("buenzel") != null){
			loadThorough(filename, frameNo, dn);
			return;
		}
		try{
			FlatNexusFile nx = new FlatNexusFile(filename, NexusFile.NXACC_READ);
			nx.openpath(nxpath);
			dim = new int[32];
			info = new int[2];
			nx.getinfo(dim,info);
			start[0] = Math.max(0,frameNo-1);
			if(start[0] >= dim[0]){
				start[0] = dim[0] -1;
			}
			start[1] = 0;
			start[2] = 0;
			size[0] = 1;
			size[1] = dim[1];
			size[2] = dim[2];
			byte raw[] = nx.getrawslab(start, size, info[1]);
			if(info[1] != NexusFile.NX_INT32 && info[1] != NexusFile.NX_UINT32){
				/**
				 * TODO: fix this to handle all types of data?
				 */
				throw new NexusException("Unsupported data conversion");
			}
			int data[] = HDFNativeData.byteToInt(raw);
			if(getProperty("swap") != null){
				int targetdata[] = new int[data.length];
				Array2DUtil au = new Array2DUtil();
				au.rotate90(data, targetdata, dim[2], dim[1]);
				data = targetdata;
			}
			IntValue iv = (IntValue) dn.getValue();
			iv.setData(data);
			dn.notifyUpdate();
			raw = null;
			data = null;
			nx.close();
		}catch(Exception noemi){
			UIRegistry.getAdapter().displayError(noemi.getMessage());
		}
	}

	private void loadThorough(String filename, int frameNo, InternalParameter dn) {
		int data[], rawdata[][][], dim[], info[], i, j;
		
		try{
			FlatNexusFile nx = new FlatNexusFile(filename, NexusFile.NXACC_READ);
			nx.openpath(getProperty("nxpath"));
			dim = new int[32];
			info = new int[2];
			nx.getinfo(dim,info);
			rawdata = new int[dim[0]][dim[1]][dim[2]];
			data = new int[dim[1]*dim[2]];
			nx.getdata(rawdata);
			for(j = 0; j < dim[2]; j++ ){
				for(i = 0; i < dim[1]; i++){
					data[i + j*dim[1]] = rawdata[frameNo][j][i];
				}
			}
			nx.close();
			IntValue iv = (IntValue)dn.getValue();
			iv.setData(data);
			dn.notifyUpdate();
			rawdata = null;
			data = null;
		}catch(Exception eva){
			UIRegistry.getAdapter().displayError(eva.getMessage());
		}
	}

	private int getFrameNo() {
		InternalParameter no = (InternalParameter) TreeUtil.findChild(this, "framenumber");
		IntValue iv = (IntValue) no.getValue();
		return iv.getValue(0);
	}

}
