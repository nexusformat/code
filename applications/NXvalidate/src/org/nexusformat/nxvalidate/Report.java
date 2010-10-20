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
 * Report.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Vector;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

class Report {

    NXSnode report;
    ArrayList<SVRLitem> errors;
    private static String NXROOT = "NXroot";
    private static String SVRLROOT = "svrl:schematron-output";

    Report(final File reduced, final File reportFile)
            throws ParserConfigurationException, SAXException, IOException {

        // parse the reduced file and turn it into a node tree
        Document redDoc = parse(reduced);
        report = new NXSnode(getNXroot(redDoc));

        // add the svrl report to the reduced
        Document svrlDoc = parse(reportFile);
        this.errors = this.report.addSVRL(getSVRLroot(svrlDoc));
    }

    public int numErrors() {
        return this.errors.size();
    }

    public ArrayList<SVRLitem> getReport() {
        return this.errors;
    }

    public void printReport() {
        int size = this.numErrors();
        for (int i = 0; i < size; i++) {
            System.out.println(this.errors.get(i));
            if (i + 1 < size) {
                System.out.println("----------");
            }
        }
    }

    public NXSnode getTree() {
        return this.report;
    }

    public void printTree() {
        this.report.printTree();
    }

    static private Node getNXroot(final Document doc) {
        NodeList nodes = doc.getElementsByTagName(NXROOT);
        int size = nodes.getLength();
        if (size == 1) {
            return nodes.item(0);
        }
        throw new Error(
                "Failed to find one " + NXROOT + " (found " + size + ")");
    }

    static private Node getSVRLroot(final Document doc) {
        NodeList nodes = doc.getElementsByTagName(SVRLROOT);
        int size = nodes.getLength();
        if (size == 1) {
            return nodes.item(0);
        }
        throw new Error(
                "Failed to find one " + SVRLROOT + " (found " + size + ")");
    }

    static private Document parse(final File file)
            throws ParserConfigurationException, SAXException, IOException {
        
        DocumentBuilder builder =
                DocumentBuilderFactory.newInstance().newDocumentBuilder();
        return builder.parse(file);
    }
}
