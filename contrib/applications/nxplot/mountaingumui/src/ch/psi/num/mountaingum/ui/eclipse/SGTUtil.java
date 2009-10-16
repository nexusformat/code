/**
 * A class with static uitility function for the SGT graphics toolkit.
 * 
 * copyrght: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.eclipse;

import gov.noaa.pmel.sgt.JPane;

import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.RepaintManager;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.printing.Printer;

import ch.psi.num.mountaingum.ui.util.SWTUtil;

public class SGTUtil {
	public static BufferedImage drawInImage(JPane pane, int width, int height){
		BufferedImage im = new BufferedImage(width,height,BufferedImage.TYPE_INT_RGB);
		Graphics2D g = im.createGraphics();
		RepaintManager rm = RepaintManager.currentManager(pane);
		rm.setDoubleBufferingEnabled(false);
		Dimension d = pane.getSize();
		double sx = (double)width/(double)d.width;
		double sy = (double)height/(double)d.height;
		g.scale(sx, sy);
		pane.draw(g, width,height);
		rm.setDoubleBufferingEnabled(true);
		return im;
	}
	public static void print(JPane plot, Printer p){
		int width, height;
		boolean test, rotate;
		
		test = p.startJob("Mountaingum");
		test = p.startPage();
		GC gc = new GC(p);
		Rectangle r = p.getClientArea();
		if(r.height > r.width){
			width = r.height;
			height = r.width;
			rotate = true;
		} else {
			width = r.width;
			height = r.height;
			rotate = false;
		}
		Rectangle trim = p.computeTrim(0, 0, 0, 0); 
		BufferedImage bf = drawInImage(plot, width, height);
		ImageData id = SWTUtil.convertToSWT(bf);
		if(rotate){
			id = SWTUtil.rotate(id, SWT.LEFT);
		}
		Image im = new Image(p,id);
		gc.drawImage(im,0,0,id.width,id.height,
				-trim.x, -trim.y, id.width, id.height);
		im.dispose();
		gc.dispose();
		p.endPage();
		p.endJob();
		bf.flush();
	}
}
