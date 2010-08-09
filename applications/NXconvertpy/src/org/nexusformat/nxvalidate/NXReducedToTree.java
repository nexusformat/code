/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Vector;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreePath;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import org.nexusformat.nxvalidate.exceptions.NXConvertpyException;
import org.w3c.dom.Document;

/**
 * This maps the NX Reduced XML Document (as a DOM document) to
 * a JTree model.
 * @author ser65
 */
public class NXReducedToTree extends DefaultTreeModel {

    private Vector listenerList = new Vector();
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

    // Basic TreeModel operations
    public Object getRoot() {

        return root;
        
    }

    public boolean isLeaf(Object aNode) {

        // Determines whether the icon shows up to the left.
        // Return true for any node with no children
        NXNodeMapper node = (NXNodeMapper) aNode;

        if (node.getChildCount() > 0) {
            return false;
        }

        return true;
    }

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
            listenerList.addElement(listener);
        }

    }

    @Override
    public void removeTreeModelListener(TreeModelListener listener) {

        if (listener != null) {
            listenerList.removeElement(listener);
        }

    }

    // Note: Since XML works with 1.1, this example uses Vector.
    // If coding for 1.2 or later, though, I'd use this instead:
    //   private List listenerList = new LinkedList();
    // The operations on the List are then add(), remove() and
    // iteration, via:
    //  Iterator it = listenerList.iterator();
    //  while ( it.hasNext() ) {
    //    TreeModelListener listener = (TreeModelListener) it.next();
    //    ...
    //  }
    public void fireTreeNodesChanged(TreeModelEvent e) {

        Enumeration listeners = listenerList.elements();

        while (listeners.hasMoreElements()) {
            TreeModelListener listener =
                    (TreeModelListener) listeners.nextElement();
            listener.treeNodesChanged(e);
        }
        

    }

    public void fireTreeNodesInserted(TreeModelEvent e) {

        Enumeration listeners = listenerList.elements();

        while (listeners.hasMoreElements()) {
            TreeModelListener listener =
                    (TreeModelListener) listeners.nextElement();
            listener.treeNodesInserted(e);
        }
        

    }

    public void fireTreeNodesRemoved(TreeModelEvent e) {

        Enumeration listeners = listenerList.elements();

        while (listeners.hasMoreElements()) {
            TreeModelListener listener =
                    (TreeModelListener) listeners.nextElement();
            listener.treeNodesRemoved(e);
        }
        

    }

    public void fireTreeStructureChanged(TreeModelEvent e) {

        Enumeration listeners = listenerList.elements();

        while (listeners.hasMoreElements()) {
            TreeModelListener listener =
                    (TreeModelListener) listeners.nextElement();
            listener.treeStructureChanged(e);
        }
        
        
    }
}
