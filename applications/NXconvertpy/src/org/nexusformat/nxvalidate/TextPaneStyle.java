package org.nexusformat.nxvalidate;

import java.awt.Color;
import java.util.logging.Level;
import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 *
 * @author Stephen Rankin
 */
public class TextPaneStyle {

    private JTextPane pane = null;

    public TextPaneStyle(JTextPane pane){
        this.pane = pane;
    }

    public void updateTextPane(NXNodeMapper node) {

        
        String newline = "\n";
        String[] initString = {
            "another ", //italic
            "styled ", //bold
            "text ", //small
            "component, ", //large
            "which supports embedded components..." + newline,//regular
            " " + newline, //button
            "...and embedded icons..." + newline, //regular
            " ", //icon
            newline + "JTextPane is a subclass of JEditorPane that "
            + "uses a StyledEditorKit and StyledDocument, and provides "
            + "cover methods for interacting with those objects."
        };

        String[] initStyles = {"regular", "italic", "bold", "small", "large",
            "regular", "button", "regular", "icon",
            "regular"
        };

        StyledDocument doc = pane.getStyledDocument();

        addStylesToDocument(doc);
        String[] atts = node.getAttributeList();

        try {

            doc.remove(0, doc.getLength());

            doc.insertString(0, node.toString()
                    + newline + newline, doc.getStyle("title"));

            doc.insertString(doc.getLength(), "Attributes:"
                    + newline + newline, doc.getStyle("heading"));

            for (int i = 0; i
                    < atts.length; i++) {
                doc.insertString(doc.getLength(),
                        "@ " + atts[i] + newline, doc.getStyle("bold"));
            }

            doc.insertString(doc.getLength(), newline + newline,
                    doc.getStyle("heading"));

            doc.insertString(doc.getLength(), "Node Value:"
                    + newline + newline, doc.getStyle("heading"));

            doc.insertString(doc.getLength(),
                    node.getValue(), doc.getStyle("bold"));

            doc.insertString(doc.getLength(), newline + newline,
                    doc.getStyle("heading"));

            doc.insertString(doc.getLength(), "Validation Tests:"
                    + newline + newline, doc.getStyle("heading"));



            if (node.getNodeTests() != null) {

                for (int i = 0; i
                        < node.getNodeTests().size();
                        ++i) {
                    doc.insertString(doc.getLength(),
                            node.getNodeTests().get(i), doc.getStyle("bold"));

                    doc.insertString(doc.getLength(), newline + newline,
                            doc.getStyle("heading"));


                }

            }

            doc.insertString(doc.getLength(), "Validation Errors:"
                    + newline + newline, doc.getStyle("errorheading"));

            if (node.getNodeTexts() != null) {

                for (int i = 0; i
                        < node.getNodeTexts().size();
                        ++i) {
                    doc.insertString(doc.getLength(),
                            node.getNodeTexts().get(i), doc.getStyle("error"));

                    doc.insertString(doc.getLength(), newline + newline,
                            doc.getStyle("heading"));


                }

            }

            doc.insertString(doc.getLength(), "Diagnostic Errors:"
                    + newline + newline, doc.getStyle("errorheading"));

            if (node.getNodeDiags() != null) {

                for (int i = 0; i
                        < node.getNodeDiags().size();
                        ++i) {
                    doc.insertString(doc.getLength(),
                            node.getNodeDiags().get(i), doc.getStyle("error"));

                    doc.insertString(doc.getLength(), newline + newline,
                            doc.getStyle("heading"));

                }

            }

        } catch (BadLocationException ex) {
            Logger.getLogger(NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    null, ex);
        }

    }

    private void addStylesToDocument(StyledDocument doc) {

        //Initialize some styles.
        Style def = StyleContext.getDefaultStyleContext().
                getStyle(StyleContext.DEFAULT_STYLE);

        Style regular = doc.addStyle("regular", def);
        StyleConstants.setFontFamily(def, "SansSerif");

        Style s = doc.addStyle("italic", regular);
        StyleConstants.setItalic(s, true);

        s = doc.addStyle("bold", regular);
        StyleConstants.setBold(s, true);

        s = doc.addStyle("small", regular);
        StyleConstants.setFontSize(s, 10);

        s = doc.addStyle("large", regular);
        StyleConstants.setFontSize(s, 16);

        s = doc.addStyle("heading", regular);
        StyleConstants.setFontSize(s, 16);
        StyleConstants.setBold(s, true);

        s = doc.addStyle("title", regular);
        StyleConstants.setFontSize(s, 24);
        StyleConstants.setBold(s, true);

        s = doc.addStyle("errorheading", regular);
        StyleConstants.setFontSize(s, 16);
        StyleConstants.setBold(s, true);
        StyleConstants.setForeground(s, Color.red);

        s = doc.addStyle("error", regular);
        StyleConstants.setBold(s, true);
        StyleConstants.setForeground(s, Color.red);

    }

}
