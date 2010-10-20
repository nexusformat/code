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
 * NXNodeMapper.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.NoSuchElementException;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreeNode;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 *
 * @author Stephen Rankin
 */
public class NXNodeMapper implements MutableTreeNode {

    private Node domNode = null;
    private String nodeName = null;
    private boolean isRoot = false;
    private boolean isLeaf = false;
    private boolean isDocument = false;
    private boolean allowsChildren = false;
    private ArrayList<NXNodeMapper> documents = null;
    static final int ELEMENT_TYPE = Node.ELEMENT_NODE;
    private int childCount = 0;
    private ArrayList<Node> children = null;
    private NXNodeMapper root = null;
    private File nxsFile = null;
    private File nxdlFile = null;
    private File reducedFile = null;
    private File resultsFile = null;
    private Document reducedDoc = null;
    private Document resultsDoc = null;
    private boolean badNode = false;
    private boolean warnNode = false;
    private boolean validatedNode = false;
    private Object userObject = null;

    // Construct an Adapter node from a DOM node
    public NXNodeMapper(Node node, boolean isDocument, File nxsFile) {

        this.isDocument = isDocument;
        domNode = node;

        allowsChildren = true;
        if (node.getNodeType() != ELEMENT_TYPE) {
            isLeaf = true;
        }

        this.nxsFile = nxsFile;
        nodeName = nxsFile.getAbsolutePath();
        children = getElements();
    }

    // Construct an Adapter node from a DOM node
    public NXNodeMapper(Node node, boolean isDocument, String nodeName) {

        this.isDocument = isDocument;
        domNode = node;

        allowsChildren = true;
        if (node.getNodeType() != ELEMENT_TYPE) {
            isLeaf = true;
        }

        this.nodeName = nodeName;
        children = getElements();
    }

    public NXNodeMapper(String nodeName) {
        this.nodeName = nodeName;
        isRoot = true;
        allowsChildren = true;
        documents = new ArrayList<NXNodeMapper>();
    }

    /**
     * Check to see if this node is the root node.
     * @return true is it is the root node, false otherwise.
     */
    public boolean isRoot() {
        return isRoot;
    }

    /**
     * Check to see if this node is a document, i.e the list of nodes directly
     * under the root node that are the open NXS documents.
     *
     * @return true if the node is a document node.
     */
    public boolean isDocument() {
        return isDocument;
    }

    /**
     * Set the flag that says that this node is a documents node i.e. one of the
     * list of nodes directly under the root node that are the open NXS documents.
     * @param isDocument a flag which is true if the node is a document node.
     */
    public void setDocument(boolean isDocument) {
        this.isDocument = isDocument;
    }

    /**
     * Set the root node that this node belongs to.
     * @param root the root node.
     */
    public void setRoot(NXNodeMapper root) {
        this.root = root;
    }

    /**
     * Get the NXS file belonging to the root node. Any other node should return
     * null
     * @return the NXS file as a File object.
     */
    public File getNXSFile() {
        return nxsFile;
    }

    /**
     * Get the NXDL file belonging to the root node. Any other node should return
     * null
     * @return the NXDL file as a File object.
     */
    public File getNXDLFile() {
        return nxdlFile;
    }

    /**
     * Set the NXDL file belonging to the root node. Should not be set.
     * @param nxdlFile the NXDL file as a File object.
     */
    public void setNXDLFile(File nxdlFile) {
        this.nxdlFile = nxdlFile;
    }

    /**
     * Get the resultant file produced after NXConvert has been run on the
     * NXS file
     * (reduced to XML).
     * @return the NXConvert reduced file.
     */
    public File getReducedFile() {
        return reducedFile;
    }

    /**
     * Set the resultant file produced after NXConvert has been run on the
     * NXS file.
     * @param reducedFile the NXConvert reduced file.
     */
    public void setReducedFile(File reducedFile) {
        this.reducedFile = reducedFile;
    }

