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
 * XMLTreeRenderer.java
 *
 */
package org.nexusformat.nxvalidate;

import java.awt.Component;
import java.util.ResourceBundle;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JTree;
import javax.swing.plaf.basic.BasicTreeUI;
import javax.swing.tree.DefaultTreeCellRenderer;

/**
 * We need to modify the tree representing the reduced document when
 * nodes are found to be bad (validation fails on them). the bad nodes
 * are marked with a red cross icon and a tool tip is added to the node.
 * 
 * @author Stephen Rankin
 */
public class XMLTreeRenderer extends DefaultTreeCellRenderer {

    private String badPath = "";
    private String goodPath = "";
    private String warnPath = "";
    private String rootPath = "";
    private String newPath = "";
    private String infoPath = "";
    private String infoGoodPath = "";
    private String infoBadPath = "";
    private String infoWarnPath = "";

    private ResourceBundle bundle = null;
    private JTree tree = null;
    /**
     * Setup a tree renderer with custom icons.
     * @param path the relative path to the image icon.
     */
    public XMLTreeRenderer(String rootPath, String badPath,
                           String goodPath, String warnPath,
                           String newPath, String infoPath,
                           String infoGoodPath, String infoBadPath,
                           String infoWarnPath) {
        this.badPath = badPath;
        this.goodPath = goodPath;
        this.warnPath = warnPath;
        this.rootPath = rootPath;
        this.newPath = newPath;
        this.infoPath = infoPath;
        this.infoGoodPath = infoGoodPath;
        this.infoBadPath = infoBadPath;
        this.infoWarnPath = infoWarnPath;
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

        boolean validated = false;
        this.tree = tree;

        //Add the bad node error icon and tool tip text.
        NXNodeMapper node = (NXNodeMapper) value;

        if(node.getRoot()!=null){
            validated = node.getRoot().getValidatedNode();
        }
        else{
            validated = node.getValidatedNode();
        }
        
        if(node.hasBadChildren()){

        }

        /*boolean badKids = node.checkBadChildren();
        if (badKids) {
            setIcon(createImageIcon("resources/dialog-warning.png", "warningIcon"));
        }*/
        if (node.getBadNode() && node.hasBadChildren()) {
            setIcon(createImageIcon(infoBadPath, "errorIcon"));
        }
        else if(node.getBadNode() && !node.hasBadChildren()) {
            setIcon(createImageIcon(badPath, "errorIcon"));
        }
        else if(node.getWarnNode() && node.hasBadChildren()){
            setIcon(createImageIcon(infoWarnPath, "warnIcon"));
        }
        else if(node.getWarnNode() && !node.hasBadChildren()){
            setIcon(createImageIcon(warnPath, "warnIcon"));
        }
        else if (node.isRoot()){
            setIcon(createImageIcon(rootPath, "rootIcon"));
        }
        else if(!validated){
            setIcon(createImageIcon(newPath, "newIcon"));
        }
        else if(node.isDocument() && node.hasBadChildren()){
            setIcon(createImageIcon(infoPath, "infoIcon"));
        }
        else{
            if(node.hasBadChildren()) {
                setIcon(createImageIcon(infoGoodPath, "goodIcon"));
            }
            else{
                setIcon(createImageIcon(goodPath, "goodIcon"));
            }
        }
        
        return this;
    }

    /**
     * sets the icon for the handle of an expanded node.
     *
     * Note: this will only succeed if the current ui delegate is
     * a BasicTreeUI otherwise it will do nothing.
     *
     * @param expanded
     */
    public void setExpandedIcon(Icon expanded) {
        if (tree.getUI() instanceof BasicTreeUI) {
            ((BasicTreeUI) tree.getUI()).setExpandedIcon(expanded);
        }
    }

    /**
     * sets the icon for the handel of a collapsed node.
     *
     * Note: this will only succeed if the current ui delegate is
     * a BasicTreeUI otherwise it will do nothing.
     *
     * @param collapsed
     */
    public void setCollapsedIcon(Icon collapsed) {
        if (tree.getUI() instanceof BasicTreeUI) {
            ((BasicTreeUI) tree.getUI()).setCollapsedIcon(collapsed);
        }
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
