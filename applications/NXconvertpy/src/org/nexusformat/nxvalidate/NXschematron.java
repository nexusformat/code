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

    private File reducedNeXusFile;
    private File schematronFile;
    private boolean keepTemp;
    private InputStream dsdlIncludeXSLTStream = null;
    private InputStream abstractExpandXSLTStream = null;
    private InputStream svrlForXslt2XSLTStream = null;
    private InputStream nxdl2schXSLTStream = null;
    //protected String schematronxslt[] = new String[]{"iso_dsdl_include.xsl",
    //    "iso_abstract_expand.xsl", "iso_svrl_for_xslt2.xsl"};

    public NXschematron(File reducedNeXusFile, File schematronFile,
            final boolean keepTemp) {

        this.reducedNeXusFile = reducedNeXusFile;
        this.schematronFile = schematronFile;
        this.keepTemp = keepTemp;

        // The Schematron files...
        dsdlIncludeXSLTStream =
                NXschematron.class.getResourceAsStream(
                "resources/iso_dsdl_include.xsl");
        abstractExpandXSLTStream =
                NXschematron.class.getResourceAsStream(
                "resources/iso_abstract_expand.xsl");
        svrlForXslt2XSLTStream =
                NXschematron.class.getResourceAsStream(
                "resources/iso_svrl_for_xslt2.xsl");
        //XSLT for NXDL to sch
        nxdl2schXSLTStream =
                NXschematron.class.getResourceAsStream(
                "resources/nxdl2sch.xsl");

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

    File validate() throws IOException, TransformerException {
        return validate(reducedNeXusFile, schematronFile);
    }

    File validate(File reducedNeXusFile, File schematronFile) throws
            IOException, TransformerException {

        // Step 0
        File schematron0 = File.createTempFile("nxdlFile", ".step0");
        if (!this.keepTemp) {
            schematron0.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(schematronFile), nxdl2schXSLTStream,
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
        TransformoMatic(new FileInputStream(schematron1),
                abstractExpandXSLTStream,
                new FileOutputStream(schematron2));

        // Step 3
        File schemaFile = File.createTempFile("schema", ".xslt");
        if (!this.keepTemp) {
            schemaFile.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(schematron2),svrlForXslt2XSLTStream,
                new FileOutputStream(schemaFile));

        // Now lets validate the actual reduced file.
        File resultsFile = File.createTempFile("result", ".xml");

        if (!this.keepTemp) {
            resultsFile.deleteOnExit();
        }
        TransformoMatic(new FileInputStream(reducedNeXusFile),
                new FileInputStream(schemaFile),
                new FileOutputStream(resultsFile));

        // Return the filename
        return resultsFile;
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Must specify two input files");
            return;
        }
        try {
            NXschematron sch = new NXschematron(
                    new File(args[0]), new File(args[1]), false);
            File results = sch.validate();
            System.out.println(results.getAbsolutePath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