    /**
     * Get the resultant file produced after the validation procedure has been
     * performed.
     * @return the validation results file.
     */
    public File getResultsFile() {
        return resultsFile;
    }

    /**
     * Get the resultant file produced after the validation procedure has been
     * performed.
     * @param resultsFile the validation results file.
     */
    public void setResultsFile(File resultsFile) {
        this.resultsFile = resultsFile;
    }

    /**
     * Get the resultant DOM document produced after the validation procedure
     * has been performed.
     * @return the validation results DOM document.
     */
    public Document getResultsDoc() {
        return resultsDoc;
    }

    /**
     * Get the resultant DOM document produced after the validation procedure
     * has been performed.
     * @param resultsDoc the validation results DOM document.
     */
    public void setResultsDoc(Document resultsDoc) {
        this.resultsDoc = resultsDoc;
    }

    /**
     * Initially the Nexus file is converted to a reduced (all data removed)
     * XML document via the Nexus convert command. This method gets the
     * W3C DOM document of the reduced XML file.
     * @return the W3C DOM document of the reduced XML file.
     */
    public Document getReducedDoc() {
        return reducedDoc;
    }

    /**
     * Initially the Nexus file is converted to a reduced (all data removed)
     * XML document via the Nexus convert command. This method sets the
     * W3C DOM document of the reduced XML file.
     * @param reducedDoc the W3C DOM document of the reduced XML file.
     */
    public void setReducedDoc(Document reducedDoc) {
        this.reducedDoc = reducedDoc;
    }

    /**
     * If a node in the reduced XML document fails one of the schematron tests
     * then the node can be marked as a bad node with a boolean flag. The flag
     * is set to true if the node failed one of the tests.
     * @param badNode true if the node failed one of the schematron tests.
     */
    public void setBadNode(boolean badNode) {
        domNode.setUserData("bad", new Boolean(badNode),null);
        this.badNode = badNode;
    }

    /**
     * If a node in the reduced XML document fails one of the schematron tests
     * then the node can be marked as a bad node with a boolean flag. The flag
     * is set to true if the node failed one of the tests.
     * @return true if the node failed one of the schematron tests.
     */
    public boolean getBadNode() {
        checkBadNode();
        return badNode;
    }

    /**
     * If a node in the reduced XML document fails one of the schematron report tests
     * then the node can be marked as a warning node with a boolean flag. The flag
     * is set to true if the node failed one of the tests.
     * @param warnNode true if the node failed one of the schematron tests.
     */
    public void setWarnNode(boolean warnNode) {
        domNode.setUserData("warn", new Boolean(warnNode),null);
        this.warnNode = warnNode;
    }

    /**
     * If a node in the reduced XML document fails one of the schematron report tests
     * then the node can be marked as a warning node with a boolean flag. The flag
     * is set to true if the node failed one of the tests.
     * @return true if the node failed one of the schematron tests.
     */
    public boolean getWarnNode() {
        checkWarnNode();
        return warnNode;
    }

    /**
     * If a node has been validated then we can indicate the fact with a boolean flag. The flag
     * is set to true if the node has been validated.
     * @param validatedNode true if the node has been validated.
     */
    public void setValidatedNode(boolean validatedNode) {
        domNode.setUserData("validated", new Boolean(validatedNode),null);
        this.validatedNode = validatedNode;
    }

    /**
     * If a node has been validated then we can indicate the fact with a boolean flag. The flag
     * is set to true if the node has been validated.
     * @return true if the node failed one of the schematron tests.
     */
    public boolean getValidatedNode() {
        checkValidatedNode();
        return validatedNode;
    }

    /**
     * Get the W3C DOM node corresponding to this tree node.
     * @return the W3C DOM node corresponding to this tree node.
     */
    public Node getDomNode() {
        return domNode;
    }

    /**
     * A convenience method to force the checking if a node is bad
     * (failed schematron tests). Sets the flag to indicate the node is bad.
     */
    public void checkBadNode() {
        if (!isRoot) {
            Boolean bad = (Boolean) domNode.getUserData("bad");
            if (bad != null) {
                this.badNode = bad.booleanValue();
            }
        }
    }

