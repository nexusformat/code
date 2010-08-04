package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

/**
 * An interface which represents the concept that the elements in an XML
 * Document can be good or bad after they have been validated against some
 * criteria defined in the implementation.
 * @author Stephen Rankin
 */
public interface NodeFilterInterface {

    /**
     * Sets a DOM document (filter document) that contains a list of nodes
     * that have failed any validation tests, for example, the SVRL file.
     * @param filterDoc a DOM document.
     */
    public void setFilterDocument(Document filterDoc);

    /**
     * Sets the document to which to apply the filter.
     * @param doc a DOM document.
     */
    public void setDocument(Document doc);

    /**
     * A list of nodes which are bad.
     * @return
     */
    public ArrayList<Node> getBadNodeList();


    /**
     * Reset the bad nodes to indicate that they are now good nodes, i.e.
     * resets them back to there original state.
     */
    public void resetBadNodes();

}
