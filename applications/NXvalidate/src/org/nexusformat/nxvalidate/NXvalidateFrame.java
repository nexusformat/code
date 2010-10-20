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
  * NXvalidateFrame.java
  *
  * Created on 02-Jun-2010, 14:17:04
  */

package org.nexusformat.nxvalidate;

import java.awt.Dialog.ModalityType;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.help.CSH;
import javax.help.HelpBroker;
import javax.help.HelpSet;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreePath;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;

/**
 *
 * @author Stephen Rankin
 */
public class NXvalidateFrame extends javax.swing.JFrame {

    private DocumentBuilderFactory factory = null;
    private DocumentBuilder builder = null;
    private NXReducedToTree domTree = null;
    private NXNodeMapper root = null;
    private ResourceBundle bundle = null;
    private File nxsFile = null;
    private File nxdlFile = null;
    private File reducedFile = null;
    private File resultsFile = null;
    private TreeUtils treeUtils = null;
    private UserSettings settings = null;
    private String nxconvertCommand = null;
    private File saveDirectory = null;
    private boolean foundNXconvert = false;
    private MouseListener popupListener = null;
    private TextPaneStyle txtStyle = null;
    private FileActions fileLoadingActions = null;
    private HelpBroker hb = null;

    /** Creates new form NXvalidateFrame */
    public NXvalidateFrame() {
        initComponents();
        setup();
    }

    private void setup() {

        factory = DocumentBuilderFactory.newInstance();
        factory.setNamespaceAware(true);

        root = new NXNodeMapper("Files");

        domTree = new NXReducedToTree(root);
        jTree1.setModel(domTree);

        try {
            builder = factory.newDocumentBuilder();
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE, null, ex);
        }

        bundle = ResourceBundle.getBundle(
                "org/nexusformat/nxvalidate/resources/nxvalidate");

        treeUtils = new TreeUtils();

        XMLTreeRenderer rend = new XMLTreeRenderer(
                "resources/nexus32x32.png", "resources/bad.png",
                "resources/good.png", "resources/warn.png",
                "resources/peg-e.png", "resources/info.png",
                "resources/info-good.png", "resources/info-bad.png",
                "resources/info-warn.png");

        jTree1.setCellRenderer(rend);

        settings = new UserSettings();