    /**
     * A convenience method to force the checking if a node has warnings
     * . Sets the flag to indicate the node has warnings.
     */
    public void checkWarnNode() {
        if (!isRoot) {
            Boolean warn = (Boolean) domNode.getUserData("warn");
            if (warn != null) {
                this.warnNode = warn.booleanValue();
            }
        }
    }

    /**
     * A convenience method to force the checking if a node has been validated
     * . Sets the flag to indicate the node has warnings.
     */
    public void checkValidatedNode() {
        if (!isRoot) {
            Boolean validated = (Boolean) domNode.getUserData("validated");
            if (validated != null) {
                this.validatedNode = validated.booleanValue();
            }
        }
    }

    /**
     * A method to check to see if any of a nodes children are bad.
     * @return true if there are any bad children
     *
     */
    public boolean checkBadChildren() {

        boolean result = false;
        Boolean bad;


        if (!isRoot) {

            for (int i = 0; i < domNode.getChildNodes().getLength(); i++) {
                Node node = domNode.getChildNodes().item(i);
                bad = (Boolean) node.getUserData("bad");
                if (bad != null) {
                    result = true;
                }

                // Now let's check the grandkids... and beyond...
                bad = (Boolean) checkChildren(node);
                if (bad) {
                    result = true;
                }
            }

        }
        return result;
    }

    /**
     * Recursive method used by checkBadChildren() to recursively check down
     * the tree.
     * @param node
     * @return true if a child anywhere in the tree is bad!
     */
    private boolean checkChildren(Node node) {
        boolean result = false;
        for (int i = 0; i < node.getChildNodes().getLength(); i++) {
            Node tmpNode = node.getChildNodes().item(i);
            if (tmpNode.hasChildNodes()) {
                checkChildren(tmpNode);
            }
            Boolean bad = (Boolean) tmpNode.getUserData("bad");
            if (bad != null) {
                result = true;
            }
        }
        return result;
    }

    /**
     * Resets the various properties of a node, i.e flag to indicate that it
     * is a bad node, the text for the tests that failed etc.
     */
    public void resetNode() {

        badNode = false;
        domNode.setUserData("texts", null, null);
        domNode.setUserData("tests", null, null);
        domNode.setUserData("diags", null, null);
        domNode.setUserData("diagatts", null, null);
        domNode.setUserData("bad", new Boolean(false), null);
    }

    /**
     * This is a list of text strings which correspond to the error message
     * results of the schematron tests.
     * @return list of text strings corresponding to the schematron test results.
     */
    public ArrayList<String> getNodeTexts() {
        if (!isRoot) {
            return (ArrayList<String>) domNode.getUserData("texts");
        } else {
            return null;
        }
    }

    /**
     * This is a list of text strings which correspond to the schematron
     * test descriptions.
     * @return list of text strings corresponding to the schematron test
     *         descriptions.
     */
    public ArrayList<String> getNodeTests() {
        if (!isRoot) {
            return (ArrayList<String>) domNode.getUserData("tests");
        } else {
            return null;
        }
    }

    /**
     * This is a list of text strings which correspond to the schematron
     * diagnostic errors.
     * @return list of text strings corresponding to the schematron
     *         diagnostic errors.
     */
    public ArrayList<String> getNodeDiags() {
        if (!isRoot) {
            return (ArrayList<String>) domNode.getUserData("diags");
        } else {
            return null;
        }
    }

    public ArrayList<String> getNodeDiagAtts() {
        if (!isRoot) {
            return (ArrayList<String>) domNode.getUserData("diagatts");
        } else {
            return null;
        }
    }

    // Return the node name
    @Override
    public String toString() {
        return nodeName;
    }

