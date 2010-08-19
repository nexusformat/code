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
 * For further information, see <http://www.neutron.anl.gov/NeXus/>
 *
 * NodeFilterInterface.java
 *
 */

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
