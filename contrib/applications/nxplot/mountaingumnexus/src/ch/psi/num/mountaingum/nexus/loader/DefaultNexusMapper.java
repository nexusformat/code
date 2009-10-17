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
}