    public int getIndex(TreeNode child1) {

        NXNodeMapper child = (NXNodeMapper) child1;
        int count = getChildCount();
        if (isRoot) {

            if (documents.contains(child)) {

                return documents.indexOf(child);
            }

        } else {
            for (int i = 0; i
                    < count; i++) {
                NXNodeMapper n = this.getChildAt(i);

                if (child.domNode == n.domNode) {
                    return i;
                }
            }
        }
        return -1; // Should never get here.
    }

    public NXNodeMapper getChildAt(int searchIndex) {

        if (isRoot) {

            return documents.get(searchIndex);

        } else {
            //Note: JTree index is zero-based.
            Node node = domNode.getChildNodes().item(searchIndex);

            // Return Nth displayable node
            int elementNodeIndex = 0;



            for (int i = 0; i
                    < domNode.getChildNodes().getLength(); i++) {
                node = domNode.getChildNodes().item(i);

                if ((node.getNodeType() == ELEMENT_TYPE)
                        && (elementNodeIndex++ == searchIndex)) {
                    break;
                }
            }
            return new NXNodeMapper(node, false, node.getNodeName());
        }

    }

    public int getChildCount() {

        if (isRoot) {

            return documents.size();



        }

        return childCount;



    }

    public Enumeration children() {
        return new children();

    }

    public boolean isLeaf() {
        return isLeaf;
    }

    public boolean getAllowsChildren() {
        return allowsChildren;
    }

    public TreeNode getParent() {

        if (isRoot) {
            return null;
        } else if (isDocument) {

            return root;

        } else if (domNode.getParentNode() != null) {

            if (domNode.getParentNode().getNodeType() == domNode.DOCUMENT_NODE) {

                Document doc = (Document) domNode.getParentNode();

                return new NXNodeMapper(domNode.getParentNode(), true,
                        ((File) doc.getUserData("file")).getAbsolutePath());

            } else {

                return new NXNodeMapper(domNode.getParentNode(), false,
                        domNode.getParentNode().getNodeName());
            }
        } else {
            return null;
        }

    }

    public NXNodeMapper getRoot() {

        if(domNode==null){
             return null;
        }

        Node tempNode = domNode.getParentNode();

        boolean gotRoot = false;
        NXNodeMapper rootNode = null;

        if (isRoot) {
            return null;
        } else if (isDocument) {

            return this;

        } else if (tempNode != null) {

            while(!gotRoot){

                if (tempNode.getNodeType() == domNode.DOCUMENT_NODE) {

                    Document doc = (Document) tempNode;

                    rootNode = new NXNodeMapper(tempNode, true,
                        ((File) doc.getUserData("file")).getAbsolutePath());
                    gotRoot = true;
                }
                else{
                    tempNode = tempNode.getParentNode();
                }
            }

        } else {
            return null;
        }

        return rootNode;

    }

    /**
     * Each node of the reduced XML document may have attributes associated
     * with it, this method provides a list of the attributes. Each string
     * contains the attribute name and the value.
     * @return a list of the attributes and their values.
     */
    public String[] getAttributeList() {

        if (isRoot) {
            return new String[0];


        }

        ArrayList<String> atts = new ArrayList<String>();



        if (domNode.hasAttributes()) {

            NamedNodeMap att = domNode.getAttributes();


            int na = domNode.getAttributes().getLength();



            for (int i = 0; i
                    < na;
                    ++i) {
                atts.add(att.item(i).getNodeName() + " = "
                        + att.item(i).getNodeValue());


            }

        }
        return atts.toArray(new String[0]);
    }

    /**
     * Each node of the reduced XML document may have a value associated with it.
     * This method returns that value as a string.
     * @return the value of the XML node.
     */
    public String getValue() {

        if (isRoot) {
            return "";


        }

        if (domNode.getNodeType() == ELEMENT_TYPE) {
            return getTextValue(domNode);


        }

        if (domNode.getTextContent() != null) {
            return domNode.getTextContent().trim();


        }

        return "";



    }

