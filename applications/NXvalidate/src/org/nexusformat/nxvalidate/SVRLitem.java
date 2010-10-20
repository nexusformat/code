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
 * SVRLitem.java
 *
 */
package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import java.util.Vector;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class SVRLitem {
	private static String NS_START = "[namespace-uri";
	private static String NS_STOP = "']";
	
	private String[] location;
	private String xpath_str;
	private String type;
	private String test;
	private String message;

	public SVRLitem(final Node node) {
		this.setLocation(node);
		this.setType(node.getNodeName());
		this.setAttrs(node.getAttributes());
		this.setMessage(node);
	}

	public String toString() {
		StringBuilder buffer = new StringBuilder();
		buffer.append(this.test + " failed at " + this.getLocation());
		if (this.message.length() > 0) {
			buffer.append("\n");
			buffer.append(this.message);
		}
		return buffer.toString();
	}

	ArrayList<String> getLocationArray() {
		ArrayList<String> result = new ArrayList<String>();
		for (int i = 0; i < this.location.length; i++) {
			result.add(this.location[i]);
		}
		return result;
	}

	public String getLocation() {
		StringBuilder buffer = new StringBuilder();
		for (String item: this.location) {
			buffer.append("/");
			buffer.append(item);
		}
		return buffer.toString();
	}

	public String getType() {
		return this.type;
	}

	public String getTest() {
		return this.test;
	}
	
	public String getMessage() {
		return this.message;
	}

	private void setType(final String type) {
		if (type.equals("svrl:failed-assert")) {
			this.type = "failed assert";
		} else {
			throw new Error("Do not understand type " + type);
		}
	}

	private void setAttrs(final NamedNodeMap attrs) {
		Node attr;
		// get the test name
		attr = attrs.getNamedItem("test");
		if (attr != null) {
			this.test = attr.getNodeValue();
		}
	}
	private void setMessage(final Node node) {
		if (node.getNodeName().equals("svrl:text")) {
			NodeList nodes = node.getChildNodes();
			int size = nodes.getLength();
			if (size == 0) {
				this.message = "";
			} else if(size == 1) {
				this.message = nodes.item(0).getNodeValue();
				this.message = this.message.trim();
			} else {
				throw new Error("Expected only 1 node, found "
						+ nodes.getLength());
			}
		} else {
			NodeList nodes = node.getChildNodes();
			int size = nodes.getLength();
			Node temp;
			for (int i = 0; i < size; i++) {
				temp = nodes.item(i);
				if (temp.getNodeName().equals("svrl:text")) {
					this.setMessage(temp);
					return;
				}
			}
		}
	}

	private void setLocation(final Node node) {
		// deal with the root splitting
		this.xpath_str = getLocation(node);
		String temp = this.xpath_str.substring(3);
		this.location = temp.split("/\\*:");

		// trim out the namespace junk
		for (int i = 0; i < this.location.length; i++) {
			this.location[i] = removeNS(this.location[i]);
		}
	}
	static private String removeNS(final String orig) {
		int left = orig.indexOf(NS_START);
		int right = orig.indexOf(NS_STOP, left + 1);

		return orig.substring(0, left)
		       + orig.substring(right + NS_STOP.length());
	}

	public static boolean hasLocation(final Node candidate) {
		if (candidate.getNodeType() != Node.ELEMENT_NODE) {
			return false;
		}
		String location = getLocation(candidate);
		if (location == null) {
			return false;
		}
		if (location.length() <= 0) {
			return false;
		}
		return true;
	}

	public static String getLocation(final Node node) {
		NamedNodeMap attrs = node.getAttributes();
		Node attr = attrs.getNamedItem("location");
		if (attr == null) {
			return "";
		}
		else {
			return attr.getNodeValue();
		}
	}

}
