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

	private String name;
	private String nxclass;
	private Map<String, String> attrs;
	private Vector<NXSnode> children;

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
			this.nxclass = SDS;
		}
		else if (name.equals("NAPItype")) {
			this.name = this.nxclass;
			this.nxclass = SDS;
		}
		else {
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

	public String toString() {
		return this.name + ":" + this.nxclass;
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
