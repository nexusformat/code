package org.nexusformat;

import java.util.Vector;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

public class SVRLitem {
	private static String NS_START = "[namespace-uri";
	private static String NS_STOP = "']";
	
	private String[] location;
	private String xpath_str;

	public SVRLitem(final Node node) {
		this.setLocation(node);
		System.out.println(this.getLocation());
	}

	Vector<String> getLocationArray() {
		Vector<String> result = new Vector<String>();
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
