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
 * TextPaneStyle.java
 *
 */
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

    public TextPaneStyle(JTextPane pane) {
        this.pane = pane;
    }

    private void printValidationErrors(NXNodeMapper node, StyledDocument doc,
            int type) throws BadLocationException {

        String newline = "\n";

        doc.insertString(doc.getLength(), newline + newline,
                    doc.getStyle("heading"));

        if(type == 0){
            doc.insertString(doc.getLength(), "Validation Tests:" +
                         newline + newline, doc.getStyle("heading"));
        }
        else{
            doc.insertString(doc.getLength(), "Warning Tests:" +
                         newline + newline, doc.getStyle("heading"));
        }

        if (node.getNodeTests() != null) {
            for (int i = 0; i < node.getNodeTests().size(); ++i) {
                doc.insertString(doc.getLength(),
                        node.getNodeTests().get(i), doc.getStyle("bold"));
                doc.insertString(doc.getLength(),
                        newline + newline, doc.getStyle("heading"));
            }
        }

        if(type == 0){
            doc.insertString(doc.getLength(), "Validation Errors:" +
                newline + newline, doc.getStyle("errorheading"));
        }
        else{
            doc.insertString(doc.getLength(), "Warning Errors:" +
                newline + newline, doc.getStyle("warningheading"));
        }

        if (node.getNodeTexts() != null) {
            for (int i = 0; i < node.getNodeTexts().size(); ++i) {
               
                if(type == 0){
                    doc.insertString(doc.getLength(),
                        node.getNodeTexts().get(i), doc.getStyle("error"));
                }
                else{
                    doc.insertString(doc.getLength(),
                        node.getNodeTexts().get(i), doc.getStyle("warning"));
                }

                doc.insertString(doc.getLength(),
                        newline + newline, doc.getStyle("heading"));
            }
        }

        if(type == 0){
            doc.insertString(doc.getLength(), "Diagnostic Errors:" +
                newline + newline, doc.getStyle("errorheading"));
        }
        else{
            doc.insertString(doc.getLength(), "Diagnostic Errors:" +
                newline + newline, doc.getStyle("warningheading"));
        }

        if (node.getNodeDiags() != null) {
            for (int i = 0; i < node.getNodeDiags().size(); ++i) {

                if(type == 0){
                    doc.insertString(doc.getLength(),
                        node.getNodeDiags().get(i), doc.getStyle("error"));
                }
                else{
                    doc.insertString(doc.getLength(),
                        node.getNodeDiags().get(i), doc.getStyle("warning"));
                }
                
                doc.insertString(doc.getLength(),
                        newline + newline, doc.getStyle("heading"));
            }
        }

    }

    public void updateTextPane(NXNodeMapper node) {

        String newline = "\n";

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

            if(node.getBadNode()){
                printValidationErrors(node, doc, 0);
            }
            else if(node.getWarnNode()){
                printValidationErrors(node, doc, 1);
            }

        } catch (BadLocationException ex) {
            Logger.getLogger(TextPaneStyle.class.getName()).log(Level.SEVERE,
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

        s = doc.addStyle("warningheading", regular);
        StyleConstants.setFontSize(s, 16);
        StyleConstants.setBold(s, true);
        StyleConstants.setForeground(s, Color.BLUE);

        s = doc.addStyle("warning", regular);
        StyleConstants.setBold(s, true);
        StyleConstants.setForeground(s, Color.BLUE);

    }
}
