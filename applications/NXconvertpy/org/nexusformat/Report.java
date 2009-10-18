package org.nexusformat;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

class Report {
	Document reduced;
	Document report;
	private static String RED_ROOT = "NXroot";

	Report(final String reduced, final String report)
	            throws ParserConfigurationException, SAXException, IOException {
		this.reduced = parse(reduced);
		this.report = parse(report);
		
		//print(this.report);
		NXSnode node = new NXSnode(getNXroot(this.reduced));
		node.printTree();
	}
	
	static private Node getNXroot(final Document doc) {
		NodeList nodes = doc.getElementsByTagName(RED_ROOT);
		int size = nodes.getLength();
		if (size == 1) {
			return nodes.item(0);
		}
		throw new Error("Failed to find one NXroot (found " + size + ")");
	}
	
	static private Document parse(final String filename)
	            throws ParserConfigurationException, SAXException, IOException {
		File file = new File(filename);
		DocumentBuilder builder
		            = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		return builder.parse(file);
	}
	
	static private void print(final Document doc) {
		NodeList nodes = doc.getChildNodes();
		int size = nodes.getLength();
		for (int i = 0; i < size; i++) {
			print(nodes.item(i));
		}
	}

	static private void print(final Node node) {
		System.out.println(node);
		NodeList nodes = node.getChildNodes();
		int size = nodes.getLength();
		for (int i = 0; i < size; i++) {
			print(nodes.item(i));
		}
	}
}
