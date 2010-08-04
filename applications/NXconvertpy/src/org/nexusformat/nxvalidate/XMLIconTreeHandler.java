package org.nexusformat.nxvalidate;

import java.util.ResourceBundle;
import javax.swing.tree.DefaultMutableTreeNode;

import org.nexusformat.nxvalidate.NXvalidateGuiTree.ITag;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class XMLIconTreeHandler extends DefaultHandler {

    private DefaultMutableTreeNode root, currentNode;
    private ResourceBundle bundle = null;

    public DefaultMutableTreeNode getRoot() {
        return root;
    }

    @Override
    public void startElement(String namespaceURI, String lName, String qName,
            Attributes attrs) throws SAXException {
        String eName = lName;
        if ("".equals(eName)) {
            eName = qName;
        }
        ITag t = new ITag(eName, attrs);
        DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(t);
        if (currentNode == null) {
            root = newNode;
        } else {
            currentNode.add(newNode);
        }
        currentNode = newNode;
    }

    @Override
    public void endElement(String namespaceURI, String sName, String qName)
            throws SAXException {
        currentNode = (DefaultMutableTreeNode) currentNode.getParent();
    }

    @Override
    public void characters(char buf[], int offset, int len) throws SAXException {
        String s = new String(buf, offset, len).trim();
        ((ITag) currentNode.getUserObject()).addData(s);
    }
}
