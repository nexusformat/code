 /* NeXus - Neutron & X-ray Common Data Format
  *
  * NeXus file validation GUI tool.
  *
  * Copyright (C) 2010 Stephen Rankin
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information, see <http://www.neutron.anl.gov/NeXus/>
  *
  * NXReducedToTree.java
  *
  */
package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import java.util.Enumeration;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreePath;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

/**
 * This maps the NX Reduced XML Document (as a DOM document) to
 * a JTree model.
 * @author Stephen Rankin
 */
public class NXReducedToTree extends DefaultTreeModel {

    private ArrayList<TreeModelListener> listenerList =
            new ArrayList<TreeModelListener>();
    private NXNodeMapper root = null;
    
    public NXReducedToTree(TreeNode root) {
        super(root);
        this.root = (NXNodeMapper)root;
    }

    public void updateTree() {

        TreeModelEvent evt = new TreeModelEvent(this,
                                              new Object[] {root});
        fireTreeStructureChanged(evt);
    }

    @Override
    public Object getRoot() {

        return root;
        
    }

    @Override
    public boolean isLeaf(Object aNode) {

        // Determines whether the icon shows up to the left.
        // Return true for any node with no children
        NXNodeMapper node = (NXNodeMapper) aNode;

        if (node.getChildCount() > 0) {
            return false;
        }

        return true;
    }

    @Override
    public int getChildCount(Object parent) {

        NXNodeMapper node = (NXNodeMapper) parent;
        return node.getChildCount();
        
    }

    @Override
    public Object getChild(Object parent, int index) {

        NXNodeMapper node = (NXNodeMapper) parent;
        return node.getChildAt(index);
        
    }

    @Override
    public int getIndexOfChild(Object parent, Object child) {

        NXNodeMapper node = (NXNodeMapper) parent;
        return node.getIndex((NXNodeMapper) child);

    }

    @Override
    public void valueForPathChanged(TreePath path, Object newValue) {
       

    }

    @Override
    public void addTreeModelListener(TreeModelListener listener) {

        if ((listener != null) && !listenerList.contains(listener)) {
            listenerList.add(listener);
        }

    }

    @Override
    public void removeTreeModelListener(TreeModelListener listener) {

        if (listener != null) {
            listenerList.remove(listener);
        }

    }

    public void fireTreeNodesChanged(TreeModelEvent ev) {
        events(ev);
    }

    public void fireTreeNodesInserted(TreeModelEvent ev) {
        events(ev);
    }

    public void fireTreeNodesRemoved(TreeModelEvent ev) {
        events(ev);
    }

    public void fireTreeStructureChanged(TreeModelEvent ev) {
        events(ev);
    }

    private void events(TreeModelEvent ev){

        for(int i = 0; i<listenerList.size(); ++i) {
            TreeModelListener listener =
                    (TreeModelListener) listenerList.get(i);
            listener.treeNodesChanged(ev);
        }

    }

}
