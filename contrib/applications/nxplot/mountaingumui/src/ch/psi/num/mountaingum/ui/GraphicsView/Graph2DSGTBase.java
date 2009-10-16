/**
 * This class is a base class which encloses some common functionality for 2D
 * graphing classes.
 * 
 * copyright: GPL
 * 
 * Mark Koen
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import gov.noaa.pmel.sgt.CartesianGraph;
import gov.noaa.pmel.sgt.IndexedColorMap;
import gov.noaa.pmel.sgt.Layer;
import gov.noaa.pmel.sgt.LinearTransform;
import gov.noaa.pmel.sgt.LogTransform;
import gov.noaa.pmel.sgt.swing.JPlotLayout;
import gov.noaa.pmel.util.Domain;

import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.StringTokenizer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Scale;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.DataRange;
import ch.psi.num.mountaingum.ui.Graph;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.SGTUtil;

public class Graph2DSGTBase extends Graph implements NodeEditor,
		SelectionListener, MouseMotionListener, PropertyChangeListener {

	protected TreeNode graphNode;
	protected JPlotLayout lpl;
	protected Button log;
	protected Button normal;
	protected Button reset;
	protected Button refresh;
	protected Button automatic;
	protected Button manual;
	protected Combo colorMaps;
	protected double minData;
	protected double maxData;
	protected boolean logData;
	protected String dataName;
	protected String mapName;
	protected Label pos;
	protected Scale lower;
	protected Scale upper;
	protected Domain zoomDomain;
	protected boolean manualRange;
	protected IndexedColorMap map, logMap; 
	protected double washedData[];
	
	public Graph2DSGTBase(){
		super();
		map = null;
		logMap = null;
		washedData = null;
		zoomDomain = null;
	}

	class RangeSelector implements SelectionListener {
		public void widgetSelected(SelectionEvent e) {
			int min, max;
			min = lower.getSelection();
			max = upper.getSelection();
//			System.out.println("min, max = " + min + ", " + max);
			/*
			 * make sure that there is a reasonable range
			 */
			if(max < min){
				max = min +1;
				upper.setSelection(max);
			}
			if(min > max){
				min = max - 1;
				if(min < 0){
					min = 0;
					max = 1;
					upper.setSelection(max);
				}
				lower.setSelection(min);
			}
			if(!manualRange){
				manualRange = true;
				manual.setSelection(true);
				automatic.setSelection(false);
			}
			minData = (double)min;
			maxData = (double)max;
			makePlot();
		}
		public void widgetDefaultSelected(SelectionEvent e) {
			// Nothing to do
		}
	}
	class CollectFunc implements IFunc {

		public Object apply(Object o) {
			if( !(o instanceof ParameterNode)){
				return null;
			}
			ParameterNode n = (ParameterNode)o;
			String type = n.getProperty("type");
			if(type != null){
				if(type.equals("axis")){
					String dim = n.getProperty("dim");
					if(dim != null){
						if(dim.indexOf("0") >= 0){
							setXAxis(new GraphAxis(n.getName(),n));
						} else {
							setYAxis(new GraphAxis(n.getName(),n));
						}
					}
				} else if(type.equals("data")){
					addData(new GraphData2D(n.getName(),n));
					dataName = n.getName();
				}
			}
			return null;
		}
		
	}

	public void makePlot() {
		// TODO Auto-generated method stub

	}

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		// TODO Auto-generated method stub
		return null;
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// TODO Auto-generated method stub

	}

	protected void makeRangeSelection(Composite graphContainer) {
		Group rangeGroup = new Group(graphContainer,SWT.SHADOW_ETCHED_IN);
		rangeGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		rangeGroup.setText("Mapping Range");
		rangeGroup.setLayout(new FillLayout(SWT.HORIZONTAL));
		Group lowGroup = new Group(rangeGroup,SWT.SHADOW_ETCHED_IN);
		lowGroup.setText("Lower Limit");
		lowGroup.setLayout(new FillLayout(SWT.HORIZONTAL));
		lower = new Scale(lowGroup,SWT.HORIZONTAL);
		lower.setMinimum(0);
		lower.setSelection(0);
		Group upGroup = new Group(rangeGroup,SWT.SHADOW_ETCHED_IN);
		upGroup.setText("Upper Limit");
		upGroup.setLayout(new FillLayout(SWT.HORIZONTAL));
		upper = new Scale(upGroup,SWT.HORIZONTAL);
		upper.setMinimum(0);
		
		RangeSelector rs = new RangeSelector();
		lower.addSelectionListener(rs);
		upper.addSelectionListener(rs);
	}

	protected void collectData() {
		clearAll();
		FuncUtil.map(new ChildStream(graphNode), new CollectFunc());
	}

	public void changeData(GraphData g) {
		makePlot();
	}

	protected double[] fixDataRange(GraphData gd) {
			double val;
			double data[];
			
			if(logData){
				data = gd.getDataLogWashed();
			} else {
				data = gd.getData();
			}
			if(washedData == null || washedData.length != data.length){
				washedData = null;
				washedData = new double[data.length];
			}
			
			if(!manualRange){
				System.arraycopy(data, 0, washedData, 0, data.length);
				return washedData;
			} else {
				for(int i = 0; i < data.length; i++){
					val = data[i];
					if(val < minData){
						val = minData;
					}
					if(val > maxData){
						val = maxData;
					}
					if(logData && val < .0){
						val = .1;
					}
					washedData[i] = val;
				}
				return washedData;
			}
	}

	protected void makeDefaultMap() {
		int[] red =
		{   0,  0,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0,  0,  0,  0,
				0,  7, 23, 39, 55, 71, 87,103,
				119,135,151,167,183,199,215,231,
				247,255,255,255,255,255,255,255,
				255,255,255,255,255,255,255,255,
				255,246,228,211,193,175,158,140};
		int[] green =
		{   0,  0,  0,  0,  0,  0,  0,  0,
				0, 11, 27, 43, 59, 75, 91,107,
				123,139,155,171,187,203,219,235,
				251,255,255,255,255,255,255,255,
				255,255,255,255,255,255,255,255,
				255,247,231,215,199,183,167,151,
				135,119,103, 87, 71, 55, 39, 23,
				7,  0,  0,  0,  0,  0,  0,  0};
		int[] blue =
		{   0,143,159,175,191,207,223,239,
				255,255,255,255,255,255,255,255,
				255,255,255,255,255,255,255,255,
				255,247,231,215,199,183,167,151,
				135,119,103, 87, 71, 55, 39, 23,
				7,  0,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0,  0,  0,  0};
		updateTransforms();
	}

	protected void makeColorMap() throws IOException {
		int r[], g[], b[], count = 0;
		String line;
		StringTokenizer stb;

		r = new int[256];
		g = new int[256];
		b = new int[256];
		String myMapName =  mapName + ".rgb";
		URL ulli = getClass().getResource("/ch/psi/num/mountaingum/ui/GraphicsView/maps/" + myMapName);
		BufferedReader in = new BufferedReader(new InputStreamReader(ulli.openStream()));
		while((line = in.readLine()) != null ){
			stb = new StringTokenizer(line," ", false);
			if(stb.countTokens() >= 3){
				r[count] = Integer.parseInt(stb.nextToken());
				g[count] = Integer.parseInt(stb.nextToken());
				b[count] = Integer.parseInt(stb.nextToken());
				count++;
			}
		}
		in.close();
		map = new IndexedColorMap(r,g,b);
		logMap = new IndexedColorMap(r,g,b);
		updateTransforms();
	}

	protected void updateTransforms(){
		logMap.setTransform(new LogTransform(0.0, (double)logMap.getMaximumIndex(),
				Math.max(.1, minData), Math.max(.15,maxData)));
		map.setTransform(new LinearTransform(0.0, (double)logMap.getMaximumIndex(),
				minData, maxData));
		
	}
	public void widgetSelected(SelectionEvent e) {
		if(e.getSource() == colorMaps){
			mapName = colorMaps.getText();
			map = null;
			makePlot();
			return;
		}
	
		Button c = (Button)e.getSource();
		if(c == normal){
			if(c.getSelection() == true){
				logData = false;
				map = null;
				makePlot();
			}
			return;
		}
		if(c == log){
			if(c.getSelection() == true){
				logData = true;
				map = null;
				makePlot();
			}
			return;
		}
		if(c == manual){
			manualRange = true;
			makePlot();
		}
		if(c == automatic){
			manualRange = false;
			lower.setSelection(0);
			DataRange dr = getData(dataName).getDataRange();
			upper.setSelection((int)dr.max);
			minData = dr.min;
			maxData = dr.max;
			makePlot();
		}
		if(c == refresh){
			refresh();
			return;
		}
		if(c == reset){
			lpl.resetZoom();
		}
	}

	public void mouseDragged(MouseEvent e) {
		// Nothing to do
	}

	public void mouseMoved(MouseEvent e) {
		final double xp, yp;
		final int x, y, II;
		double dx, dy;
		int xIdx, yIdx;
		GraphData xAxis, yAxis;
		
		Layer l = lpl.getFirstLayer();
		CartesianGraph g = (CartesianGraph)l.getGraph();
		xp = l.getXDtoP(e.getX());
		dx = g.getXPtoU(xp);
		x = (int)dx;
		yp = l.getYDtoP(e.getY());
		dy = g.getYPtoU(yp);
		y = (int)dy;
		xAxis = getXAxis();
		yAxis = getYAxis();
		
		xIdx  = xAxis.findIndex(dx);
		yIdx =  yAxis.findIndex(dy);
		if(xIdx < 0){
			xIdx = 0;
		}
		if(xIdx > xAxis.getDataLength()){
			xIdx = xAxis.getDataLength() - 1;
		}
		if(yIdx < 0){
			yIdx = 0;
		}
		if(yIdx > yAxis.getDataLength()){
			yIdx = yAxis.getDataLength() - 1;
		}
		
		GraphData gd = getData(dataName);
		int posi[] = new int[2];
		posi[0] = xIdx;
		posi[1] = yIdx;
		II = (int)gd.getValue(posi);
		
		Display d = pos.getDisplay();
		d.asyncExec(new Runnable(){
			public void run() {
				pos.setText(x + "," +y + "," + II);
			}}
		);
	}

	public BufferedImage makeImage(int width, int height) {
		return SGTUtil.drawInImage(lpl,width,height);
	}

	public void print(Printer p) {
		SGTUtil.print(lpl, p);
	}

	public void propertyChange(PropertyChangeEvent evt) {
		String name = evt.getPropertyName();
		if(name.equals("zoomRectangle")){
			Rectangle zm = (Rectangle)evt.getNewValue();
			if(zm.width <= 1 || zm.height <= 1 ){
				return;
			}
			zoomDomain = lpl.getRange();
		}
		if(name.equals("domainRange")){
			zoomDomain = lpl.getRange();
		}
	}

	protected void make2DControls(Composite controlPanel){
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
		posGroup.setBackground(controlPanel.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		posGroup.setLayout(new FillLayout(SWT.VERTICAL));
		posGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		posGroup.setText("x,y,I");
		pos = new Label(posGroup,SWT.NONE);
		pos.setText("Unknown");
		
	}
}
