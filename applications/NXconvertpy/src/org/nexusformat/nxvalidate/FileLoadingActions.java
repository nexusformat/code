/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.ResourceBundle;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.JTree;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.nexusformat.nxvalidate.exceptions.NXConvertpyException;

/**
 *
 * @author Stephen Rankin
 */
public class FileLoadingActions implements Runnable {

    private File nxsFile = null;
    private File nxdlFile = null;
    private File reducedFile = null;
    private File resultsFile = null;
    private File nxconvertFile = null;
    private DocumentBuilderFactory factory = null;
    private DocumentBuilder builder = null;
    private NXReducedToTree domTree = null;
    private NXNodeMapper root = null;
    private int which = 1;
    private TreeUtils treeUtils = null;
    private ValidatorUtils validator = null;
    private Document reducedDoc = null;
    private Document resultsDoc = null;
    private JTree jTree = null;
    private boolean validateResult = false;
    private ResourceBundle bundle = null;
    private JOptionPane dialogReportProblem;
    private NXvalidateFrame frame = null;
    private File[] dataFiles = null;
    private ArrayList<String> dataFileList = null;
    private ArrayList<String> badDataFileList = null;
    private boolean conversionFail = false;
    private boolean isNotBulk = false;

    public FileLoadingActions(NXvalidateFrame frame, JTree jTree,
            DocumentBuilder builder, NXReducedToTree domTree,
            NXNodeMapper root) {

        this.domTree = domTree;
        this.root = root;
        this.builder = builder;
        this.frame = frame;
        this.jTree = jTree;

        bundle = ResourceBundle.getBundle(
                "org/nexusformat/nxvalidate/resources/nxvalidate");
        dialogReportProblem = new JOptionPane();

        treeUtils = new TreeUtils();

    }

    public void setWhich(int which) {
        this.which = which;
    }

    public void setNXSFile(File nxsFile) {
        this.nxsFile = nxsFile;
    }

    public void setNXDLFile(File nxdlFile) {
        this.nxdlFile = nxdlFile;
    }

    public void setNXConvertFile(File nxconvertFile) {
        this.nxconvertFile = nxconvertFile;
    }

    public File getNXConvertFile() {
        return nxconvertFile;
    }

    public void setReducedFile(File reducedFile) {
        this.reducedFile = reducedFile;
    }

    public void setResultsDoc(Document resultsDoc) {
        this.resultsDoc = resultsDoc;
    }

    public void setReducedDoc(Document reducedDoc) {
        this.reducedDoc = reducedDoc;
    }

    public void setDataFiles(File[] dataFiles) {
        this.dataFiles = dataFiles;
    }

    public boolean getValidateResult() {
        return validateResult;
    }

    public ArrayList<String> getDataFileList() {
        return dataFileList;
    }

    public void setDataFileList(ArrayList<String> dataFileList) {
        this.dataFileList = dataFileList;
    }

    public boolean getConversionResult() {
        return conversionFail;
    }

    public ArrayList<String> getBadDataFileList() {
        return badDataFileList;
    }

