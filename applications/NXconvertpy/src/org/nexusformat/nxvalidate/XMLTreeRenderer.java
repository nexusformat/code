/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.awt.Component;
import java.util.ResourceBundle;
import javax.swing.ImageIcon;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeCellRenderer;

/**
 * We need to modify the tree representing the reduced document when
 * nodes are found to be bad (validation fails on them). the bad nodes
 * are marked with a red cross icon and a tool tip is added to the node.
 * 
 * @author Stephen Rankin
 */
public class XMLTreeRenderer extends DefaultTreeCellRenderer {

    private String path = "";
    private ResourceBundle bundle = null;

    public XMLTreeRenderer(String path) {
        this.path = path;
        bundle = ResourceBundle.getBundle(
                "org/nexusformat/nxvalidate/resources/nxvalidate");
    }

    @Override
    public Component getTreeCellRendererComponent(
            JTree tree,
            Object value,
            boolean sel,
            boolean expanded,
            boolean leaf,
            int row,
            boolean hasFocus) {

        super.getTreeCellRendererComponent(
                tree, value, sel,
                expanded, leaf, row,
                hasFocus);

        //Add the bad node error icon and tool tip text.
        NXNodeMapper node = (NXNodeMapper) value;
        node.checkBadNode();
        if (node.getBadNode()) {
            setIcon(createImageIcon(path, "errorIcon"));
            setToolTipText(bundle.getString("elementError"));
        } else {
            setToolTipText(null); //no tool tip
        }

        return this;
    }

    /** Returns an ImageIcon, or null if the path was invalid. */
    protected ImageIcon createImageIcon(String path,
            String description) {
        java.net.URL imgURL = getClass().getResource(path);
        if (imgURL != null) {
            return new ImageIcon(imgURL, description);
        } else {
            System.err.println("Couldn't find file: " + path);
            return null;
        }
    }
}
