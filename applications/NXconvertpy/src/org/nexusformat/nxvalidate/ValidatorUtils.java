/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import org.nexusformat.nxvalidate.exceptions.NXConvertpyException;
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
    private File nxconvertFile = null;

    public ValidatorUtils(File nxconvertFile) {

        this.nxconvertFile = nxconvertFile;

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
     * @param schematronFile the Schematron File object.
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
     * @throws NXConvertpyException
     */
    public File validate() throws NXConvertpyException {

        File result = null;

        //Do the conversion to the reduced format.
        if (convertNxs && nxsFile != null) {
            try {
                NXconvert converter = new NXconvert(nxsFile, keepTemp, nxconvertFile);
                reduced = converter.convert();
            } catch (Exception e) {
                Logger.getLogger(ValidatorUtils.class.getName()).log(Level.SEVERE,
                        "While converting \"" + nxsFile
                        + "\" to reduced xml format");
                throw new NXConvertpyException("While converting \"" + nxsFile
                        + "\" to reduced xml format");
            }
        }

        //Do the validation.
        if (reduced != null && schematronFile != null) {

            // create the validation setup
            NXschematron schematron = new NXschematron(reduced,
                    schematronFile, keepTemp);

            try {
                result = schematron.validate();
            } catch (Exception e) {
                Logger.getLogger(ValidatorUtils.class.getName()).log(Level.SEVERE,
                        "While creating validation report");
                throw new NXConvertpyException("While creating validation report");
            }
        }

        return result;

    }
}
