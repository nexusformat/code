/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.nexusformat.nxvalidate;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JPopupMenu;

/**
 *
 * @author Stephen Rankin
 */
public class PopupListener extends MouseAdapter {

    private JPopupMenu popup;

    public PopupListener(JPopupMenu popup) {
        this.popup = popup;
    }

    public void mousePressed(MouseEvent e) {
        maybeShowPopup(e);
    }

    public void mouseReleased(MouseEvent e) {
        maybeShowPopup(e);
    }

    private void maybeShowPopup(MouseEvent e) {
        if (e.isPopupTrigger()) {
            popup.show(e.getComponent(),
                    e.getX(), e.getY());
        }
    }
}
