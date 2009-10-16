/**
 * This is a child of Graph2DSGT which makes a 
 * quadratic plot.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2007
 */
package ch.psi.num.mountaingum.sans;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.beans.PropertyChangeEvent;

import javax.swing.RepaintManager;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.GraphicsView.Graph2DSGT;
import gov.noaa.pmel.util.Dimension2D;
import gov.noaa.pmel.util.Domain;
import gov.noaa.pmel.util.Range2D;

public class SansViewer extends Graph2DSGT {
	protected Label cog;
	protected COG2D cogCalc;

	public SansViewer(){
		super();
		cogCalc = new COG2D();
	}
	/**
	 * overriden in order to make quadratic image
	 */
	public BufferedImage makeImage(int width, int height) {
		Dimension d = lpl.getSize();
		double sx = (double)width/(double)d.width;
		double sy = (double)height/(double)d.height;
		height = (int)((double)sx*width/sy); 
		BufferedImage im = new BufferedImage(width,height,BufferedImage.TYPE_INT_RGB);
		Graphics2D g = im.createGraphics();
		g.setColor(Color.white);
		g.fillRect(0, 0, width, height);
		g.setColor(Color.black);
		RepaintManager rm = RepaintManager.currentManager(lpl);
		rm.setDoubleBufferingEnabled(false);
		g.scale(sx, sy);
		lpl.draw(g, width,height);
		rm.setDoubleBufferingEnabled(true);
		return im;
	}

	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		Object o = super.makeEditor(uiData, n);
		lpl.setLayerSizeP(new Dimension2D(6.0,6.0));
		return o;
	}

	protected Composite makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);

		Group colGroup = new Group(controlPanel, SWT.SHADOW_ETCHED_IN);
		colGroup.setLayout(new FillLayout(SWT.VERTICAL));
		colGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		colGroup.setText("Colour Mapping");
		colorMaps = new Combo(colGroup,SWT.SINGLE);
		colorMaps.add("Rainbow");
		colorMaps.add("Lyapunov");
		colorMaps.add("Jet");
		colorMaps.select(0);
		colorMaps.addSelectionListener(this);
		
		Group logGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		logGroup.setLayout(new FillLayout(SWT.VERTICAL));
		logGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		logGroup.setText("Data Mapping");
		normal = new Button(logGroup, SWT.RADIO);
		normal.setText("Normal");
		normal.setSelection(true);
		normal.addSelectionListener(this);
		log = new Button(logGroup,SWT.RADIO);
		log.setText("log");
		log.addSelectionListener(this);
				
		Group mapGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		mapGroup.setLayout(new FillLayout(SWT.VERTICAL));
		mapGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		mapGroup.setText("Z Mapping");
		automatic = new Button(mapGroup, SWT.RADIO);
		automatic.setText("Automatic");
		automatic.setSelection(true);
		automatic.addSelectionListener(this);
		manual = new Button(mapGroup,SWT.RADIO);
		manual.setText("Manual");
		manual.addSelectionListener(this);
		
		Group posGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		posGroup.setLayout(new FillLayout(SWT.VERTICAL));
		posGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		posGroup.setText("x,y,I");
		pos = new Label(posGroup,SWT.NONE);
		pos.setText("Unknown");
		pos.setBackground(controlPanel.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		
		Group cogGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		cogGroup.setLayout(new FillLayout(SWT.VERTICAL));
		cogGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		cogGroup.setText("COG x,y, Sum(I)");
		cog = new Label(cogGroup,SWT.NONE);
		cog.setText("Unknown");
		cog.setBackground(controlPanel.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		
		reset = new Button(controlPanel,SWT.PUSH);
		reset.setText("Reset Zoom");
		reset.addSelectionListener(this);
		reset.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		refresh = new Button(controlPanel,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);
		refresh.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		return controlPanel;
	}

	public void makePlot() {
		super.makePlot();
		try{
			calculateCOG();
		}catch(Exception eva){
			/*
			 * can happen if not fully initialized
			 */
		}
	}

	private void calculateCOG() {
		Domain dom = zoomDomain;
		GraphData gv = getData(dataName);
		GraphAxis x, y;
		
		double d[] = gv.getData();
		x = getXAxis();
		int xdim = x.getData().length;
		y = getYAxis();
		int ydim = y.getData().length;
		if(dom == null){
			cogCalc.calculate(d, xdim,ydim);
		} else {
			Range2D r = dom.getXRange();
			int xstart = (int)r.start + xdim/2;
			int xend = (int)r.end + xdim/2;
			r = dom.getYRange();
			int ystart = (int)r.start + ydim/2;
			int yend = (int)r.end + ydim/2;
			cogCalc.calculate(d, xdim,ydim, 
					xstart, xend, ystart, yend);
			
		}
		StringBuffer stb = new StringBuffer();
		stb.append((int)cogCalc.x - xdim/2);
		stb.append(", ");
		stb.append((int)cogCalc.y - ydim/2);
		stb.append(", ");
		stb.append((int)cogCalc.cogTotal);
		
		final String txt = stb.toString();
		cog.getDisplay().asyncExec(new Runnable() {
			public void run() {
				cog.setText(txt);
			}});
	}
	public void propertyChange(PropertyChangeEvent evt) {
		super.propertyChange(evt);
		String name = evt.getPropertyName();
		if(name.equals("domainRange") || name.equals("zoomRectangle")){
			calculateCOG();
		}
	}
}
