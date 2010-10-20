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
 * For further information, see <http://www.nexusformat.org/>
 *
 * TreeUtils.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.util.ArrayList;
import java.util.Enumeration;
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

        ArrayList<NXNodeMapper> nodes = new ArrayList<NXNodeMapper>();
        ArrayList<TreePath> paths = new ArrayList<TreePath>();
        NXNodeMapper baseNode = getBaseNode(tree);

        if (baseNode==null) {
            return;
        }

        Enumeration children = baseNode.children();
        NXNodeMapper tmpNode = null;

        NXNodeMapper parent = null;
        TreePath path = null;
        ArrayList<NXNodeMapper> tmpPath = null;
        ArrayList<NXNodeMapper> tmpPath2 = null;
        nodes.add(baseNode);

        while (children.hasMoreElements()) {

            tmpNode = (NXNodeMapper)children.nextElement();
            nodes.add(tmpNode);
            nodes.addAll(getSubNodes(tmpNode));

        }

        for (int i = 0; i < nodes.size(); ++i) {
           
           nodes.get(i).checkBadNode();
           
           if(nodes.get(i).getBadNode()){

               tmpPath = new ArrayList<NXNodeMapper>();
               //tmpPath.add(nodes.get(i));

               parent = (NXNodeMapper)nodes.get(i).getParent();

               while(!parent.toString().equals(baseNode.toString()) && parent!=null){
                   tmpPath.add(parent);
                   parent = (NXNodeMapper)parent.getParent();
               }
               tmpPath.add(baseNode);
               tmpPath.add((NXNodeMapper)baseNode.getParent());
               
               tmpPath2 = new ArrayList<NXNodeMapper>();
               
               int number = tmpPath.size() -1;
               for(int j = 0; j < tmpPath.size();++j){
                   tmpPath2.add(tmpPath.get(number -j));
               }

               paths.add(new TreePath(tmpPath2.toArray()));
               tree.expandPath(new TreePath(tmpPath2.toArray()));
           }
        }

    }

    private ArrayList<NXNodeMapper> getSubNodes(NXNodeMapper node){

        ArrayList<NXNodeMapper> nodes = new ArrayList<NXNodeMapper>();
        NXNodeMapper tmpNode = null;

        Enumeration children = node.children();

        while (children.hasMoreElements()) {
            
            tmpNode = (NXNodeMapper)children.nextElement();
            nodes.add(tmpNode);
            nodes.addAll(getSubNodes(tmpNode));
            
        }

        return nodes;

    }

    public boolean hasBadChildren(NXNodeMapper node){

        ArrayList<NXNodeMapper> subNodes = getSubNodes(node);
        boolean bad = false;
        for(int i = 0;i<subNodes.size();++i){
            if(subNodes.get(i).getBadNode()){
                bad = true;
                break;
            }
        }
        return bad;
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
        if (getRootNode(tree) != null) {
            return getRootNode(tree);
        } else {
            return null;
        }
    }

    public File getNXDLFile(JTree tree) {
        if (getRootNode(tree) != null) {
            return getRootNode(tree).getNXDLFile();
        } else {
            return null;
        }
    }

    public File getReducedFile(JTree tree) {
        if (getRootNode(tree) != null) {
            return getRootNode(tree).getReducedFile();
        } else {
            return null;
        }
    }

    public Document getReducedDoc(JTree tree) {
        if (getRootNode(tree) != null) {
            return getRootNode(tree).getReducedDoc();
        } else {
            return null;
        }
    }

    public Document getResultsDoc(JTree tree) {
        if (getRootNode(tree) != null) {
            return getRootNode(tree).getResultsDoc();
        } else {
            return null;
        }
    }

    public File getResultsFile(JTree tree) {
        if (getRootNode(tree) != null) {
            return getRootNode(tree).getResultsFile();
        } else {
            return null;
        }
    }

    public void setNXDLFile(JTree tree, File file) {
        if (getRootNode(tree) != null) {
            getRootNode(tree).setNXDLFile(file);
        }
    }

    public void setReducedFile(JTree tree, File file) {
        if (getRootNode(tree) != null) {
            getRootNode(tree).setReducedFile(file);
        }
    }

    public void setReducedDoc(JTree tree, Document doc) {
        if (getRootNode(tree) != null) {
            getRootNode(tree).setReducedDoc(doc);
        }
    }

    public void setResultsDoc(JTree tree, Document doc) {
        if (getRootNode(tree) != null) {
            getRootNode(tree).setResultsDoc(doc);
        }
    }

    public void setResultsFile(JTree tree, File file) {
        if (getRootNode(tree) != null) {
            getRootNode(tree).setResultsFile(file);
        }
    }

    public void setValidated(JTree tree, File file) {
        NXNodeMapper tmpNode = getRootNode(tree);
        if (tmpNode != null) {
            if(tmpNode.getResultsFile().equals(file)){
                tmpNode.setValidatedNode(true);
            }
        }
    }
    
    private NXNodeMapper getRootNode(JTree tree) {
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
