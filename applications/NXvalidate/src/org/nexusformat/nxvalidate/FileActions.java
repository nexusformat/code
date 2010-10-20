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
 * FileActions.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.ResourceBundle;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.JTree;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.nexusformat.nxvalidate.exceptions.NXvalidateException;

/**
 *
 * @author Stephen Rankin
 */
public class FileActions implements Runnable {

    private File nxsFile = null;
    private File nxdlFile = null;
    private File reducedFile = null;
    private File resultsFile = null;
    private String nxconvertCommand = null;
    private File saveDirectory = null;
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

    public FileActions(NXvalidateFrame frame, JTree jTree,
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

    public void setNXConvertFile(String nxconvertCommand) {
        this.nxconvertCommand = nxconvertCommand;
    }

    public String getNXConvertFile() {
        return nxconvertCommand;
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

    public File getSaveDirectory() {
        return saveDirectory;
    }

    public void setSaveDirectory(File saveDirectory) {
        this.saveDirectory = saveDirectory;
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
                filter.resetNodes();
            }

            //Do the validation.
            if (nxconvertCommand != null) {
                validator = new ValidatorUtils(nxsFile,nxconvertCommand);
            } else {
                dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating file, nxconvert command is not set.");
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

            treeUtils.setValidated(jTree, resultsFile);

            //domTree.updateTree();
            Logger.getLogger(NXvalidateFrame.class.getName()).log(
                    Level.INFO, "Finished Validating.");

            if(isNotBulk){
                dialogReportProblem.showMessageDialog(
                    frame,
                    bundle.getString("validationCompleteMessage"));
            }
        } catch (NXvalidateException ex) {
            dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating file: " + nxsFile);
            Logger.getLogger(FileActions.class.getName()).log(
                    Level.WARNING, null, ex);
        } catch (SAXException ex) {
            dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating file: " + nxsFile);
            Logger.getLogger(FileActions.class.getName()).log(
                    Level.WARNING, null, ex);
        } catch (IOException ex) {
            dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating file: " + nxsFile);
            Logger.getLogger(FileActions.class.getName()).log(
                    Level.WARNING, null, ex);
        } catch (Exception ex) {
            dialogReportProblem.showMessageDialog(
                        frame,
                        "Problem Validating file: " + nxsFile);
            Logger.getLogger(FileActions.class.getName()).log(
                    Level.WARNING, null, ex);
        }

    }

    public void loadFile() {

        try {

            //Reduce the file with NXConvert.
            NXconvert convert = new NXconvert(nxsFile, true, nxconvertCommand);
            File reducedFile = convert.convert();
            
            if(reducedFile==null){
                return;
            }

            //Display reduced file
            Document document = builder.parse(reducedFile);
            document.setUserData("file", nxsFile, null);
            NXNodeMapper node = new NXNodeMapper(
                    document, true, nxsFile);
            node.setReducedDoc(document);
            this.reducedDoc = document;
            
            if (nxdlFile != null) {
                node.setNXDLFile(nxdlFile);
            }
            this.reducedFile = reducedFile;
            node.setReducedFile(reducedFile);
            node.setRoot(root);
            root.insert(node,0);
            //domTree.updateTree();

            
        } catch (NXvalidateException ex) {
            conversionFail = true;
            badDataFileList.add(nxsFile.getAbsolutePath());
        } catch (InterruptedException ex) {
            Logger.getLogger(
                    FileActions.class.getName()).log(Level.SEVERE,
                    null, ex);
        } catch (SAXException ex) {
            Logger.getLogger(
                    FileActions.class.getName()).log(Level.SEVERE,
                    null, ex);
        } catch (IOException ex) {
            Logger.getLogger(
                    FileActions.class.getName()).log(Level.SEVERE,
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
            dialogReportProblem.showMessageDialog(
                    frame,
                    bundle.getString("validationCompleteMessage"));
        }

    }

    private void copy(File src, File dst) throws IOException {
        InputStream in = new FileInputStream(src);
        OutputStream out = new FileOutputStream(dst);

        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0) {
            out.write(buf, 0, len);
        }
        in.close();
        out.close();
    }

    public void saveResults(File directory){

        NXNodeMapper tmpNode = null;
        File tmpReduced = null;
        File tmpResults = null;
        Enumeration children = root.children();
        
        while(children.hasMoreElements()){

            tmpNode = (NXNodeMapper)children.nextElement();
            
            if(tmpNode.getReducedFile()!=null){

                tmpReduced = new File(directory.getAbsolutePath() +
                        directory.separator + tmpNode.getReducedFile().getName());
                tmpResults = new File(directory.getAbsolutePath() +
                        directory.separator + tmpNode.getResultsFile().getName());
                try{
                    copy(tmpNode.getReducedFile(),tmpReduced);
                    copy(tmpNode.getResultsFile(),tmpResults);
                } catch (IOException ex) {
                    Logger.getLogger(
                    FileActions.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    public void run() {

        if (which == 1) {
            isNotBulk = true;
            loadFile();
            domTree.updateTree();
            isNotBulk = false;
        } else if (which == 2) {
            isNotBulk = true;
            validateFile();
            domTree.updateTree();
            isNotBulk = false;
        } else if (which == 3) {
            bulkCheck();
        } else if (which == 4) {
            bulkLoad();
        }else if (which == 5) {
            bulkValidate();
        } else if (which == 6) {
            saveResults(saveDirectory);
        }
    }
}
