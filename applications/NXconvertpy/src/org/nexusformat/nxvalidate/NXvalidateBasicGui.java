package org.nexusformat.nxvalidate;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.ToolTipManager;
import javax.swing.UIManager;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeModel;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

public class NXvalidateBasicGui extends JPanel implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2935498610579762249L;
	static private final String newline = "\n";
	JButton browseFileButton;
	JButton validateButton;
	JLabel filenameLabel;
	JTextField filenameText;
	JTextArea log;
	JTree tree;
	JFileChooser fc;
	String reducedNeXusFilename;
	String schematronFile = "schematron.sch";
	NXconvert converter;
	NXschematron schematron;
	Vector<Report> reports;
	NXvalidate validator;

	public NXvalidateBasicGui() {
		super(new GridBagLayout());

		GridBagConstraints c = new GridBagConstraints();

		// Create the log first, because the action listeners
		// need to refer to it.
		log = new JTextArea(5, 20);
		log.setMargin(new Insets(5, 5, 5, 5));
		log.setEditable(false);
		JScrollPane logScrollPane = new JScrollPane(log);

		// Create a file chooser
		fc = new JFileChooser();

		fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);

		// Create the filename label
		filenameLabel = new JLabel("Filename:");

		// Create the filename text field
		filenameText = new JTextField(30);
		filenameText.setToolTipText("The NeXus file to validate");

		// Create the open button.
		browseFileButton = new JButton("Browse...");
		browseFileButton.addActionListener(this);

		// Create the validation button.
		validateButton = new JButton("Perform Validation");
		validateButton.setToolTipText("Perform Validation");
		validateButton.addActionListener(this);

		// For layout purposes, put the buttons in a separate panel
		JPanel filePanel = new JPanel(); // use FlowLayout
		filePanel.add(filenameLabel);
		filePanel.add(filenameText);
		filePanel.add(browseFileButton);

		JPanel validatePanel = new JPanel();
		validatePanel.add(validateButton);

		// Create the tree view
		tree = new JTree(new NXSnode());

		DefaultTreeCellRenderer rend1 = new DefaultTreeCellRenderer();
		// IconAndTipRenderer rend2 = new
		// IconAndTipRenderer(rend1.getOpenIcon(),
		// rend1.getClosedIcon(), rend1.getLeafIcon());
		// tree.setCellRenderer(rend2);
		ToolTipManager.sharedInstance().registerComponent(tree);

		// Add the buttons and the log to this panel.

		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridy = 0;
		c.gridx = 0;
		c.weightx = 0.5;
		c.insets = new Insets(0, 10, 0, 0);
		add(filenameLabel, c);

		c.fill = GridBagConstraints.HORIZONTAL;
		c.insets = new Insets(0, 0, 0, 0);
		c.gridy = 0;
		c.gridx = 1;
		c.weightx = 0.5;
		add(filenameText, c);

		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridy = 0;
		c.gridx = 2;
		c.weightx = 0.5;
		add(browseFileButton, c);

		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridy = 1;
		c.gridx = 0;
		c.weightx = 1.0;
		c.gridwidth = 3;
		add(validateButton, c);

		c.fill = GridBagConstraints.HORIZONTAL;
		c.weightx = 1.0;
		c.gridy = 2;
		add(new JScrollPane(tree), c);

		c.weightx = 0.0;
		c.gridy = 3;
		add(logScrollPane, c);
	}

	public TreeModel parseXML(String filename) throws Exception {
		SAXParserFactory factory = SAXParserFactory.newInstance();
		XMLIconTreeHandler handler = new XMLIconTreeHandler();
		SAXParser saxParser = factory.newSAXParser();
		saxParser.parse(new File(filename), handler);
		return new DefaultTreeModel(handler.getRoot());
	}

	public void actionPerformed(ActionEvent e) {

		// Handle open button action.
		if (e.getSource() == browseFileButton) {
			int returnVal = fc.showOpenDialog(NXvalidateBasicGui.this);

			if (returnVal == JFileChooser.APPROVE_OPTION) {
				File file = fc.getSelectedFile();
				// Set the text box value to show the selected filename.
				filenameText.setText(file.getAbsolutePath());
				log.append("Selected NeXus File: " + file.getName() + "."
						+ newline);
				// TODO move code to make the reduced file here (and also after
				// the filename has been typed).
			} else {
				log.append("Browse command cancelled by user." + newline);
			}
			log.setCaretPosition(log.getDocument().getLength());

			// Handle save button action.
		} else if (e.getSource() == validateButton) {
			// Do the validation
			try {

				validator = new NXvalidate();
				validator.setFilename(filenameText.getText());
				validator.setConvertNxs(true);
				validator.setSchematron(schematronFile);
				validator.setKeepTemp(true);
				validator.process();

				// Get the reports
				reports = validator.getReports();

				// As we only have the ability to pass 1 file at the
				// moment through the GUI - we will throw an exception
				// if we have more than one report.

				if (this.reports.size() > 1) {
					throw new Exception(
							"We only sent a single file to be processed, but have received multiple reports back.");
				}

				// converter = new NXconvert(filenameText.getText(), true);
				// log.append("Converting NeXus file into reduced format."
				// + newline);
				// reducedNeXusFilename = converter.convert();
				// // log.append(filenameText.getText() + " ==> "
				// // + reducedNeXusFilename + newline);
				// log.append("Finished making reduced file." + newline);
				//
				// // Update tree
				// tree.setModel(this.parseXML(reducedNeXusFilename));
				//
				// // create the validation setup
				// log.append("Validating against " + schematronFile + "."
				// + newline);
				// schematron = new NXschematron(reducedNeXusFilename,
				// schematronFile, true);
				// String result = schematron.validate();
				// 
				//
				// // create the report
				// report = new Report(reducedNeXusFilename, result);
				// log.append("There were " + report.numErrors() +
				// " errors found."
				// + newline);
				// // log.append(report.getReport() + newline);
				//			

				log.append("Finished Validating." + newline);

				Report report = reports.get(0);
				tree.setModel(report.getTree());

				log.append("There were " + report.numErrors()
						+ " errors found." + newline);

				Vector<SVRLitem> messages = report.getReport();

				Iterator i = messages.iterator();
				while (i.hasNext()) {
					log.append("->" + i.next() + newline);
				}

			} catch (Exception e1) {
				log.append(e1.getMessage() + newline);
				e1.printStackTrace();
			}

			log.setCaretPosition(log.getDocument().getLength());
		}
	}

	// /** Returns an ImageIcon, or null if the path was invalid. */
	// protected static ImageIcon createImageIcon(String path) {
	// java.net.URL imgURL = NXvalidateBasicGui.class.getResource(path);
	// if (imgURL != null) {
	// return new ImageIcon(imgURL);
	// } else {
	// System.err.println("Couldn't find file: " + path);
	// return null;
	// }
	// }

	/**
	 * Create the GUI and show it. For thread safety, this method should be
	 * invoked from the event dispatch thread.
	 */
	private static void createAndShowGUI() {
		// Create and set up the window.
		JFrame frame = new JFrame("NeXus Validation Tool");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Add content to the window.
		frame.add(new NXvalidateBasicGui());

		// Display the window.
		frame.pack();
		frame.setVisible(true);
	}

	public static void main(String[] args) {
		// Schedule a job for the event dispatch thread:
		// creating and showing this application's GUI.
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				// Turn off metal's use of bold fonts
				UIManager.put("swing.boldMetal", Boolean.FALSE);
				createAndShowGUI();
			}
		});
	}

}