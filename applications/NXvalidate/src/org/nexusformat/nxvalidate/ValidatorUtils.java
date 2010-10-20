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
 * ValidatorUtils.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import org.nexusformat.nxvalidate.exceptions.NXvalidateException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A class the wraps the conversion and validation processes.
 * @author Stephen Rankin
 */
public class ValidatorUtils {

    private File reduced = null;
    private File nxsFile = null;
    private boolean keepTemp = false;
    private File schematronFile = null;
    private boolean convertNxs = false;
    private String nxconvertCommand = null;


    public ValidatorUtils(File nxsFile, String nxconvertCommand) {

        this.nxconvertCommand = nxconvertCommand;
        this.nxsFile = nxsFile;
    }

    /**
     * Set the File object pointing to the reduced NEXUS file.
     *
     * @param reduced the reduced NEXUS File object.
     */
    public void setReduced(File reduced) {
        this.reduced = reduced;
    }

    /**
     *
     * Get the File object pointing to the reduced NEXUS file.
     *
     * @return the reduced NEXUS File object.
     */
    public File getReduced() {
        return reduced;
    }

    /**
     * Set the File object pointing to the NEXUS file.
     *
     * @param nxsFile the NEXUS File object.
     */
    public void setNXS(File nxsFile) {
        this.nxsFile = nxsFile;
    }

    /**
     *
     * Get the File object pointing to the NEXUS file.
     *
     * @return the NEXUS File object.
     */
    public File getNXS() {
        return nxsFile;
    }

    /**
     * Set the File object pointing to the Schematron file that contains the
     * rules for checking the reduced document.
     *
     * @param schematronFile the Schematron File object.
     */
    public void setSchematron(File schematronFile) {
        this.schematronFile = schematronFile;
    }

    /**
     * Get the File object pointing to the Schematron file that contains the
     * rules for checking the reduced document.
     *
     * @return the Schematron File object.
     */
    public File getSchematron() {
        return schematronFile;
    }

    /**
     * Set a flag that indicates that the conversion of a NEXUS file 
     * to a reduced document should be performed (true).
     *
     * @param convertNxs the conversion flag.
     */
    public void doConversion(boolean convertNxs) {
        this.convertNxs = convertNxs;
    }

    /**
     * Set a flag that indicates that the temporary files
     * (such as the reduced document and the validation results)
     * should be kept (true).
     *
     * @param keepTemp the keep temp files flag.
     */
    public void setKeepTemp(boolean keepTemp) {
        this.keepTemp = keepTemp;
    }

    /**
     * This methods performs the validation using the NEXUS (or reduced) file
     * and the Schematron file. The returned file is the results file.
     * @return the results file as a File object.
     * @throws NXvalidateException
     */
    public File validate() throws NXvalidateException {

        File result = null;

        //Do the conversion to the reduced format.
        if (convertNxs && nxsFile != null) {
            try {
                NXconvert converter = new NXconvert(nxsFile, keepTemp, nxconvertCommand);
                reduced = converter.convert();
            } catch (Exception e) {
                Logger.getLogger(ValidatorUtils.class.getName()).log(Level.SEVERE,
                        "While converting \"" + nxsFile
                        + "\" to reduced xml format");
                throw new NXvalidateException("While converting \"" + nxsFile
                        + "\" to reduced xml format");
            }
        }

        //Do the validation.
        if (reduced != null && schematronFile != null) {

            // create the validation setup
            NXschematron schematron = new NXschematron(nxsFile, reduced,
                    schematronFile, keepTemp);

            try {
                result = schematron.validate();
            } catch (Exception e) {
                Logger.getLogger(ValidatorUtils.class.getName()).log(Level.SEVERE,
                        "While creating validation report");
                throw new NXvalidateException("While creating validation report", e);
            }
        }

        return result;

    }
}
