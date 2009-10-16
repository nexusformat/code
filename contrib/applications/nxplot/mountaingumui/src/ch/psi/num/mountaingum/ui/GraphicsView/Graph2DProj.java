/**
 * This is a version of a 2D graphics display which has a 
 * projection of the displayed area displayed in a second plot 
 * below the master plot. The projection can be switched to 
 * be on either the x or y axis. After a zoom, only the 
 * zoomed area is represented in the projection.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June-July 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import gov.noaa.pmel.sgt.GridAttribute;
import gov.noaa.pmel.sgt.LineAttribute;
import gov.noaa.pmel.sgt.dm.SGTMetaData;
import gov.noaa.pmel.sgt.dm.SimpleGrid;
import gov.noaa.pmel.sgt.dm.SimpleLine;
import gov.noaa.pmel.sgt.swing.JPlotLayout;
import gov.noaa.pmel.util.Dimension2D;
import gov.noaa.pmel.util.Domain;
import gov.noaa.pmel.util.Range2D;

import java.awt.Color;
import java.awt.Frame;
import java.awt.EventQueue;
import java.awt.Rectangle;
import java.beans.PropertyChangeEvent;
import java.io.IOException;

import org.eclipse.swt.SWT;
import org.eclipse.swt.awt.SWT_AWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.DataRange;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.util.Array2DUtil;
import ch.psi.num.mountaingum.util.ArrayTools;

public class Graph2DProj extends Graph2DSGTBase implements DisposeListener {
	/*
	 * for the 2D plot
	 */
	protected Frame embed2DFrame;
	protected Composite embed2DComposite;
	/* 
	 * for the projection
	 */
	protected Frame embedProj;
	protected Composite embedProjComposite;
	protected SGTLineLayout projPlot;
	protected Button xSelected, ySelected;
	protected double projData[], projAxis[];
	protected Array2DUtil ute;
	/*
	 * axis modes
	 */
	protected static final int XAXIS = 0;
	protected static final int YAXIS = 1;
	protected int axisMode;
	
	/*
	 * Siighhh! The dying flag is used 
	 * to prevent an exception while closing
	 * GTSE
	 */
	protected boolean dying;
	protected boolean starting;
	
	public Graph2DProj(){
		logData = false; 
		manualRange = false;
		mapName = "Rainbow";
		zoomDomain = null;
		map = null;
		axisMode = XAXIS;
		ute = new Array2DUtil();
		dying = false;
		starting = true;
	}
	
	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		graphNode = n;
		lpl = new JPlotLayout(true,false,false,"MountainGumPlot", null,true);
		lpl.addPropertyChangeListener(this);
		collectData();
		
		/*
		 * a Container with a Gridlayout with two rows
		 */
		Composite container = new Composite((Composite)uiData, SWT.NONE);
		container.addDisposeListener(this);
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 2;
		container.setLayout(gritti);
		
		/*
		 * The left row gets a sash form
		 */
		SashForm saskia = new SashForm(container, SWT.VERTICAL);
		saskia.setLayoutData(new GridData(GridData.FILL_BOTH));
		
		/*
		 * This is the 2D graph in the sash
		 */
		Composite graphContainer = new Composite(saskia, SWT.NONE);
		graphContainer.setLayout(new GridLayout());
		embed2DComposite = new Composite(graphContainer, SWT.EMBEDDED);
		embed2DComposite.setLayoutData(new GridData(GridData.FILL_BOTH));
		embed2DFrame = SWT_AWT.new_Frame(embed2DComposite); 
		lpl.setTitles(graphNode.getName(), "", "");
		lpl.addMouseMotionListener(this);
		embed2DFrame.add(lpl);
		makeRangeSelection(graphContainer);
		
		/*
		 * This is the projection graph in the sash
		 */
		Composite projContainer = new Composite(saskia, SWT.NONE);
		projContainer.setLayout(new GridLayout());
		embedProjComposite = new Composite(projContainer, SWT.EMBEDDED);
		embedProjComposite.setLayoutData(new GridData(GridData.FILL_BOTH));
		embedProj = SWT_AWT.new_Frame(embedProjComposite);
		projPlot = new SGTLineLayout("ProjPlot", null, false);
		projPlot.setLayerSizeP(new Dimension2D(8.,3.0));
		embedProj.add(projPlot);
		
		int w[] = new int[2];
		w[0] = 60;
		w[1] = 32;
		saskia.setWeights(w);
		
		/*
		 * This makes the controls in the second, 
		 * right row of the GridLayout
		 */
		makeControls(container);
		
		/*
		 * try make the plot.
		 */
		makePlot();
		lpl.resetZoom();
		return container;
	}
	
	protected Composite makeControls(Composite container) {
		Composite controlPanel = new Composite(container,SWT.NONE);
		controlPanel.setLayoutData(new GridData(GridData.FILL_VERTICAL));
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 1;
		controlPanel.setLayout(gritti);

        make2DControls(controlPanel);

        makeProjectionControl(controlPanel);

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

	protected void makeProjectionControl(Composite controlPanel){
		Group projGroup = new Group(controlPanel,SWT.SHADOW_ETCHED_IN);
		projGroup.setLayout(new FillLayout(SWT.VERTICAL));
		projGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		projGroup.setText("Projection Axis");
		xSelected = new Button(projGroup, SWT.RADIO);
		xSelected.setText("X");
		xSelected.setSelection(true);
		xSelected.addSelectionListener(this);
		ySelected = new Button(projGroup,SWT.RADIO);
		ySelected.setText("Y");
		ySelected.addSelectionListener(this);
	}
	
	public void widgetSelected(SelectionEvent e) {
		Object c = e.getSource();
		Button cb = null;
		
		if(c == xSelected){
			cb = (Button)e.getSource();
			if(cb.getSelection() == true){
				axisMode = XAXIS;
				makeProjection();
			}
			return;
		}
		if(c == ySelected){
			cb = (Button)e.getSource();
			if(cb.getSelection() == true){
				axisMode = YAXIS;
				makeProjection();
			}
			return;
		}
		super.widgetSelected(e);
	}

	public void makePlot() {
		if(dying){
			return;
		}
		make2DPlot();
		makeProjection();
	}

	/**
	 * fairly constant Entities pulled out in order to reduce 
	 * memory loss problems
	 */
	private  SGTMetaData __yMeta = null;
	private  LineAttribute __latt = null;
	private  SimpleLine __sl = null; 
	
	private void makeProjection() {
		SGTMetaData xMeta;
		GraphAxis xGraph;
		final String projTitle;
		
		if(axisMode == XAXIS){
			xGraph = getXAxis();
			projTitle = "X-Projection";
		} else {
			xGraph = getYAxis();
			projTitle = "Y-Projection";
		}
		xMeta = new SGTMetaData(xGraph.getAxisName(),"");
		if(__yMeta == null){
			__yMeta = new SGTMetaData("Sum", "");
		}
		if(__latt == null){
			__latt = new LineAttribute(LineAttribute.SOLID, Color.blue);
		}
		if(!calculateProjection()) return;
		if(__sl == null){
			__sl = new SimpleLine(projAxis, projData, "Sum");
		} else {
			__sl.setXArray(projAxis);
			__sl.setYArray(projData);
		}
		__sl.setXMetaData(xMeta);
		__sl.setYMetaData(__yMeta);
		EventQueue.invokeLater(new Runnable(){
			public void run() {
				try{
					projPlot.clear();
					projPlot.setBatch(true);
					projPlot.addData(__sl,__latt,"Sum");
					projPlot.setTitles(projTitle, "", "");
					if(axisMode == XAXIS){
						projPlot.setXRange(zoomDomain.getXRange());
					} else {
						projPlot.setXRange(zoomDomain.getYRange());
					}
					projPlot.setBatch(false);
					projPlot.repaint();
				}catch(Exception eva){
					SysRegistry.getLogger().exception(eva);					
				}
			}});
	}

	private boolean calculateProjection() {
		int xstart, xend, ystart, yend;
		GraphAxis xAxis;
		
		if(zoomDomain == null){
			zoomDomain = lpl.getRange();
		}
		Range2D xr = zoomDomain.getXRange();
		Range2D yr = zoomDomain.getYRange();

		xAxis = getXAxis();
		xstart = xAxis.findIndex(xr.start);
		xend = xAxis.findIndex(xr.end);
		
		xAxis = getYAxis();
		ystart = xAxis.findIndex(yr.start);
		yend =  xAxis.findIndex(yr.end);
		
		if(yend - ystart < 0 || xend - xstart < 0){
			/*
			 * The zoom range may not have been properly initialized: 
			 * Try reset to full axis length
			 */ 
			if(xAxis.getDataLength() > 0){
				xend = xAxis.getDataLength() - 1;
			}
			if(yAxis.getDataLength() > 0){
				yend = yAxis.getDataLength() - 1;
			}
			/*
			 * Now we are not initialized enough to work. Bomb out
			 */
			if(yend - ystart < 0 || xend - xstart < 0){
				System.out.println("Bombing out with uniniatilized axis");
				return false;
			}
		}
		
		GraphData2D tata = (GraphData2D)getData(dataName);
		if(axisMode == XAXIS){
			xAxis = getXAxis();
			makeAxis(xAxis, xstart, xend);
			projData = ute.projectX(tata.getOriData(), projData,
					getXAxis().getDataLength(), 
					getYAxis().getDataLength(), 
					xstart, xend, ystart, yend);
		} else {
			xAxis = getYAxis();
			makeAxis(xAxis, ystart, yend);
			projData = ute.projectY(tata.getOriData(), projData,
					getXAxis().getDataLength(), 
					getYAxis().getDataLength(), 
					xstart, xend, ystart, yend);
		}
		return true;
	}
	
	/**
	 * pulled out because of memory problems
	 */
	private SimpleGrid __sg = null;
	
	public void make2DPlot() {
		SGTMetaData xMeta, yMeta, zMeta;
		GraphAxis x,y;
		double data[];
		
		x = getXAxis();
		y = getYAxis();
		GraphData gd = getData(dataName);
		int size = x.getDataLength()*y.getDataLength();
		if(gd.getDataLength() < size){
			SysRegistry.getLogger().debug("Data, axis mismatch in Graph2DSGT, should "
					+ size + ", is " + gd.getDataLength());
			return;
		}
		if(starting){
			starting = false;
			zoomDomain = new Domain(new Range2D(x.getDataValue(0),x.getDataValue(x.getDataLength()-1)), 
					new Range2D(y.getDataValue(0),y.getDataValue(y.getDataLength()-1)));
		}
		lpl.clear();
		lpl.setBatch(true);
		xMeta = new SGTMetaData(x.getAxisName(),"");
		yMeta = new SGTMetaData(y.getAxisName(),"");
		zMeta = new SGTMetaData("counts","n");
		final DataRange dr = gd.getDataRange();
		if(!manualRange){
			minData = dr.min;
			maxData = dr.max;
		}
		data = fixDataRange(gd);
		/*
		ArrayTools.printStats(x.getData());
		ArrayTools.printStats(y.getData());
		ArrayTools.printStats(data);
		*/
		__sg = new SimpleGrid(data,
				x.getData(),y.getData(),dataName);
		__sg.setXMetaData(xMeta);
		__sg.setYMetaData(yMeta);
		__sg.setZMetaData(zMeta);
		if(map == null){
			try{
				makeColorMap();
			}catch(IOException eva){
				SysRegistry.getLogger().debug(eva.getMessage() + 
						" while trying to load colour map " + colorMaps.getText());
				makeDefaultMap();
			}
		}
		final GridAttribute ga = new GridAttribute();
		ga.setStyle(GridAttribute.RASTER);
		if(logData){
			ga.setColorMap(logMap);
		} else {
			ga.setColorMap(map);
		}
		Display d = embed2DComposite.getDisplay();
		lpl.setTitles(gd.getTitle(), "","");
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try{
					lpl.addData(__sg,ga,dataName);
					lpl.setBatch(false);
					if(zoomDomain != null){
						lpl.setRangeNoVeto(zoomDomain);
					}
				}catch(Exception eva){
					SysRegistry.getLogger().exception(eva);
				}
			}
		});
		d.asyncExec(new Runnable(){
			public void run() {
				try{
					lower.setMaximum((int)dr.max);
					upper.setMaximum((int)dr.max);
					if(!manualRange){
						upper.setSelection(upper.getMaximum());
					}
				}catch(Exception eva){
					SysRegistry.getLogger().exception(eva);
				}
			}
		});
		data = null;
	}

	private void makeAxis(GraphAxis axis, int xstart, int xend) {
		int length, i, j;
		double data[];
		
		length = (xend - xstart) + 1;
		if(projAxis == null || projData == null || projAxis.length != length){
			projAxis = null;
			projData = null;
			projAxis = new double[length];
			projData = new double[length];
		}
		data = axis.getData();
		if(xstart < 0){
			xstart = 0;
		}
		if(xend >= data.length){
			xend = data.length;
		}
		for(i = xstart, j = 0; i < xend; i++, j++){
			projAxis[j] = data[i];
		}
	}

	public void propertyChange(PropertyChangeEvent evt) {
		String name = evt.getPropertyName();
		if(name.equals("zoomRectangle")){
			Rectangle zm = (Rectangle)evt.getNewValue();
			if(zm.width <= 1 || zm.height <= 1 ){
				return;
			}
			zoomDomain = lpl.getRange();
			makeProjection();
		}
		if(name.equals("domainRange")){
			zoomDomain = lpl.getRange();
			makeProjection();
		}
		super.propertyChange(evt);
	}

	public void widgetDisposed(DisposeEvent e) {
		dying = true;
	}
}
