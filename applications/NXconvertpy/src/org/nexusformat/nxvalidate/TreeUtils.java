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
 * TextPaneStyle.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import javax.swing.JTree;
import javax.swing.tree.TreePath;
import org.w3c.dom.Document;

/**
 *
 * @author Stephen Rankin
 */
public class TreeUtils {

    public void hideGoodNodes(JTree tree){

        if (tree.isSelectionEmpty()) {
            return;
        }

        NXNodeMapper baseNode = getBaseNode(tree);
        NXNodeMapper node = null;
        
        int rows = tree.getRowCount();

        for (int i = 0; i < rows; ++i) {
            node = (NXNodeMapper) tree.getPathForRow(i).getLastPathComponent();
            System.out.println("Node Name: " + node.toString() + ": " + node.getBadNode());
        }

    }

    public void showGoodNodes(JTree tree){

        if (tree.isSelectionEmpty()) {
            return;
        }

        NXNodeMapper baseNode = getBaseNode(tree);
        NXNodeMapper node = null;

        int rows = tree.getRowCount();

        for (int i = 0; i < rows; ++i) {
            node = (NXNodeMapper) tree.getPathForRow(i).getLastPathComponent();
            System.out.println("Node Name: " + node.toString());
        }

    }

    public String getTreePath(JTree tree) {

        if (tree.isSelectionEmpty()) {
            return null;
        }

        TreePath treePath = tree.getSelectionPath().getParentPath();
        NXNodeMapper node =
                (NXNodeMapper) tree.getSelectionPath().getLastPathComponent();

        String path = "";
        NXNodeMapper tmpNode = null;
        Object[] nodes = treePath.getPath();

        for (int i = 0; i < treePath.getPathCount(); ++i) {
            tmpNode = (NXNodeMapper) nodes[i];
            path = path + "/" + tmpNode.toString();
        }

        path = path + "/" + node.toString();

        return path;

    }

    public NXNodeMapper getBaseNode(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree);
        } else {
            return null;
        }
    }

    public File getNXDLFile(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree).getSchematronFile();
        } else {
            return null;
        }
    }

    public File getReducedFile(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree).getReducedFile();
        } else {
            return null;
        }
    }

    public Document getReducedDoc(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree).getReducedDoc();
        } else {
            return null;
        }
    }

    public Document getResultsDoc(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree).getResultsDoc();
        } else {
            return null;
        }
    }

    public File getResultsFile(JTree tree) {
        if (getNode(tree) != null) {
            return getNode(tree).getResultsFile();
        } else {
            return null;
        }
    }

    public void setNXDLFile(JTree tree, File file) {
        if (getNode(tree) != null) {
            getNode(tree).setSchematronFile(file);
        }
    }

    public void setReducedFile(JTree tree, File file) {
        if (getNode(tree) != null) {
            getNode(tree).setReducedFile(file);
        }
    }

    public void setReducedDoc(JTree tree, Document doc) {
        if (getNode(tree) != null) {
            getNode(tree).setReducedDoc(doc);
        }
    }

    public void setResultsDoc(JTree tree, Document doc) {
        if (getNode(tree) != null) {
            getNode(tree).setResultsDoc(doc);
        }
    }

    public void setResultsFile(JTree tree, File file) {
        if (getNode(tree) != null) {
            getNode(tree).setResultsFile(file);
        }
    }

    private NXNodeMapper getNode(JTree tree) {
        if (tree.isSelectionEmpty()) {
            return null;
        }

        TreePath treePath = tree.getSelectionPath().getParentPath();

        if(treePath==null){
             return null;
        }

        NXNodeMapper tmpNode = null;
        Object[] nodes = treePath.getPath();
        if (nodes.length > 1) {
            tmpNode = (NXNodeMapper) nodes[1];
        } else if (nodes.length == 1) {
            tmpNode =
                    (NXNodeMapper) tree.getSelectionPath().getLastPathComponent();
        } else {
            return null;
        }
        return tmpNode;
    }

}
