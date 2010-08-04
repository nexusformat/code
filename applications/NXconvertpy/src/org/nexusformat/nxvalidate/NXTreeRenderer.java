/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.awt.Component;
import javax.swing.Icon;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;

/**
 *
 * @author steve
 */
public class NXTreeRenderer extends DefaultTreeCellRenderer {

    Icon errorIcon;

    public NXTreeRenderer(Icon icon) {
        errorIcon = icon;
    }

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
        
        if (isErrorNode(value)) {
            setIcon(errorIcon);
            setToolTipText("This Element has an Error.");
        } else {
            setToolTipText(null); //no tool tip
        }

        return this;
    }



    protected boolean isErrorNode(Object value) {
        
        return false;
    }
}
