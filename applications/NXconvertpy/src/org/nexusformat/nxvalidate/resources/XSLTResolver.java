/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate.resources;

/**
 * A copy from http://www.jezuk.co.uk/cgi-bin/view/jez?id=2260
 * @author ser65
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
