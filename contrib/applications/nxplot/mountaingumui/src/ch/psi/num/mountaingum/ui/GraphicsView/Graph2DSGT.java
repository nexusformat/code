/**
 * This is the default  class for displaying 2D histograms. It is based on the 
 * SGT visualisation package.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2007
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import java.awt.EventQueue;
import java.awt.Frame;
import java.io.IOException;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.DataRange;
import ch.psi.num.mountaingum.ui.GraphAxis;
import ch.psi.num.mountaingum.ui.GraphData;

import org.eclipse.swt.awt.SWT_AWT;
import org.eclipse.swt.events.SelectionEvent;

import gov.noaa.pmel.sgt.GridAttribute;
import gov.noaa.pmel.sgt.dm.SGTMetaData;
import gov.noaa.pmel.sgt.dm.SimpleGrid;
import gov.noaa.pmel.sgt.swing.JPlotLayout;

public class Graph2DSGT extends Graph2DSGTBase {
	protected Frame embedFrame;
	protected Composite embedComposite; 
	
	public Graph2DSGT(){
		logData = false; 
		manualRange = false;
		mapName = "Rainbow";
		zoomDomain = null;
		map = null;
	}
	public Object makeEditor(Composite uiData, TreeNode n) throws Exception {
		graphNode = n;
		lpl = new JPlotLayout(true,false,false,"MountainGumPlot", null,true);
		lpl.addPropertyChangeListener(this);
		collectData();
		Composite container = new Composite((Composite)uiData, SWT.NONE);
		GridLayout gritti = new GridLayout();
		gritti.numColumns = 2;
		container.setLayout(gritti);
		Composite graphContainer = new Composite(container, SWT.NONE);
		graphContainer.setLayout(new GridLayout());
		graphContainer.setLayoutData(new GridData(GridData.FILL_BOTH));
		embedComposite = new Composite(graphContainer, SWT.EMBEDDED);
		embedComposite.setLayoutData(new GridData(GridData.FILL_BOTH));
		embedFrame = SWT_AWT.new_Frame(embedComposite); 
		lpl.setTitles(graphNode.getName(), "", "");
		lpl.addMouseMotionListener(this);
		embedFrame.add(lpl);
		makeRangeSelection(graphContainer);
		makeControls(container);
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
		SGTMetaData xMeta, yMeta, zMeta;
		GraphAxis x,y;
		double data[];
		
		x = getXAxis();
		y = getYAxis();
		GraphData gd = getData(dataName);
		/*
		double gurk[] = gd.getData();
		double sum = .0;
		for(int k = 0;  k < gurk.length; k++){
			sum += gurk[k];
		}
		System.out.println("Data sum = " + sum);
		*/
		int size = x.getDataLength()*y.getDataLength();
		if(gd.getDataLength() < size){
			SysRegistry.getLogger().debug("Data, axis mismatch in Graph2DSGT, should "
					+ size + ", is " + gd.getDataLength());
			return;
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
		final SimpleGrid sg = new SimpleGrid(data,
				x.getData(),y.getData(),dataName);
		sg.setXMetaData(xMeta);
		sg.setYMetaData(yMeta);
		sg.setZMetaData(zMeta);
		if(map == null){
			try{
				makeColorMap();
			}catch(IOException eva){
				SysRegistry.getLogger().debug(eva.getMessage() + 
						" while trying to load colour map " + colorMaps.getText());
				makeDefaultMap();
			}
		}
		updateTransforms();
		final GridAttribute ga = new GridAttribute();
		ga.setStyle(GridAttribute.RASTER);
		if(logData){
			ga.setColorMap(logMap);
		} else {
			ga.setColorMap(map);
		}
		lpl.setTitles(gd.getTitle(), "","");
		Display d = embedComposite.getDisplay();
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try{
					lpl.addData(sg,ga,dataName);
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
	public void processNode(TreeNode n) {
		// defunct
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
