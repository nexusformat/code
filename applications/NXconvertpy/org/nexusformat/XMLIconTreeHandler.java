package org.nexusformat;

import javax.swing.tree.DefaultMutableTreeNode;

import org.nexusformat.NXvalidateGuiTree.ITag;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class XMLIconTreeHandler extends DefaultHandler {
	private DefaultMutableTreeNode root, currentNode;

	public DefaultMutableTreeNode getRoot() {
		return root;
	}

	public void startElement(String namespaceURI, String lName, String qName,
			Attributes attrs) throws SAXException {
		String eName = lName;
		if ("".equals(eName))
			eName = qName;
		ITag t = new ITag(eName, attrs);
		DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(t);
		if (currentNode == null) {
			root = newNode;
		} else {
			currentNode.add(newNode);
		}
		currentNode = newNode;
	}

	public void endElement(String namespaceURI, String sName, String qName)
			throws SAXException {
		currentNode = (DefaultMutableTreeNode) currentNode.getParent();
	}

	public void characters(char buf[], int offset, int len) throws SAXException {
		String s = new String(buf, offset, len).trim();
		((ITag) currentNode.getUserObject()).addData(s);
	}
}