        try {
            settings.loadUserSettings();
            nxconvertCommand = settings.getNXconvert();

            if (!settings.foundNXconvert()) {
                displayErrorMessage(
                        bundle.getString("nxconvertMissingError"));
                foundNXconvert = false;
            } else {
                foundNXconvert = true;
            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    "The settings file cannot be found.", ex);
        } catch (IOException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    "The settings file IO error.", ex);
        }

        popupListener = new PopupListener(treePopupMenu);
        jTree1.addMouseListener(popupListener);

        txtStyle = new TextPaneStyle(jTextPane1);

        fileLoadingActions = new FileActions(this, jTree1, builder, domTree, root);

        String helpHS = "help/jhelpset.hs";
        HelpSet  hs = null;

        try {
            hs = new HelpSet(null, this.getClass().getResource(helpHS));
        } catch (Exception ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    "The setting help file IO error.", ex);
            return;
        }
        // Create a HelpBroker object:
        hb = hs.createHelpBroker();

        helpMenuItem.addActionListener(new CSH.DisplayHelpFromSource( hb ));

    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        dialogReportProblem = new javax.swing.JOptionPane();
        treePopupMenu = new javax.swing.JPopupMenu();
        closeFileMenuItem = new javax.swing.JMenuItem();
        jFileChooser1 = new javax.swing.JFileChooser();
        jPanel2 = new javax.swing.JPanel();
        jSplitPane1 = new javax.swing.JSplitPane();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTree1 = new javax.swing.JTree();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextPane1 = new javax.swing.JTextPane();
        jMenuBar1 = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        openFilesMenuItem = new javax.swing.JMenuItem();
        jSeparator4 = new javax.swing.JPopupMenu.Separator();
        saveMenuItem = new javax.swing.JMenuItem();
        jSeparator7 = new javax.swing.JPopupMenu.Separator();
        closeAllMenuItem = new javax.swing.JMenuItem();
        jSeparator5 = new javax.swing.JPopupMenu.Separator();
        exitAppMenuItem = new javax.swing.JMenuItem();
        toolsMenu = new javax.swing.JMenu();
        validateSelectedMenuItem = new javax.swing.JMenuItem();
        jSeparator2 = new javax.swing.JPopupMenu.Separator();
        filterMenuItem = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JPopupMenu.Separator();
        bulkMenuItem = new javax.swing.JMenuItem();
        jSeparator3 = new javax.swing.JPopupMenu.Separator();
        settingsMenuItem = new javax.swing.JMenuItem();
        helpMenu = new javax.swing.JMenu();
        helpMenuItem = new javax.swing.JMenuItem();
        jSeparator6 = new javax.swing.JPopupMenu.Separator();
        aboutMenuItem = new javax.swing.JMenuItem();

        closeFileMenuItem.setText("Close File");
        closeFileMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                closeFileMenuItemActionPerformed(evt);
            }
        });
        treePopupMenu.add(closeFileMenuItem);

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        java.util.ResourceBundle bundle = java.util.ResourceBundle.getBundle("org/nexusformat/nxvalidate/resources/nxvalidate"); // NOI18N
        setTitle(bundle.getString("applicationTitle")); // NOI18N
        setMinimumSize(new java.awt.Dimension(400, 400));

        jPanel2.setBorder(javax.swing.BorderFactory.createCompoundBorder(javax.swing.BorderFactory.createEmptyBorder(5, 5, 5, 5), javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.LOWERED)));
        jPanel2.setPreferredSize(new java.awt.Dimension(130, 130));

        jSplitPane1.setDividerLocation(200);
        jSplitPane1.setContinuousLayout(true);

        javax.swing.tree.DefaultMutableTreeNode treeNode1 = new javax.swing.tree.DefaultMutableTreeNode("NXS Files");
        jTree1.setModel(new javax.swing.tree.DefaultTreeModel(treeNode1));
        jTree1.addTreeSelectionListener(new javax.swing.event.TreeSelectionListener() {
            public void valueChanged(javax.swing.event.TreeSelectionEvent evt) {
                jTree1ValueChanged(evt);
            }
        });
        jScrollPane1.setViewportView(jTree1);

        jSplitPane1.setLeftComponent(jScrollPane1);

        jScrollPane2.setViewportView(jTextPane1);

        jSplitPane1.setRightComponent(jScrollPane2);

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSplitPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 386, Short.MAX_VALUE)
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSplitPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 259, Short.MAX_VALUE)
        );

        getContentPane().add(jPanel2, java.awt.BorderLayout.CENTER);

        fileMenu.setText(bundle.getString("fileMenuItem")); // NOI18N

        openFilesMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F, java.awt.event.InputEvent.CTRL_MASK));
        openFilesMenuItem.setText("Open Files");
        openFilesMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openFilesMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(openFilesMenuItem);
        fileMenu.add(jSeparator4);

        saveMenuItem.setText("Save Results");
        saveMenuItem.setToolTipText("Save results to files.");
        saveMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                saveMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(saveMenuItem);
        fileMenu.add(jSeparator7);

        closeAllMenuItem.setText("Close All Files");
        closeAllMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                closeAllMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(closeAllMenuItem);
        fileMenu.add(jSeparator5);

        exitAppMenuItem.setText("Exit");
        exitAppMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                exitAppMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(exitAppMenuItem);

        jMenuBar1.add(fileMenu);

        toolsMenu.setText("Tools");

        validateSelectedMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_V, java.awt.event.InputEvent.CTRL_MASK));
        validateSelectedMenuItem.setText("Validate Selected");
        validateSelectedMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                validateSelectedMenuItemActionPerformed(evt);
            }
        });
        toolsMenu.add(validateSelectedMenuItem);
        toolsMenu.add(jSeparator2);

        filterMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_G, java.awt.event.InputEvent.CTRL_MASK));
        filterMenuItem.setText("Show Failed Nodes");
        filterMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                filterMenuItemActionPerformed(evt);
            }
        });
        toolsMenu.add(filterMenuItem);
        toolsMenu.add(jSeparator1);

        bulkMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_B, java.awt.event.InputEvent.CTRL_MASK));
        bulkMenuItem.setText("Bulk Validation");
        bulkMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bulkMenuItemActionPerformed(evt);
            }
        });
        toolsMenu.add(bulkMenuItem);
        toolsMenu.add(jSeparator3);

        settingsMenuItem.setText("Settings");
        settingsMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                settingsMenuItemActionPerformed(evt);
            }
        });
        toolsMenu.add(settingsMenuItem);

        jMenuBar1.add(toolsMenu);

        helpMenu.setText("Help");

        helpMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_H, java.awt.event.InputEvent.CTRL_MASK));
        helpMenuItem.setText("Help Content");
        helpMenu.add(helpMenuItem);
        helpMenu.add(jSeparator6);

        aboutMenuItem.setText("About");
        aboutMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                aboutMenuItemActionPerformed(evt);
            }
        });
        helpMenu.add(aboutMenuItem);

        jMenuBar1.add(helpMenu);

        setJMenuBar(jMenuBar1);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    public void displayErrorMessage(String message) {

        JTextArea ta = new JTextArea(message);
        ta.setEditable(false);
        ta.setWrapStyleWord(true);
        ta.setLineWrap(true);
        ta.setSize(200, 100);
        dialogReportProblem.showMessageDialog(this, ta);

    }

    private boolean loadOpenFilesDialog() {

        NXLoadFilesDialog loadFile = new NXLoadFilesDialog(this, true);
        loadFile.setModalityType(ModalityType.APPLICATION_MODAL);
        loadFile.setVisible(true);
        nxsFile = loadFile.getNXSFile();
        nxdlFile = loadFile.getNXDLFile();
        return loadFile.OKButtonUsed();

    }

    private boolean saveResultsFilesDialog() {

        NXLoadFilesDialog loadFile = new NXLoadFilesDialog(this, true);
        loadFile.setModalityType(ModalityType.APPLICATION_MODAL);
        loadFile.setVisible(true);
        nxsFile = loadFile.getNXSFile();
        nxdlFile = loadFile.getNXDLFile();
        return loadFile.OKButtonUsed();

    }

    private boolean loadValidatFileDialog() {

        NXValidateDialog loadValidate = new NXValidateDialog(this, true);
        loadValidate.setModalityType(ModalityType.APPLICATION_MODAL);
        if (nxdlFile != null) {
            loadValidate.setNXDLFile(nxdlFile);
        }
        loadValidate.setVisible(true);
        nxdlFile = loadValidate.getNXDLFile();
        return loadValidate.OKButtonUsed();

    }

    private boolean loadSettingsFileDialog() {

        NXSettingsDialog loadSettings = new NXSettingsDialog(this, true);
        loadSettings.setModalityType(ModalityType.APPLICATION_MODAL);
        if (nxconvertCommand != null) {
            loadSettings.setNXConvertCommand(nxconvertCommand);
        }
        loadSettings.setVisible(true);
        nxconvertCommand = loadSettings.getNXConvertFile();
        return loadSettings.OKButtonUsed();

    }

    private void openFilesMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openFilesMenuItemActionPerformed

        if (evt.getSource() == openFilesMenuItem) {
            if (!foundNXconvert) {
                displayErrorMessage(
                        bundle.getString("nxconvertMissingError"));
                return;
            }

            boolean result = loadOpenFilesDialog();

            if (result) {
                if (nxsFile != null) {
                    fileLoadingActions.setWhich(1);
                    fileLoadingActions.setNXSFile(nxsFile);
                    fileLoadingActions.setNXConvertFile(nxconvertCommand);
                    fileLoadingActions.setNXDLFile(nxdlFile);
                    (new Thread(fileLoadingActions)).start();
                } else {
                    dialogReportProblem.showMessageDialog(this,
                            bundle.getString("openNexusFileMessage"));
                }
            }
        }

    }//GEN-LAST:event_openFilesMenuItemActionPerformed

    private void jTree1ValueChanged(javax.swing.event.TreeSelectionEvent evt) {//GEN-FIRST:event_jTree1ValueChanged

        if (evt.getSource() == jTree1) {
            if (jTree1.getSelectionPath() != null) {
                NXNodeMapper node =
                        (NXNodeMapper) jTree1.getSelectionPath().getLastPathComponent();
                txtStyle.updateTextPane(node);
            }
        }

    }//GEN-LAST:event_jTree1ValueChanged

    private void validateSelectedMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_validateSelectedMenuItemActionPerformed

        if (evt.getSource() == validateSelectedMenuItem) {

            //Nothing selected in tree
            if (jTree1.isSelectionEmpty()) {
                return;
            }

            //Root node selected in tree
            TreePath selected = jTree1.getSelectionPath();
            int size = selected.getPath().length;
            NXNodeMapper endNode = (NXNodeMapper) selected.getPath()[size - 1];
            if (endNode.isRoot()) {
                return;
            }

            //No nxconvert binary found.
            if (!foundNXconvert) {
                displayErrorMessage(
                        bundle.getString("nxconvertMissingError"));
                return;
            }

            Document reducedDoc = null;
            Document resultsDoc = null;

            //Check to see if the reduction and validation have not already
            //been done, i.e. the files and documents already exist for the
            //current selected tree.
            if (treeUtils.getNXDLFile(jTree1) != null) {
                nxdlFile = treeUtils.getNXDLFile(jTree1);
            }

            if (treeUtils.getReducedFile(jTree1) != null) {
                reducedFile = treeUtils.getReducedFile(jTree1);
                reducedDoc = treeUtils.getReducedDoc(jTree1);
            }

            if (treeUtils.getResultsFile(jTree1) != null) {
                resultsFile = treeUtils.getResultsFile(jTree1);
                resultsDoc = treeUtils.getResultsDoc(jTree1);
            }

            //Show the validate file dialog.
            boolean result = loadValidatFileDialog();

            if (result) {
                if (nxdlFile != null) {

                    fileLoadingActions.setWhich(2);
                    fileLoadingActions.setNXSFile(nxsFile);
                    fileLoadingActions.setNXConvertFile(nxconvertCommand);
                    fileLoadingActions.setNXDLFile(nxdlFile);
                    fileLoadingActions.setReducedFile(reducedFile);
                    fileLoadingActions.setReducedDoc(reducedDoc);
                    fileLoadingActions.setResultsDoc(resultsDoc);

                    Thread thread = new Thread(fileLoadingActions);

                    thread.start();

                } else {
                    dialogReportProblem.showMessageDialog(this,
                            bundle.getString("openSchemaFileMessage"));
                }
            }
        }

    }//GEN-LAST:event_validateSelectedMenuItemActionPerformed

    private void settingsMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_settingsMenuItemActionPerformed

        if (evt.getSource() == settingsMenuItem) {

            boolean result = loadSettingsFileDialog();

            if (result) {
                if (nxconvertCommand != null) {
                    
                    settings.setNXconvert(nxconvertCommand);
                    settings.saveUserSettings();
                    foundNXconvert = settings.foundNXconvert();
                }
            }

            if (!foundNXconvert) {
                displayErrorMessage(
                        bundle.getString("nxconvertMissingError"));
                return;
            }

        }

    }//GEN-LAST:event_settingsMenuItemActionPerformed

    private void closeFileMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeFileMenuItemActionPerformed

        if (evt.getSource() == closeFileMenuItem) {

            NXNodeMapper node = treeUtils.getBaseNode(jTree1);
            if (node != null) {
                if (!node.isRoot()) {
                    root.remove(node);
                    domTree.removeNodeFromParent((MutableTreeNode) node);
                    domTree.updateTree();
                }
            }

        }
    }//GEN-LAST:event_closeFileMenuItemActionPerformed

    private void exitAppMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_exitAppMenuItemActionPerformed
        if (evt.getSource() == exitAppMenuItem) {
            this.dispose();
        }
    }//GEN-LAST:event_exitAppMenuItemActionPerformed

    private void closeAllMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeAllMenuItemActionPerformed
        if (evt.getSource() == closeAllMenuItem) {

            int result = dialogReportProblem.showConfirmDialog(this,
                    bundle.getString("closeAllWarningMessage"), null,
                    JOptionPane.OK_CANCEL_OPTION);

            if (result == 0) {
                ArrayList<NXNodeMapper> nodes = root.getOpenNodes();
                for (int i = 0; i < nodes.size(); ++i) {
                    domTree.removeNodeFromParent((MutableTreeNode) nodes.get(i));
                }
                root.removeAllNodes();
                domTree.updateTree();
            }
        }
    }//GEN-LAST:event_closeAllMenuItemActionPerformed

    private void bulkMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_bulkMenuItemActionPerformed

        if (evt.getSource() == bulkMenuItem) {

            fileLoadingActions.setNXConvertFile(nxconvertCommand);
            BulkLoadFilesFrame bulk = new BulkLoadFilesFrame(fileLoadingActions);
            bulk.setVisible(true);

        }

    }//GEN-LAST:event_bulkMenuItemActionPerformed

    private void filterMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_filterMenuItemActionPerformed

        if (evt.getSource() == filterMenuItem) {
            treeUtils.hideGoodNodes(jTree1);
        }

    }//GEN-LAST:event_filterMenuItemActionPerformed

    private void saveMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveMenuItemActionPerformed
       if (evt.getSource() == saveMenuItem) {

            jFileChooser1.setMultiSelectionEnabled(false);
            jFileChooser1.setFileSelectionMode(jFileChooser1.DIRECTORIES_ONLY);
            jFileChooser1.setApproveButtonText("Save");
            int returnVal = jFileChooser1.showOpenDialog(this);

            if (returnVal == jFileChooser1.APPROVE_OPTION) {

                saveDirectory = jFileChooser1.getSelectedFile();

                if(!saveDirectory.exists()){
                    saveDirectory.mkdir();
                }

                fileLoadingActions.setSaveDirectory(saveDirectory);
                fileLoadingActions.setWhich(6);
                Thread thread = new Thread(fileLoadingActions);
                thread.start();

                dialogReportProblem.showMessageDialog(
                    this,
                    bundle.getString("savedResultsMessage"));

            } else {
                saveDirectory = null;
            }

        }
    }//GEN-LAST:event_saveMenuItemActionPerformed

    private void aboutMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_aboutMenuItemActionPerformed
        if (evt.getSource() ==  aboutMenuItem) {
            AboutDialog about = new AboutDialog(this,true);
            about.setVisible(true);
        }
    }//GEN-LAST:event_aboutMenuItemActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new NXvalidateFrame().setVisible(true);
            }
        });


    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem aboutMenuItem;
    private javax.swing.JMenuItem bulkMenuItem;
    private javax.swing.JMenuItem closeAllMenuItem;
    private javax.swing.JMenuItem closeFileMenuItem;
    private javax.swing.JOptionPane dialogReportProblem;
    private javax.swing.JMenuItem exitAppMenuItem;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JMenuItem filterMenuItem;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JMenuItem helpMenuItem;
    private javax.swing.JFileChooser jFileChooser1;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JPopupMenu.Separator jSeparator1;
    private javax.swing.JPopupMenu.Separator jSeparator2;
    private javax.swing.JPopupMenu.Separator jSeparator3;
    private javax.swing.JPopupMenu.Separator jSeparator4;
    private javax.swing.JPopupMenu.Separator jSeparator5;
    private javax.swing.JPopupMenu.Separator jSeparator6;
    private javax.swing.JPopupMenu.Separator jSeparator7;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JTextPane jTextPane1;
    private javax.swing.JTree jTree1;
    private javax.swing.JMenuItem openFilesMenuItem;
    private javax.swing.JMenuItem saveMenuItem;
    private javax.swing.JMenuItem settingsMenuItem;
    private javax.swing.JMenu toolsMenu;
    private javax.swing.JPopupMenu treePopupMenu;
    private javax.swing.JMenuItem validateSelectedMenuItem;
    // End of variables declaration//GEN-END:variables
}
