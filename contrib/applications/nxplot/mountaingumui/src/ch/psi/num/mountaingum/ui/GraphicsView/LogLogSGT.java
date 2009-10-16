/**
 * This is a JPane class specialized for displaying a double 
 * logarithmic plot. This is currently restricted to one 
 * line plot. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Rectangle;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import gov.noaa.pmel.sgt.CartesianGraph;
import gov.noaa.pmel.sgt.JPane;
import gov.noaa.pmel.sgt.Layer;
import gov.noaa.pmel.sgt.LineAttribute;
import gov.noaa.pmel.sgt.LogAxis;
import gov.noaa.pmel.sgt.LogTransform;
import gov.noaa.pmel.sgt.SGLabel;
import gov.noaa.pmel.sgt.StackedLayout;
import gov.noaa.pmel.sgt.dm.SGTData;
import gov.noaa.pmel.util.Dimension2D;
import gov.noaa.pmel.util.Point2D;
import gov.noaa.pmel.util.Range2D;
import gov.noaa.pmel.util.SoTPoint;
import gov.noaa.pmel.util.SoTRange;

public class LogLogSGT extends JPane 
				implements PropertyChangeListener{
	public final static long serialVersionUID = 127127;
	
	protected Layer lay;
	protected double xstart, xsize, xend;
	protected double ysize, ystart, yend;
	protected LogAxis xbot, yleft;
	protected CartesianGraph graph;
    protected SoTRange datax, datay; 
	protected LogTransform xt, yt;
	protected boolean inZoom; 
	protected Range2D zoomXR, zoomYR;
	
	public LogLogSGT(){
		super("LogLog", new Dimension(600,400));
		setLayout(new StackedLayout());
		setBackground(Color.white);
		lay = new Layer("LogLayer", new Dimension2D(6.0,4.0));
		add(lay);
		xstart = .6;
		xsize = 6.;
		xend = 5.4;
		ysize = 4.;
		ystart = .6;
		yend = 3.4;
		graph = null;
		inZoom = false;
		addPropertyChangeListener(this);
	}
	
	public void addData(SGTData data, String title,
			String xtitle, String ytitle){
		SoTRange xrange, yrange;
		SoTRange.Double dr; 
		Font xtfont;
		
		/*
		 * try to fix empty ranges into something sensible
		 */
		xrange = data.getXRange();
		try{
			dr = (SoTRange.Double)xrange;
			if(Math.abs(dr.end - dr.start) < 1.){
				dr.end += 1.;
			}
		}catch(Exception eva){}
		yrange = data.getYRange();
		try{
			dr = (SoTRange.Double)yrange;
			if(Math.abs(dr.end - dr.start) < 1.){
				dr.end += 1.;
			}
		}catch(Exception eva){}

		xtfont = new Font("Helvetica", Font.PLAIN,14);
		datax = xrange;
		datay = yrange;
		
		graph = new CartesianGraph(title);
		lay.setGraph(graph);
		
		xt = new LogTransform(new Range2D(xstart, xend), xrange);
		yt = new LogTransform(new Range2D(ystart, yend), yrange);
		graph.setXTransform(xt);
		graph.setYTransform(yt);
		
		xbot = new LogAxis(xtitle);
		xbot.setRangeU(xrange);
		xbot.setLocationU(
				new SoTPoint(xrange.getStart(), yrange.getStart()));
		xbot.setLabelFont(xtfont);
		SGLabel xtit = new SGLabel("xax", xtitle, 
				new Point2D.Double(0.,0.));
		xtit.setHeightP(.2);
		xbot.setTitle(xtit);
		graph.addXAxis(xbot);

		yleft = new LogAxis(ytitle);
		yleft.setRangeU(yrange);
		yleft.setLocationU(
				new SoTPoint(xrange.getStart(), yrange.getStart()));
		yleft.setLabelFont(xtfont);
		SGLabel ytit = new SGLabel("yax", ytitle, 
				new Point2D.Double(0.,0.));
		ytit.setHeightP(.2);
		yleft.setTitle(ytit);
		graph.addYAxis(yleft);
		
		LineAttribute lattr = new LineAttribute(
				LineAttribute.SOLID, Color.blue);
		graph.setData(data, lattr);
		if(inZoom){
			setZoomRange(zoomXR, zoomYR);
		}
	}

	/**
	 * This is for implementing zoom support. This follows very closely
	 * to the documentation given for SGT.
	 * @param evt The PropertyChangeEvent which says zoom is ON
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		double tmp;
		
		String name = evt.getPropertyName();
		if(name.equals("zoomRectangle")){
			Range2D xr = new Range2D();
			Range2D yr = new Range2D();
			Rectangle zm = (Rectangle)evt.getNewValue();
			if(zm.width <= 1 || zm.height <= 1 || graph == null){
				return;
			}
			
			xr.start = graph.getXPtoU(lay.getXDtoP(zm.x));
			xr.end = graph.getXPtoU(lay.getXDtoP(zm.x + zm.width));
			if(xr.start > xr.end){
				tmp = xr.start;
				xr.start = xr.end;
				xr.end = tmp;
			}

			yr.start = graph.getYPtoU(lay.getYDtoP(zm.y));
			yr.end = graph.getYPtoU(lay.getYDtoP(zm.y + zm.height));
			if(yr.start > yr.end){
				tmp = yr.start;
				yr.start = yr.end;
				yr.end = tmp;
			}
			setZoomRange(xr,yr);
			zoomXR = xr;
			zoomYR = yr;
			inZoom = true;
		}
	}
	private void setZoomRange(Range2D xr, Range2D yr) {
		setBatch(true);
		xt.setRangeU(xr);
		yt.setRangeU(yr);
		
		Point2D.Double ori = new Point2D.Double(xr.start,yr.start);
		xbot.setRangeU(xr);
		xbot.setLocationU(ori);
		yleft.setRangeU(yr);
		yleft.setLocationU(ori);
		
		graph.setClip(xr.start, xr.end, yr.start, yr.end);
		setBatch(false);
	}

	public void resetZoom(){
		setBatch(true);
		inZoom = false;
		
		xt.setRangeU(datax);
		xbot.setRangeU(datax);
		xbot.setLocationU(
				new SoTPoint(datax.getStart(), datay.getStart()));
		yt.setRangeU(datay);
		yleft.setRangeU(datay);
		yleft.setLocationU(
				new SoTPoint(datax.getStart(), datay.getStart()));
		
		graph.setClip(datax, datay);

		setBatch(false);
	}
}
