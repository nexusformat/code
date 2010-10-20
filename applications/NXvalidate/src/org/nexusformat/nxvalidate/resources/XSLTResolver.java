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
 * XSLTResolver.java
 *
 */
package org.nexusformat.nxvalidate.resources;

/**
 * A copy from http://www.jezuk.co.uk/cgi-bin/view/jez?id=2260
 * @author Stephen Rankin
 */
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.transform.URIResolver;
import javax.xml.transform.Source;
import javax.xml.transform.TransformerException;
import javax.xml.transform.stream.StreamSource;
import java.io.InputStream;
import java.net.URI;

public class XSLTResolver implements URIResolver {

    public Source resolve(String href, String base)
            throws TransformerException {
        String newHref = null;

        if(href==null){
            return null;
        }
        else if(href.equals("")){

            try {
                URI uri = new URI(base);
                String[] path = uri.getPath().split("/");
                newHref = path[path.length-1];
                System.out.println("newHref: " + newHref);
            } catch (URISyntaxException ex) {
                Logger.getLogger(XSLTResolver.class.getName()).log(Level.SEVERE, null, ex);

            }
            return null;

        }
        
        try
        {
            if(newHref != null){
                InputStream is = XSLTResolver.class.getResourceAsStream(newHref);
                return new StreamSource(is, newHref);
            }
            else{
                InputStream is = XSLTResolver.class.getResourceAsStream(href);
                return new StreamSource(is, href);
            }
        }
        catch (Exception ex) {
            throw new TransformerException(ex);
        }
    }
}
