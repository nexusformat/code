package org.nexusformat;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

class NXSnode {
	private static final String PADDING = "  ";
	private static final String SDS = "SDS";
	private static final Logger LOG = Logger.getInstance();

	private String name;
	private String nxclass;
	private Map<String, String> attrs;
	private Vector<NXSnode> children;
	private Vector<SVRLitem> svrl;

	NXSnode(final Node node) {
		this.nxclass  = node.getNodeName();
		this.attrs = new HashMap<String, String>();
		this.setAttrs(node.getAttributes());

		NodeList nodes = node.getChildNodes();
		int size = nodes.getLength();
		this.children = new Vector<NXSnode>();
		for (int i = 0; i < size; i++) {
			this.addChild(nodes.item(i));
		}
		this.svrl = new Vector<SVRLitem>();
	}

	void addSVRL(final Node svrl) {
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
		} else if (name.equals("NAPItype")) {
			this.name = this.nxclass;
			this.nxclass = SDS;
		} else if (name.equals("target")) {
			this.name = this.nxclass;
			this.nxclass = null;
			this.attrs.put(name, value);
		} else {
			this.attrs.put(name, value);
		}
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
		String result = this.name + ":" + this.nxclass;
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
}
