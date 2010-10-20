/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Nexus Team
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
 * NXSnode.java
 *
 */
package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import java.util.Vector;

import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

class NXSnode extends AbstractNXSnode implements TreeModel {

    private static final String PADDING = "  ";
    private static final String NXROOT = "NXroot";
    private static final String TYPE = "NAPItype";
    private static final String LINK = "NAPIlink";
    private static final String SDS = "SDS";
    private static final Logger LOG = Logger.getInstance();
    private String name;
    private String nxclass;
    private ArrayList<AbstractNXSnode> children;
    private ArrayList<SVRLitem> svrl;

    NXSnode() {
        super(NXROOT);
        this.nxclass = "";
        this.children = new ArrayList<AbstractNXSnode>();
        this.svrl = new ArrayList<SVRLitem>();
    }

    NXSnode(final Node node) {
        this();
        this.nxclass = node.getNodeName();
        this.setAttrs(node.getAttributes());

        NodeList nodes = node.getChildNodes();
        int size = nodes.getLength();
        for (int i = 0; i < size; i++) {
            this.addChild(nodes.item(i));
        }
    }

    ArrayList<SVRLitem> addSVRL(final Node svrl) {
        // generate a list of xml error nodes
        ArrayList<Node> errors = new ArrayList<Node>();
        addSVRL(svrl, errors);

        // convert it to java classes
        ArrayList<SVRLitem> items = new ArrayList<SVRLitem>();
        for (Node node : errors) {
            items.add(new SVRLitem(node));
        }

        // find the appropriate nodes to attach it to
        NXSnode nxsnode;
        for (SVRLitem item : items) {
            nxsnode = getNode(this, item.getLocationArray(), 1);
            nxsnode.svrl.add(item);
        }

        return items;
    }

    private static NXSnode getNode(NXSnode parent, ArrayList<String> location,
            int depth) {
        // chop up this part of the path to be useful
        if (location.size() <= depth) {
            return parent;
        }
        String partPath = location.get(depth);
        int left = partPath.indexOf("[");
        int right = partPath.indexOf("]", left);
        String name = partPath.substring(0, left);
        int index = Integer.parseInt(partPath.substring(left + 1, right)) - 1;
        LOG.debug("Looking for " + name + "[" + index + "]");

        // get the options - only works with NXSnodes
        ArrayList<NXSnode> choices = new ArrayList<NXSnode>();
        NXSnode temp;
        for (AbstractNXSnode child : parent.children) {
            if (child instanceof NXSnode) {
                temp = (NXSnode) child;
                if (equals(temp.getType(), name)) {
                    choices.add(temp);
                } else if (equals(temp.getName(), name)) {
                    choices.add(temp);
                }
            }
        }

        // pick which one to return
        int numChoice = choices.size();
        LOG.debug("Found " + numChoice + " options");
        if ((numChoice <= 0) || (numChoice < index)) {
            return parent;
        }
        if (depth >= location.size()) {
            return choices.get(index);
        } else {
            return getNode(choices.get(index), location, depth + 1);
        }
    }

    private static boolean equals(final String left, final String right) {
        if (left == null) {
            return false;
        }
        if (right == null) {
            return false;
        }
        return left.equals(right);
    }

    private static void addSVRL(final Node node, final ArrayList<Node> errors) {
        if (SVRLitem.hasLocation(node)) {
            errors.add(node);
            return;
        }
        NodeList nodes = node.getChildNodes();
        int size = nodes.getLength();
        for (int i = 0; i < size; i++) {
            addSVRL(nodes.item(i), errors);
        }
    }

    private void setAttrs(final NamedNodeMap attrs) {
        if (attrs == null) {
            return;
        }
        int size = attrs.getLength();
        Node attr;
        for (int i = 0; i < size; i++) {
            attr = attrs.item(i);
            this.setAttr(attr.getNodeName(), attr.getNodeValue());
        }
    }