    private ArrayList<Node> getElements() {

        ArrayList<Node> nodes = new ArrayList<Node>();



        for (int i = 0; i
                < domNode.getChildNodes().getLength(); i++) {
            Node node = domNode.getChildNodes().item(i);



            if (node.getNodeType() == ELEMENT_TYPE) {
                nodes.add(node);


                ++childCount;


            }
        }
        return nodes;


    }

    private String getTextValue(Node node) {

        if (node.hasChildNodes()) {

            NodeList nodes = node.getChildNodes();



            for (int i = 0; i
                    < nodes.getLength();
                    ++i) {
                if (nodes.item(i).getNodeType() == Node.TEXT_NODE) {
                    return nodes.item(i).getTextContent().trim();


                }
            }

        }
        return "";


    }

    /**
     * Returns a list of nodes that represent the Nexus documents that are open
     * i.e. that have been reduced.
     * @return a list of Nexus document nodes.
     */
    public ArrayList<NXNodeMapper> getOpenNodes() {
        return documents;
    }

    /**
     * Removes the list of nodes that represent the Nexus documents that are
     * open i.e. that have been reduced.
     */
    public void removeAllNodes() {
        documents.clear();
    }


    /**
     * Check to see if the node has any vary bad kids, return true if it has.
     * @return true if the kids are bad.
     */
    public boolean hasBadChildren(){
        TreeUtils treeUtils = new TreeUtils();
        return treeUtils.hasBadChildren(this);
    }


    /**
     * A class that represents the child nodes of a node from a
     * reduced document. The child nodes are represented as an enumeration.
     */
    private class children implements Enumeration {

        private int count = 0;
        private boolean more = true;
        private Node node = null;
        private NXNodeMapper nxNode = null;

        public boolean hasMoreElements() {

            if (isRoot) {

                if (documents == null) {
                    more = false;
                    return more;
                }

                if (count < documents.size()) {
                    more = true;
                } else {
                    more = false;
                }
            } else {

                if (children == null) {
                    more = false;
                    return more;
                }

                if (count < children.size()) {
                    more = true;
                } else {
                    more = false;
                }

            }
            return more;
        }

        public Object nextElement() {

            if (isRoot) {

                if (count < documents.size()) {
                    nxNode = documents.get(count);
                    count++;
                    return nxNode;
                } else {
                    throw new NoSuchElementException();
                }

            } else {

                if (count < children.size()) {
                    node = children.get(count);
                    count++;
                    return new NXNodeMapper(node, false, node.getNodeName());
                } else {
                    throw new NoSuchElementException();
                }

            }
        }
    }

    public void insert(MutableTreeNode child, int index) {

        NXNodeMapper childNode = (NXNodeMapper) child;



        if (isRoot) {
            documents.add(index, childNode);


        } else {
            NodeList list = domNode.getChildNodes();



            for (int i = 0; i
                    < list.getLength();
                    ++i) {
                if (i == index) {
                    domNode.insertBefore(childNode.domNode, list.item(i));


                }
            }
        }
    }

    public void remove(int index) {
        if (isRoot) {

            documents.remove(index);



        } else {

            NodeList list = domNode.getChildNodes();



            for (int i = 0; i
                    < list.getLength();
                    ++i) {
                if (i == index) {
                    domNode.removeChild(list.item(index));


                }
            }

        }
    }

    public void remove(MutableTreeNode node) {

        NXNodeMapper childNode = (NXNodeMapper) node;



        if (isRoot) {
            documents.remove((NXNodeMapper) node);


        } else {
            NodeList list = domNode.getChildNodes();



            for (int i = 0; i
                    < list.getLength();
                    ++i) {
                if (list.item(i).isSameNode(childNode.domNode)) {
                    domNode.removeChild(childNode.domNode);


                }
            }
        }
    }

    public void removeFromParent() {
        if (isRoot) {
            return;


        } else {
            NXNodeMapper parentNode = (NXNodeMapper) getParent();
            parentNode.remove(this);


        }
    }

    public void setParent(MutableTreeNode newParent) {
    }

    public void setUserObject(Object object) {

        userObject = object;


    }
}
