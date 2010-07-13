/**
 * 
 */
package org.nexusformat.nxvalidate;

import org.nexusformat.nxvalidate.resources.XSLTResolver;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import java.io.InputStream;
import java.io.OutputStream;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

public class NXschematron {

    private String fileToValidate;
    private String SchematronToUse;
    private boolean keepTemp;
    private InputStream dsdlIncludeXSLTStream = null;
    private InputStream abstractExpandXSLTStream = null;
    private InputStream svrlForXslt2XSLTStream = null;
    private InputStream nxdl2schXSLTStream = null;
    //protected String schematronxslt[] = new String[]{"iso_dsdl_include.xsl",
    //    "iso_abstract_expand.xsl", "iso_svrl_for_xslt2.xsl"};

    public NXschematron(String ReducedNeXusFilename, String SchematronFilename,
            final boolean keepTemp) {

        this.fileToValidate = ReducedNeXusFilename;
        this.SchematronToUse = SchematronFilename;
        this.keepTemp = keepTemp;

        // The Schematron files...
        dsdlIncludeXSLTStream =
                NXschematron.class.getResourceAsStream("resources/iso_dsdl_include.xsl");
        abstractExpandXSLTStream =
                NXschematron.class.getResourceAsStream("resources/iso_abstract_expand.xsl");
        svrlForXslt2XSLTStream =
                NXschematron.class.getResourceAsStream("resources/iso_svrl_for_xslt2.xsl");
        //XSLT for NXDL to sch
        nxdl2schXSLTStream =
                NXschematron.class.getResourceAsStream("resources/nxdl2sch.xsl");

    }

    /**
     * Transform an XML file to somthing else given ans XSLT transformation.
     * @param inputFilename the XML input file name.
     * @param xslFilename the xslt file name.
     * @param outputFilename the filename to store the results
     *        of the transformation.
     * @throws TransformerException
     */
    public static void TransformoMatic(String inputFilename,
            String xslFilename, String outputFilename)
            throws TransformerException {
        TransformoMatic(new File(inputFilename), new File(xslFilename),
                new File(outputFilename));
    }

    /**
     * Transform an XML file to somthing else given ans XSLT transformation.
     * @param inputFile the XML input file
     * @param xslFile the xslt file.
     * @param outputFile the result of the transformation.
     * @throws TransformerException
     */
    public static void TransformoMatic(File inputFile, File xslFile,
            File outputFile) throws TransformerException {

        // Create an instance of the transform factory
        TransformerFactory myFactory = TransformerFactory.newInstance(
                "net.sf.saxon.TransformerFactoryImpl", null);
        myFactory.setURIResolver(new XSLTResolver());
        // Create a transformer for the xsl stylesheet
        Transformer transformer = myFactory.newTransformer(new StreamSource(
                xslFile));
        transformer.setParameter("generate-paths", "yes");
        transformer.setURIResolver(new XSLTResolver());
        // Do the transform
        transformer.transform(new StreamSource(inputFile), new StreamResult(
                outputFile));

    }

    public static void TransformoMatic(InputStream inputFileStream,
            InputStream xslFileStream, OutputStream outputFileStream)
            throws TransformerException {

        // Create an instance of the transform factory
        TransformerFactory myFactory = TransformerFactory.newInstance(
                "net.sf.saxon.TransformerFactoryImpl", null);

        myFactory.setURIResolver(new XSLTResolver());
        // Create a transformer for the xsl stylesheet
        Transformer transformer = myFactory.newTransformer(new StreamSource(
                xslFileStream));
        
        transformer.setParameter("generate-paths", "yes");
        transformer.setURIResolver(new XSLTResolver());
        // Do the transform
        transformer.transform(new StreamSource(inputFileStream),
                new StreamResult(outputFileStream));

    }

    String validate() throws IOException, TransformerException {
        return this.validate(this.fileToValidate, this.SchematronToUse);
    }

    String validate(String filename, String schematron) throws IOException,
            TransformerException {

        // Step 0
        File nxdlFile = new File(schematron);
        File schematron0 = File.createTempFile("nxdlFile", ".step0");
        if (!this.keepTemp) {
            schematron0.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(nxdlFile), nxdl2schXSLTStream,
                new FileOutputStream(schematron0));

        // Step 1
        File schematron1 = File.createTempFile("schematron", ".step1");
        if (!this.keepTemp) {
            schematron1.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(schematron0), dsdlIncludeXSLTStream,
                new FileOutputStream(schematron1));

        // Step 2
        File schematron2 = File.createTempFile("schematron", ".step2");
        if (!this.keepTemp) {
            schematron2.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(schematron1), abstractExpandXSLTStream,
                new FileOutputStream(schematron2));

        // Step 3
        File SchemaFile = File.createTempFile("schema", ".xslt");
        if (!this.keepTemp) {
            SchemaFile.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(schematron2),svrlForXslt2XSLTStream ,
                new FileOutputStream(SchemaFile));

        // Now lets validate the actual reduced file.
        File resultsFile = File.createTempFile("result", ".xml");

        if (!this.keepTemp) {
            resultsFile.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(filename),
                new FileInputStream(SchemaFile),
                new FileOutputStream(resultsFile));

        // Return the filename
        return resultsFile.getCanonicalPath();
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Must specify two input files");
            return;
        }
        try {
            NXschematron sch = new NXschematron(args[0], args[1], false);
            String results = sch.validate();
            System.out.println(results);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
