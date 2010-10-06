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
 * XMLTreeRenderer.java
 *
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
        boolean badKids = node.checkBadChildren();
        if (badKids) {
            setIcon(createImageIcon("resources/dialog-warning.png", "warningIcon"));
        }
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
