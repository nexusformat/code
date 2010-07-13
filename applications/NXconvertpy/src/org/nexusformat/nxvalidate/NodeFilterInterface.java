package org.nexusformat.nxvalidate;

import java.util.ArrayList;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

/**
 *
 * @author ser65
 */
public interface NodeFilterInterface {

    /**
     * Sets a DOM document (filter document) that containes a list of nodes
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

}