    void setAttr(final String name, final String value) {
        if (name.equals("name")) {
            this.setName(value);
            return;
        } else if (name.equals(TYPE)) {
            this.setName(this.nxclass);
            this.nxclass = SDS;
            return;
        }
        this.children.add(0, new Attribute(name, value));
    }

    private void addChild(final Node node) {
        if (node == null) {
            return;
        }
        int type = node.getNodeType();
        if (type != Node.ELEMENT_NODE) {
            return;
        }
        this.children.add(new NXSnode(node));
    }

    String getType() {
        return this.nxclass;
    }

    boolean hasError() {
        return (this.svrl.size() > 0);
    }

    private String getAttrValue(final String name) {
        int size = this.children.size();
        AbstractNXSnode node;
        for (int i = 0; i < size; i++) {
            node = this.children.get(i);
            if (node instanceof NXSnode) {
                return "";
            } else if (this.children.get(i).getName().equals(name)) {
                return ((Attribute) node).getValue();
            }
        }
        return "";
    }

    public String toString() {
        String result;
        if (NXROOT.equals(this.getName()) || NXROOT.equals(this.nxclass)) {
            result = NXROOT;
        } else if (LINK.equals(this.getName())) {
            result = this.getName() + ":target=" + this.getAttrValue("target");
        } else if (SDS.equals(this.nxclass)) {
            result = this.getName() + ":" + TYPE + "=" + this.getAttrValue(TYPE);
        } else {
            result = this.getName() + ":" + this.nxclass;
        }
        if (this.hasError()) {
            return result + "*";
        } else {
            return result;
        }
    }

    public void printTree() {
        this.printTree("");
    }

    private void printTree(String padding) {
        System.out.println(padding + this.toString());
        padding += PADDING;
        for (AbstractNXSnode node : this.children) {
            if (node instanceof NXSnode) // don't bother with attributes
            {
                ((NXSnode) node).printTree(padding);
            }
        }
    }

    public boolean equals(final Object other) {
        // do the simple checks
        if (this == other) {
            return true;
        }
        if (other == null) {
            return false;
        }
        if (!(other instanceof NXSnode)) {
            return false;
        }

        // cast and do deep comparison
        NXSnode temp = (NXSnode) other;
        if (!this.getName().equals(temp.getName())) {
            return false;
        }
        if (!this.nxclass.equals(temp.nxclass)) {
            return false;
        }
        if (!this.children.equals(temp.children)) {
            return false;
        }
        if (!this.svrl.equals(temp.svrl)) {
            return false;
        }

        // this far must be ok
        return true;
    }

    private static NXSnode toNXSnode(final Object object) {
        if (object == null) {
            throw new Error("Cannot convert null object to NXSnode");
        }
        if (!(object instanceof NXSnode)) {
            throw new Error("Cannot convert \"" + object + "\"  to NXSnode");
        }
        return (NXSnode) object;
    }

    // ---------------- TreeModel requirements
    @Override
    public void addTreeModelListener(TreeModelListener l) {
        // TODO Auto-generated method stub
    }

    public Object getChild(Object parent, int index) {
        if (parent instanceof NXSnode) {
            return ((NXSnode) parent).children.get(index);
        } else {
            return null;
        }
    }

    public int getChildCount(Object parent) {
        NXSnode temp = toNXSnode(parent);
        return temp.children.size();
    }

    public int getIndexOfChild(Object parent, Object child) {
        if (parent == null) {
            return -1;
        }
        if (child == null) {
            return -1;
        }
        NXSnode myParent = toNXSnode(parent);
        if (child instanceof AbstractNXSnode) {
            return myParent.children.indexOf((AbstractNXSnode) child);
        } else {
            return -1;
        }
    }

    public Object getRoot() {
        return this;
    }

    public boolean isLeaf(final Object node) {
        if (node instanceof NXSnode) {
            return (((NXSnode) node).children.size() <= 0);
        } else {
            return true;
        }
    }

    @Override
    public void removeTreeModelListener(TreeModelListener l) {
        // TODO Auto-generated method stub
    }

    @Override
    public void valueForPathChanged(TreePath path, Object newValue) {
        // TODO Auto-generated method stub
    }
}
