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
 * XMLIconTreeHandler.java
 *
 */

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