    public void validateFile() {

        try {

            //Validation has already been done so we reset the tree
            //before doing the validation again.
            if (resultsDoc != null) {
                SVRLNodeFilter filter = new SVRLNodeFilter();
                filter.setFilterDocument(resultsDoc);
                filter.setDocument(reducedDoc);
                filter.resetBadNodes();
            }

            //Do the validation.
            if (nxconvertFile != null) {
                validator = new ValidatorUtils(nxconvertFile);
            } else {
                dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating");
            }
            validator.setSchematron(nxdlFile);
            validator.setReduced(reducedFile);
            resultsFile = validator.validate();

            SVRLNodeFilter filter = new SVRLNodeFilter();

            resultsDoc = builder.parse(resultsFile);
            filter.setFilterDocument(resultsDoc);
            filter.setDocument(reducedDoc);
            filter.getBadNodeList();

            treeUtils.setResultsDoc(jTree, resultsDoc);
            treeUtils.setResultsFile(jTree, resultsFile);

            //domTree.updateTree();
            Logger.getLogger(NXvalidateFrame.class.getName()).log(
                    Level.INFO, "Finished Validating.");

            if(isNotBulk){
                dialogReportProblem.showMessageDialog(
                    frame,
                    bundle.getString("validationCompleteMessage"));
            }
        } catch (NXConvertpyException ex) {
            Logger.getLogger(NXvalidateFrame.class.getName()).log(
                    Level.SEVERE, null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(NXvalidateFrame.class.getName()).log(
                    Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(NXvalidateFrame.class.getName()).log(
                    Level.SEVERE, null, ex);
        }

    }

    public void loadFile() {

        try {

            //Reduce the file with NXConvert.
            NXconvert convert = new NXconvert(nxsFile, true, nxconvertFile);
            File reducedFile = convert.convert();

            //Display reduced file
            Document document = builder.parse(reducedFile);
            NXNodeMapper node = new NXNodeMapper(
                    document, true, nxsFile);
            node.setReducedDoc(document);
            this.reducedDoc = document;
            if (nxdlFile != null) {
                node.setSchematronFile(nxdlFile);
            }
            this.reducedFile = reducedFile;
            node.setReducedFile(reducedFile);
            node.setRoot(root);
            root.insert(node);
            //domTree.updateTree();

        } catch (NXConvertpyException ex) {
            conversionFail = true;
            badDataFileList.add(nxsFile.getAbsolutePath());
        } catch (InterruptedException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    null, ex);
        } catch (IOException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    null, ex);
        }
    }

    private ArrayList<String> getSubFiles(String file) {

        ArrayList<String> fileList = new ArrayList<String>();

        File tmpFile = new File(file);

        String[] files = tmpFile.list();

        if (files == null) {
            return fileList;
        }

        for (int i = 0; i < files.length; ++i) {
            if (new File(tmpFile.getAbsolutePath() + tmpFile.separator + files[i]).isDirectory()) {
                fileList.addAll(getSubFiles(tmpFile.getAbsolutePath() + tmpFile.separator + files[i]));
            } else {
                fileList.add(tmpFile.getAbsolutePath() + tmpFile.separator + files[i]);
            }
        }

        return fileList;
    }

    public void bulkCheck() {

        if (dataFiles != null) {

            dataFileList = new ArrayList<String>();

            for (int i = 0; i < dataFiles.length; ++i) {

                if (dataFiles[i].isDirectory()) {
                    dataFileList.addAll(getSubFiles(dataFiles[i].getAbsolutePath()));
                } else {
                    dataFileList.add(dataFiles[i].getAbsolutePath());
                }

            }
            this.dataFileList = dataFileList;
        }

    }

    public void bulkLoad() {
        //Do conversion for each selected file.
        badDataFileList = new ArrayList<String>();
        if (dataFileList != null) {
            for (int i = 0; i < dataFileList.size(); ++i) {
                nxsFile = new File(dataFileList.get(i));
                loadFile();
            }
            domTree.updateTree();
        }
    }

    public void bulkValidate() {

        //Do Conversion and validation.
        badDataFileList = new ArrayList<String>();
        if (dataFileList != null) {
            for (int i = 0; i < dataFileList.size(); ++i) {
                nxsFile = new File(dataFileList.get(i));
                loadFile();
                validateFile();
            }
            domTree.updateTree();
        }

    }

    public void run() {

        if (which == 1) {
            isNotBulk = true;
            loadFile();
            domTree.updateTree();
        } else if (which == 2) {
            isNotBulk = true;
            validateFile();
            domTree.updateTree();
        } else if (which == 3) {
            bulkCheck();
        } else if (which == 4) {
            bulkLoad();
        }else if (which == 5) {
            bulkValidate();
        }

    }
}
