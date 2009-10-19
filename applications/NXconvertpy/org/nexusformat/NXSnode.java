package org.nexusformat;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

class NXSnode implements TreeModel {
	private static final String PADDING = "  ";
	private static final String NXROOT = "NXroot";
	private static final String TYPE = "NAPItype";
	private static final String LINK = "NAPIlink";
	private static final String SDS = "SDS";
	private static final Logger LOG = Logger.getInstance();

	private String name;
	private String nxclass;
	private Map<String, String> attrs;
	private Vector<NXSnode> children;
	private Vector<SVRLitem> svrl;

	NXSnode() {
		this.name = NXROOT;
		this.nxclass = "";
		this.attrs = new HashMap<String, String>();
		this.children = new Vector<NXSnode>();
		this.svrl = new Vector<SVRLitem>();
	}

	NXSnode(final Node node) {
		this();
		this.nxclass  = node.getNodeName();
		this.setAttrs(node.getAttributes());

		NodeList nodes = node.getChildNodes();
		int size = nodes.getLength();
		for (int i = 0; i < size; i++) {
			this.addChild(nodes.item(i));
		}
	}

	Vector<SVRLitem> addSVRL(final Node svrl) {
		// generate a list of xml error nodes
		Vector<Node> errors = new Vector<Node>();
		addSVRL(svrl, errors);

		// convert it to java classes
		Vector<SVRLitem> items = new Vector<SVRLitem>();
		for (Node node: errors) {
			items.add(new SVRLitem(node));
		}

		// find the appropriate nodes to attach it to
		NXSnode nxsnode;
		for (SVRLitem item: items) {
			nxsnode = getNode(this, item.getLocationArray(), 1);
			nxsnode.svrl.add(item);
		}

		return items;
	}

	private static NXSnode getNode(NXSnode parent, Vector<String> location,
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

		// get the options
		Vector<NXSnode> choices = new Vector<NXSnode>();
		for (NXSnode child: parent.children) {
			if (equals(child.getType(), name)) {
				choices.add(child);
			} else if (equals(child.getName(), name)) {
				choices.add(child);
			}
		}

		// pick which one to return
		int numChoice = choices.size();
		LOG.debug("Found " + numChoice + " options");
		if ((numChoice <= 0) || (numChoice < index)){
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

	private static void addSVRL(final Node node, final Vector<Node> errors) {
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
			this.name = value;
			return;
		} else if (name.equals(TYPE)) {
			this.name = this.nxclass;
			this.nxclass = SDS;
		} else if (name.equals("target")) {
			this.name = this.nxclass;
			this.nxclass = null;
		}
		this.attrs.put(name, value);
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

	String getName() {
		return this.name;
	}

	String getType() {
		return this.nxclass;
	}

	boolean hasError() {
		return (this.svrl.size() > 0);
	}

	public String toString() {
		String result;
		if (NXROOT.equals(this.name) || NXROOT.equals(this.nxclass)) {
			result = NXROOT;
		} else if (LINK.equals(this.name)) {
			result = this.name + ":target=" + this.attrs.get("target");
		} else if (SDS.equals(this.nxclass)) {
			result = this.name + ":" + TYPE + "="+ this.attrs.get(TYPE);
		} else {
			result = this.name + ":" + this.nxclass;
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
		for (NXSnode node: this.children) {
			node.printTree(padding);
		}
	}

	public boolean equals(final Object other) {
		// do the simple checks
		if (this == other)
			return true;
		if (other == null)
			return false;
		if (!(other instanceof NXSnode))
			return false;

		// cast and do deep comparison
		NXSnode temp = (NXSnode) other;
		if (!this.name.equals(temp.name))
			return false;
		if (!this.nxclass.equals(temp.nxclass))
			return false;
		if (!this.attrs.equals(temp.attrs))
			return false;
		if (!this.children.equals(temp.children))
			return false;
		if (!this.svrl.equals(temp.svrl))
			return false;

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
		NXSnode temp = toNXSnode(parent);
		return temp.children.get(index);
	}

	public int getChildCount(Object parent) {
		NXSnode temp = toNXSnode(parent);
		return temp.children.size();
	}

	public int getIndexOfChild(Object parent, Object child) {
		if (parent == null)
			return -1;
		if (child == null)
			return -1;
		NXSnode myParent = toNXSnode(parent);
		NXSnode myChild = toNXSnode(child);

		return myParent.children.indexOf(myChild);
	}

	public Object getRoot() {
		return this;
	}

	public boolean isLeaf(Object node) {
		NXSnode temp = toNXSnode(node);
		return (temp.children.size() <= 0);
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
