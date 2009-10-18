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
	NXSnode report;

	private static String NXROOT = "NXroot";
	private static String SVRLROOT = "svrl:schematron-output";

	Report(final String reduced, final String report)
	            throws ParserConfigurationException, SAXException, IOException {
		// parse the reduced file and turn it into a node tree
		Document redDoc = parse(reduced);
		this.report = new NXSnode(getNXroot(redDoc));

		// add the svrl report to the reduced
		Document svrlDoc = parse(report);
		this.report.addSVRL(getSVRLroot(svrlDoc));

		this.report.printTree();
	}
	
	static private Node getNXroot(final Document doc) {
		NodeList nodes = doc.getElementsByTagName(NXROOT);
		int size = nodes.getLength();
		if (size == 1) {
			return nodes.item(0);
		}
		throw new Error("Failed to find one " + NXROOT + " (found " + size
				        + ")");
	}

	static private Node getSVRLroot(final Document doc) {
		NodeList nodes = doc.getElementsByTagName(SVRLROOT);
		int size = nodes.getLength();
		if (size == 1) {
			return nodes.item(0);
		}
		throw new Error("Failed to find one " + SVRLROOT + " (found " + size
				        + ")");
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
