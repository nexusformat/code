/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Stephen Rankin
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
 * SVRLNodeFilter.java
 *
 */
package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import org.nexusformat.nxvalidate.exceptions.NXvalidateException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * Implementation of NodeFilterInterface which filters out elements in a NEXUS
 * reduced file if they have failed the validation tests.
 * 
 * @author Stephen Rankin
 */
public class SVRLNodeFilter implements NodeFilterInterface {

    private Document filterDoc = null;
    private Document doc = null;
    private ArrayList<Node> nodes = null;
    private ArrayList<Node> badNodes = null;
    private ArrayList<Node> warnNodes = null;

    public SVRLNodeFilter() {
        nodes = new ArrayList<Node>();
        badNodes = new ArrayList<Node>();
        warnNodes = new ArrayList<Node>();
    }

    /**
     * Sets a DOM document (filter document) that contains a list of nodes
     * that have failed any validation tests, for example, the SVRL file.
     * @param filterDoc a DOM document.
     */
    public void setFilterDocument(Document filterDoc) {

        this.filterDoc = filterDoc;

    }

    /**
     * Sets the document to which to apply the filter. The document in this case
     * is the reduced NeXus file.
     * @param doc a DOM document.
     */
    public void setDocument(Document doc) {

        this.doc = doc;

    }

    /**
     * A list of nodes which are bad.
     * @return the list of bad nodes
     */
    public ArrayList<Node> getBadNodeList() {
        try {

            getXPathList();

        } catch (NXvalidateException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        } catch (XPathExpressionException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        }
        return badNodes;
    }

    /**
     * A list of nodes which have a warning.
     * @return
     */
    public ArrayList<Node> getWarnNodeList(){
        try {

            getXPathList();

        } catch (NXvalidateException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        } catch (XPathExpressionException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        }
        return warnNodes;
    }

    /**
     * Reset the nodes to indicate that they are now good nodes, i.e.
     * resets them back to a pre-validated state.
     */
    public void resetNodes() {
        try {

            resetNodes(0);
            resetNodes(1);

        } catch (NXvalidateException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        } catch (XPathExpressionException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(
                    Level.SEVERE, null, ex);
        }

    }


    private void resetNodes(int type) throws NXvalidateException,
            XPathExpressionException {

        NodeList failed = null;
        String location = null;
        Element element = null;
        XPathFactory factory = XPathFactory.newInstance();
        NodeList nodeList = null;

        if (filterDoc != null) {
            if (filterDoc.hasChildNodes()) {

                //Get the xml elements svrl:failed-assert and
                //svrl:successful-report from the XML
                //validation results file.
                if(type == 0){
                    failed = filterDoc.getElementsByTagName("svrl:failed-assert");
                }
                else if(type == 1){
                    failed = filterDoc.getElementsByTagName("svrl:successful-report");
                }
                if (failed.getLength() > 0) {

                    for (int i = 0; i < failed.getLength(); ++i) {

                        element = (Element) failed.item(i);

                        if (element.hasChildNodes()) {

                            //Each svrl:failed-assert element has child element
                            //location which specifies the XPATH location of the
                            //bad nodes.
                            location = element.getAttribute("location");
                            XPath xpath = factory.newXPath();
                            location = location.replaceAll(
                                    "\\[namespace-uri\\(\\)="
                                    + "'http://definition.nexusformat.org/"
                                    + "schema/3.1'\\]", "");
                            XPathExpression expr = xpath.compile(location);

                            Object result = expr.evaluate(doc,
                                    XPathConstants.NODESET);
                            nodeList = (NodeList) result;

                            //Set the node back to its original state.
                            for (int j = 0; j < nodeList.getLength(); j++) {
                                
                                nodeList.item(j).setUserData("validated", null, null);
                                
                                if(type == 0){
                                    nodeList.item(j).setUserData("bad", null, null);
                                }
                                else if(type == 1){
                                    nodeList.item(j).setUserData("warn", null, null);
                                }

                                nodeList.item(j).setUserData(
                                        "tests", null, null);
                                nodeList.item(j).setUserData(
                                        "texts", null, null);
                                nodeList.item(j).setUserData(
                                        "diags", null, null);
                                nodeList.item(j).setUserData(
                                        "diagatts", null, null);
                            }

                        }

                    }

                }

            }

        }

    }

