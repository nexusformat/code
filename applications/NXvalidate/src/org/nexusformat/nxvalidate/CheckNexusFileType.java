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
 * CheckNexusFileType.java
 *
 */
package org.nexusformat.nxvalidate;

import com.sun.org.apache.xerces.internal.impl.io.MalformedByteSequenceException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.ConnectException;
import java.net.MalformedURLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

/**
 *
 * @author Stephen Rankin
 */
public class CheckNexusFileType {

    private DocumentBuilderFactory factory = null;
    private DocumentBuilder builder = null;

    public CheckNexusFileType() {
        factory = DocumentBuilderFactory.newInstance();
        factory.setNamespaceAware(true);
        try {
            builder = factory.newDocumentBuilder();
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(CheckNexusFileType.class.getName()).log(
                    Level.SEVERE, null, ex);
        }
    }

    /**
     * Checks to see if a file is a HDF/Nexus file and returns true if it is.
     * The method checks for HDF4, HDF5 and Nexus XML files.
     * @param file the input file to check.
     * @return boolean which is true if the file is a HDF/Nexus file.
     * @throws FileNotFoundException
     * @throws IOException
     */
    public boolean checkNexusFile(File file) {
        try {
            if (checkHDF5(file)) {
                return true;
            } else if (checkHDF4(file)) {
                return true;
            } else if (checkNexusXML(file)) {
                return true;
            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    /**
     * Checks to see if a file is a HDF5 file and returns true if it is.
     * @param file the input file to check.
     * @return boolean which is true if the file is a HDF5 file.
     * @throws FileNotFoundException
     * @throws IOException
     */
    public boolean checkHDF5(File file) throws FileNotFoundException,
                                               IOException {

        byte[] b = new byte[7];
        FileInputStream stream = new FileInputStream(file);
        stream.read(b);
        stream.close();
        if (b[0] != -119) {
            return false;
        } else if (b[1] != 72) {
            return false;
        } else if (b[2] != 68) {
            return false;
        } else if (b[3] != 70) {
            return false;
        } else if (b[4] != 13) {
            return false;
        } else if (b[5] != 10) {
            return false;
        } else if (b[6] != 26) {
            return false;
        }

        return true;
    }

    /**
     * Checks to see if a file is a HDF4 file and returns true if it is.
     * @param file the input file to check.
     * @return boolean which is true if the file is a HDF4 file.
     * @throws FileNotFoundException
     * @throws IOException
     */
    public boolean checkHDF4(File file) throws FileNotFoundException,
                                               IOException {

        byte[] b = new byte[4];
        FileInputStream stream = new FileInputStream(file);
        stream.read(b);
        stream.close();
        if (b[0] != 14) {
            return false;
        } else if (b[1] != 3) {
            return false;
        } else if (b[2] != 19) {
            return false;
        } else if (b[3] != 1) {
            return false;
        }

        return true;

    }

    /**
     * Checks to see if a file is a Nexus XML file and returns true if it is.
     * @param file the input file to check.
     * @return boolean which is true if the file is a Nexus XML file.
     * @throws FileNotFoundException
     * @throws IOException
     */
    private boolean checkNexusXML(File file) {

        boolean result = false;

        try {
            Document resultsDoc = builder.parse(file);
            if (resultsDoc.getDocumentElement().getNodeName().equals("NXroot")) {
                result = true;
            }
        } catch (SAXException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "SAXException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (MalformedByteSequenceException ex) {
           // Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "MalformedByteSequenceException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (ConnectException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "ConnectException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (MalformedURLException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "MalformedURLException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (FileNotFoundException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "FileNotFoundException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (IOException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
             //       "IOException: " + file.getAbsolutePath(), ex);
            return result;
        }

        return result;

    }

    /**
     * Checks to see if a file is a Nexus definition file and returns true if
     * it is.
     * @param file the input file to check.
     * @return boolean which is true if the file is a Nexus definition file.
     * @throws FileNotFoundException
     * @throws IOException
     */
    public boolean checkNXDLFile(File file) {

        boolean result = false;

        try {
            Document resultsDoc = builder.parse(file);
            if (resultsDoc.getDocumentElement().getNodeName().equals("definition")) {
                result = true;
            }
            if (resultsDoc.getDocumentElement().getNodeName().equals("definitions")) {
                result = true;
            }
        } catch (SAXException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "SAXException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (MalformedByteSequenceException ex) {
           // Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "MalformedByteSequenceException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (ConnectException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "ConnectException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (MalformedURLException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "MalformedURLException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (FileNotFoundException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
            //        "FileNotFoundException: " + file.getAbsolutePath(), ex);
            return result;
        } catch (IOException ex) {
            //Logger.getLogger(CheckNexusFileType.class.getName()).log(Level.INFO,
             //       "IOException: " + file.getAbsolutePath(), ex);
            return result;
        }

        return result;

    }

}
