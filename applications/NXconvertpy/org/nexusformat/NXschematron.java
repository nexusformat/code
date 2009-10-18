/**
 * 
 */
package org.nexusformat;

import java.io.File;
import java.io.IOException;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

public class NXschematron {

	private String fileToValidate; 
	private String SchematronToUse;
	
	public NXschematron(String ReducedNeXusFilename, String SchematronFilename) {
	
		this.fileToValidate = ReducedNeXusFilename;
		this.SchematronToUse = SchematronFilename;
		
	}

	public static void TransformoMatic(String inputFilename,
			String xslFilename, String outputFilename)
			throws TransformerException {
		TransformoMatic(new File(inputFilename), new File(xslFilename),
				new File(outputFilename));
	}

	public static void TransformoMatic(File inputFile, File xslFile,
			File outputFile) throws TransformerException {

		// Create an instance of the transform factory
		TransformerFactory myFactory = TransformerFactory.newInstance(
				"net.sf.saxon.TransformerFactoryImpl", null);

		// Create a transformer for the xsl stylesheet
		Transformer transformer = myFactory.newTransformer(new StreamSource(
				xslFile));

		// Do the transform
		transformer.transform(new StreamSource(inputFile), new StreamResult(
				outputFile));

	}

	protected String schematronxslt[] = new String[] { "iso_dsdl_include.xsl",
			"iso_abstract_expand.xsl", "iso_svrl_for_xslt2.xsl" };

	String validate() throws IOException, TransformerException {
		return this.validate(this.fileToValidate, this.SchematronToUse);
	}
	
	String validate(String filename, String schematron) throws IOException,
			TransformerException {

		// The Schematron files...
		File xslt1File = new File("iso_dsdl_include.xsl");
		File xslt2File = new File("iso_abstract_expand.xsl");
		File xslt3File = new File("iso_svrl_for_xslt2.xsl");

		// Step 1
		File schematronFile = new File(schematron);
		File schematron1 = File.createTempFile("schematron", ".step1");
		TransformoMatic(schematronFile, xslt1File, schematron1);

		// Step 2
		File schematron2 = File.createTempFile("schematron", ".step2");
		TransformoMatic(schematron1, xslt2File, schematron2);

		// Step 3
		File SchemaFile = File.createTempFile("schema", ".xslt");
		TransformoMatic(schematron2, xslt3File, SchemaFile);

		// Now lets validate the actual reduced file.
		File resultsFile = File.createTempFile("result", ".xml");
		TransformoMatic(new File(filename), SchemaFile, resultsFile);

		// Return the filename
		return resultsFile.getCanonicalPath();
	}

	public static void main(String[] args) {
		if (args.length !=2) {
			System.out.println("Must specify two input files");
			return;
		}
		try {
			NXschematron sch = new NXschematron();
			String results = sch.validate(args[0], args[1]);
			System.out.println(results);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
