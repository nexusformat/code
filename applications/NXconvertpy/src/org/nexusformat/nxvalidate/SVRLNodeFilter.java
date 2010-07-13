/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
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
import org.nexusformat.nxvalidate.exceptions.NXConvertpyException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 *
 * @author ser65
 */
public class SVRLNodeFilter implements NodeFilterInterface {

    private Document filterDoc = null;
    private Document doc = null;
    private ArrayList<Node> nodes = null;

    /**
     * Sets a DOM document (filter document) that containes a list of nodes
     * that have failed any validation tests, for example, the SVRL file.
     * @param filterDoc a DOM document.
     */
    public void setFilterDocument(Document filterDoc){

        this.filterDoc = filterDoc;

    }

    /**
     * Sets the document to which to apply the filter.
     * @param doc a DOM document.
     */
    public void setDocument(Document doc){

        this.doc = doc;

    }

    /**
     * A list of nodes which are bad.
     * @return
     */
    public ArrayList<Node> getBadNodeList(){
        try {
            getXPathList();

        } catch (NXConvertpyException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(Level.SEVERE, null, ex);
        } catch (XPathExpressionException ex) {
            Logger.getLogger(SVRLNodeFilter.class.getName()).log(Level.SEVERE, null, ex);
        }
        return nodes;
    }

    private void getXPathList() throws NXConvertpyException, XPathExpressionException {

        NodeList failed = null;
        NodeList locations = null;
        Element element = null;
        XPathFactory factory = XPathFactory.newInstance();

        if(filterDoc!=null){
            if(filterDoc.hasChildNodes()){

                failed = filterDoc.getElementsByTagName("failed-assert");

                if(failed.getLength()>0){

                    for(int i = 0; i<failed.getLength(); ++i){

                        element = (Element)failed.item(i);

                        if(element.hasChildNodes()){

                            locations = element.getElementsByTagName("location");
                            if(locations.getLength()>1){
                                throw new NXConvertpyException(
                                        "Too many location elemenets in" +
                                        "failed-assert tag: " +
                                        locations.getLength());
                            }
                            else if (locations.getLength()==0){
                                throw new NXConvertpyException(
                                        "No location elemenets in" +
                                        "failed-assert tag.");
                            }
                            else{
                                XPath xpath = factory.newXPath();
                                XPathExpression expr = xpath.compile(
                                        locations.item(0).getTextContent());
                                Object result = expr.evaluate(doc, XPathConstants.NODESET);

                                NodeList nodes = (NodeList) result;
                                for (int j = 0; j < nodes.getLength(); j++) {
                                    System.out.println(nodes.item(j).getNodeValue());
                                }


                            }

                        }

                    }

                }

            }
        }
        
    }

}
