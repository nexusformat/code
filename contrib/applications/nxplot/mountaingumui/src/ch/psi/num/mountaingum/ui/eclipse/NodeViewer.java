/**
 * This extends NodeEditor with methods for making hardcopy of graphics
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.eclipse;

import java.awt.image.BufferedImage;

import org.eclipse.swt.printing.Printer;

public interface NodeViewer extends NodeEditor {
	/**
	 * draw the graphics into an image
	 * @param width Width of the image
	 * @param height Height of the image
	 * @return A BufferedImage with the graphics in it.
	 */
	public BufferedImage makeImage(int width, int height);
	/**
	 * print the Graphics to the Printer specified.
	 * @param p The printer to print too. 
	 */
	public void print(Printer p);
}