    private void getReportingNode(int type, NodeList nodeListInput) throws NXvalidateException,
            XPathExpressionException {

        
        String location = null;
        String test = null;
        String text = null;
        String diag = null;
        String diagAtt = null;
        ArrayList<String> tests = null;
        ArrayList<String> texts = null;
        ArrayList<String> diags = null;
        ArrayList<String> diagAtts = null;
        Element element = null;
        XPathFactory factory = XPathFactory.newInstance();
        NodeList nodeList = null;

        //Loop through the nodes and get
        //the test, text diagnostic and diagnostic attribute.
        for (int i = 0; i < nodeListInput.getLength(); ++i) {

            test = null;
            text = null;
            diag = null;
            diagAtt = null;

            element = (Element) nodeListInput.item(i);

            if (element.hasChildNodes()) {

                //Each element has child element
                //location which specifies the XPATH location of the
                //bad nodes.
                location = element.getAttribute("location");
                test = element.getAttribute("test");
                if (element.getElementsByTagName("svrl:text").getLength() > 0) {
                    text = element.getElementsByTagName(
                            "svrl:text").item(0).getTextContent().trim();
                }

                if (element.getElementsByTagName(
                        "svrl:diagnostic-reference").getLength() > 0) {

                    diag = element.getElementsByTagName(
                            "svrl:diagnostic-reference").item(0).getTextContent().trim();

                    diagAtt = ((Element) element.getElementsByTagName(
                            "svrl:diagnostic-reference").item(0)).getAttribute("diagnostic");
                }

                XPath xpath = factory.newXPath();
                location = location.replaceAll(
                        "\\[namespace-uri\\(\\)="
                        + "'http://definition.nexusformat.org/"
                        + "schema/3.1'\\]", "");
                XPathExpression expr = xpath.compile(location);

                Object result = expr.evaluate(doc,
                        XPathConstants.NODESET);
                nodeList = (NodeList) result;

                for (int j = 0; j < nodeList.getLength(); j++) {

                    nodeList.item(j).setUserData("validated", new Boolean(true), null);
                    if(type == 0){
                        nodeList.item(j).setUserData("bad", new Boolean(true), null);
                        badNodes.add(nodeList.item(j));
                    }
                    else if(type == 1){
                        nodeList.item(j).setUserData("warn", new Boolean(true), null);
                        warnNodes.add(nodeList.item(j));
                    }

                    if (nodeList.item(j).getUserData("tests")
                            != null && test != null) {
                        tests = (ArrayList<String>) nodeList.item(j).getUserData(
                                "tests");
                        tests.add(test);
                    } else {
                        tests = new ArrayList<String>();
                        tests.add(test);
                    }

                    if (nodeList.item(j).getUserData("texts")
                            != null && text != null) {
                        texts = (ArrayList<String>) nodeList.item(j).getUserData(
                                "texts");
                        texts.add(text);
                    } else {
                        texts = new ArrayList<String>();
                        texts.add(text);
                    }

                    if (nodeList.item(j).getUserData("diags")
                            != null && diag != null) {
                        diags = (ArrayList<String>) nodeList.item(j).getUserData(
                                "diags");
                        diags.add(diag);
                    } else {
                        diags = new ArrayList<String>();
                        diags.add(diag);
                    }

                    if (nodeList.item(j).getUserData("diagatts")
                            != null && diagAtt != null) {
                        diagAtts = (ArrayList<String>) nodeList.item(j).getUserData(
                                "diagatts");
                        diagAtts.add(diagAtt);
                    } else {
                        diagAtts = new ArrayList<String>();
                        diagAtts.add(diagAtt);
                    }

                    nodeList.item(j).setUserData(
                            "tests", tests, null);
                    nodeList.item(j).setUserData(
                            "texts", texts, null);
                    nodeList.item(j).setUserData(
                            "diags", diags, null);
                    nodeList.item(j).setUserData(
                            "diagatts", diagAtts, null);
                    
                    nodes.add(nodeList.item(j));
                }

            }

        }


    }

    private void getXPathList() throws NXvalidateException, XPathExpressionException {

        NodeList list = null;
        
        if (filterDoc != null) {
            if (filterDoc.hasChildNodes()) {

                //Get the xml elements svrl:failed-assert from the XML
                //validation results file.
                list = filterDoc.getElementsByTagName("svrl:failed-assert");

                if (list.getLength() > 0) {
                    getReportingNode(0, list);
                }

                //Get the xml elements successful-report from the XML
                //validation results file.
                list = filterDoc.getElementsByTagName("svrl:successful-report");

                if (list.getLength() > 0) {
                     getReportingNode(1, list);
                }

            }

        }

    }
}
