/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
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

    public File getNXDCFile(JTree tree) {
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

    public void setNXDCFile(JTree tree, File file) {
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
