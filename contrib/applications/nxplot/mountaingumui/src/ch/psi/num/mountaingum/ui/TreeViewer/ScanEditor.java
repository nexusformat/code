/**
 * This is the default scan editor for mountaingum.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, February 2008
 *  
 *  many bug fixes, addition of RangeEntry calculation type
 *  
 *  Mark Koennecke, October 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import java.text.NumberFormat;
import java.util.StringTokenizer;
import java.util.Vector;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Spinner;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.NodeListListener;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeNodeList;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.util.SWTUtil;

import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.nebula.widgets.compositetable.IRowContentProvider;

public class ScanEditor implements CommandNodeEditor, 
	      SelectionListener, IRowContentProvider, NodeListListener {
	protected static final int NORMALSCAN = 0; 
	protected static final int CENTERSCAN = 1; 
	protected static final int RANGESCAN  = 2;
	private int np;
	protected int calcMode;
	protected  CommandNode comNode;
	protected ScrolledComposite scrolli;
	protected  TreeNode scanNode;

	
	// UI for scan controls
	protected Combo scanMode, countMode;
	private Spinner npScale;
	private Text preset;
	
	// UI for adding scan vars
	private Combo var;
	private Button add, clear;
	private TreeNodeList tnl;
	
	// UI for scan table
	private CompositeTable scanTable; 
	private Vector scanVars;
	protected  Composite editor;
	private NumberFormat nf;
	
	public ScanEditor(){
		np = 5;
		calcMode = NORMALSCAN;
		scanVars = new Vector();
		nf = NumberFormat.getInstance();
		nf.setMaximumFractionDigits(3);
		nf.setMinimumFractionDigits(3);
	}
	public void enqueue() {
		save();
		comNode.enqueue();
	}

	public void save() {
		ParameterNode pn;
		StringBuffer var, start, step;
		ScanEntry se, master = null;
		int i;
		
		pn = (ParameterNode)TreeUtil.findChild(comNode, "NP");
		pn.updateValue(Integer.toString(np));
		
		pn = (ParameterNode)TreeUtil.findChild(comNode, "mode");
		pn.updateValue(countMode.getText());
		
		pn = (ParameterNode)TreeUtil.findChild(comNode, "preset");
		pn.updateValue(preset.getText());
		
		var = new StringBuffer();
		start = new StringBuffer();
		step = new StringBuffer();
		
		/*
		 * first path: find the master ScanEntry: i.e, the one with
		 * the biggest step.
		 */
		for(i = 0; i < scanVars.size(); i++){
			se = (ScanEntry)scanVars.get(i);
			se.calculate(np);
			if(master == null){
				master = se;
			} else {
				if(Math.abs(se.step) > Math.abs(master.step)){
					master = se;
				}
			}
		}
		if(master != null){
			var.append(TreeUtil.pathForNode(master.var));
			var.append(',');
			start.append(master.start);
			start.append(',');
			step.append(master.step);
			step.append(',');
		}
		
		for(i = 0; i < scanVars.size(); i++){
			se = (ScanEntry)scanVars.get(i);
			se.calculate(np);
			if(se != master){
				var.append(TreeUtil.pathForNode(se.var));
				var.append(',');
				start.append(se.start);
				start.append(',');
				step.append(se.step);
				step.append(',');
			}
		}

		pn = (ParameterNode)TreeUtil.findChild(comNode, "scan_variables");
		pn.updateValue(fixString(var));

		pn = (ParameterNode)TreeUtil.findChild(comNode, "scan_start");
		pn.updateValue(fixString(start));

		pn = (ParameterNode)TreeUtil.findChild(comNode, "scan_increments");
		pn.updateValue(fixString(step));
		
		comNode.setProperty("scanmode", scanMode.getText());
	}

	private String fixString(StringBuffer var2) {
		String x = var2.toString();
		return x.substring(0,x.length()-1);
	}
	public void start() {
		save();
		comNode.start();
	}

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		scrolli = new ScrolledComposite(parent, SWT.V_SCROLL|SWT.H_SCROLL);
		editor = new Composite(scrolli, SWT.NONE);
		editor.setLayout(new GridLayout());
		scanNode = n;
		
		makeControlGroup(editor);

		makeVarGroup(editor);
		
		makeScanTable(editor);
		
		loadFromTree(n);
		comNode = (CommandNode)n;
		
		scrolli.setContent(editor);
		scrolli.setExpandHorizontal(true);
		scrolli.setExpandVertical(true);
		scrolli.setMinSize(editor.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		
		return scrolli;
	}
	protected  void loadFromTree(TreeNode n) {	
		ParameterNode c, v; 
		StringTokenizer st1, st2, st3;
		double start, step;
		String var;
		
		c = (ParameterNode)TreeUtil.findChild(n, "NP");
		try{
			np = (int)TreeUtil.getNodeValueAsDouble(c)[0];
		}catch(Exception edith){
			/*
			 * This means that the tree is not yet fully initialized 
			 * and we cannot proceed.
			 */
			//UIRegistry.getAdapter().displayError("Tree not yet fully initialized, wait and try again");
			return;
		}
		if(np < 1){
			np = 5;
		}
		npScale.setSelection(np);
		
		c = (ParameterNode)TreeUtil.findChild(n, "mode");
		int idx = countMode.indexOf(c.getValue().toString());
		countMode.select(idx);
		
		c = (ParameterNode)TreeUtil.findChild(n, "preset");
		String presi = c.getValue().toString();
		if(presi.indexOf("UNDEF") >= 0 || presi.length() < 2){
			preset.setText("60000");
		} else {
			preset.setText(c.getValue().toString());
		}
		
		c = (ParameterNode)TreeUtil.findChild(n, "scan_variables");
		st1 = new StringTokenizer(c.getValue().toString(),",",false);
		c = (ParameterNode)TreeUtil.findChild(n, "scan_start");
		st2 = new StringTokenizer(c.getValue().toString(),",",false);
		c = (ParameterNode)TreeUtil.findChild(n, "scan_increments");
		st3 = new StringTokenizer(c.getValue().toString(),",",false);
		
		while(st1.hasMoreTokens()){
			var = st1.nextToken();
			v = (ParameterNode)TreeUtil.nodeForPath(var);
			if(v != null){
				start = Double.parseDouble(st2.nextToken());
				step = Double.parseDouble(st3.nextToken());
				ScanEntry e = new NormalEntry(v);
				e.start = start;
				e.step = step;
				e.calculate(np);
				scanVars.add(e);
			}
		}
		scanTable.setNumRowsInCollection(Math.max(1,scanVars.size()));
		
		/*
		 * I have to do this here as the scan data is stored in normal 
		 * mode. Therefore I have to calculate everything in normal mode 
		 * before I can switch to any other mode.   
		 */
		String val = n.getProperty("scanmode");
		if(val != null && !val.equalsIgnoreCase("normal")){
			scanMode.select(scanMode.indexOf(val));
		}
		switchCalcMode();
	}
	protected  void makeScanTable(Composite editor) {
		Group g = new Group(editor,SWT.NONE);
		g.setLayoutData(new GridData(GridData.FILL_BOTH));
		g.setLayout(new FillLayout());
		g.setText("Scan Variables");
		
		scanTable = new CompositeTable(g,SWT.NULL);
		new ScanTableHeader(scanTable,SWT.NULL);
		new ScanTableRow(scanTable,SWT.NULL);
		scanTable.setInsertHint("Use Add above to add scan variable entries");
		scanTable.setRunTime(true);
		scanTable.setNumRowsInCollection(5);
		scanTable.addRowContentProvider(this);
		//SWTUtil.printHierarchy(g,2);
	}
	void updateRow(int row, ScanTableRow r){
		ScanEntry se = (ScanEntry)scanVars.get(row);
		se.setStart(Double.parseDouble(r.start.getText()));
		se.setCenter(Double.parseDouble(r.center.getText()));
		se.setEnd(Double.parseDouble(r.end.getText()));
		se.setStep(Double.parseDouble(r.step.getText()));
		int npn = se.calculate(np);
		if(npn != np){
			npScale.setSelection(npn);
			np = npn;
		}
		r.start.setText(nf.format(se.getStart()));
		r.center.setText(nf.format(se.getCenter()));
		r.end.setText(nf.format(se.getEnd()));
		r.step.setText(nf.format(se.getStep()));
	}
	private class AddToCombi implements IFunc {
		public Object apply(Object o) {
			TreeNode n = (TreeNode)o;
			var.add(TreeUtil.pathForNode(n));
			return null;
		}
	}
	protected void makeVarGroup(Composite editor) {
		String values = null;
		int i;
		
		Group v = new Group(editor,SWT.NONE);
		v.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		RowLayout rolli = new RowLayout();
		rolli.fill = true;
		rolli.justify = true;
		v.setLayout(rolli);
		v.setText("Add Scan Variable");
		
		Group vv = new Group(v,SWT.NONE);
		vv.setLayout(new FillLayout());
		vv.setText("Scan Variable");
		var = new Combo(vv,SWT.SIMPLE|SWT.READ_ONLY);
		TreeNode scanvar = (ParameterNode)TreeUtil.findChild(scanNode, "scan_variables");
		if(scanvar != null){
			values = scanvar.getProperty("values");
		}
		if(values == null){
			/*
			 * all drivables
			 */
			tnl = TreeRegistry.getInstance().getNodeList("drivable");
			tnl.addNodeListListener(this);
			FuncUtil.map(tnl.getNodeStream(), new AddToCombi());
		} else {
			/*
			 * values variable
			 */
			String svars[] = values.split(",");
			for(i = 0; i < svars.length; i++){
				var.add(svars[i]);
			}
		}
		var.select(0);
		var.addSelectionListener(this);
				
		add = new Button(v,SWT.PUSH);
		add.setText("Add");
		add.addSelectionListener(this);
		
		clear = new Button(v,SWT.PUSH);
		clear.setText("Clear all");
		clear.addSelectionListener(this);
	}
	protected void makeControlGroup(Composite editor) {
		Group g = new Group(editor, SWT.NONE);
		g.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		g.setText("Scan Control Parameters");
//		g.setLayout(new FillLayout());
//		g.setLayout(new  GridRowLayout(new int[] {2,1,2,1}));
		RowLayout raudi = new RowLayout();
		raudi.fill = false;
		raudi.wrap = false;
		raudi.pack = true;
		raudi.fill = true;
		raudi.type = SWT.HORIZONTAL;
		g.setLayout(raudi);
		
		Group mode = new Group(g,SWT.NONE);
		mode.setText("Scan Mode");
		mode.setLayout(new FillLayout());
		scanMode = new Combo(mode,SWT.SIMPLE|SWT.READ_ONLY);
		scanMode.add("normal");
		scanMode.add("center");
		scanMode.add("range");
		scanMode.select(0);
		scanMode.addSelectionListener(this);
		
		Group npGroup = new Group(g,SWT.NONE);
		npGroup.setText("Scan Points");
		npGroup.setLayout(new FillLayout());
		npScale = new Spinner(npGroup, SWT.VERTICAL);
		npScale.setSelection(5);
		npScale.addSelectionListener(this);
		
		Group countGroup = new Group(g,SWT.NONE);
		countGroup.setText("Count Mode");
		countGroup.setLayout(new FillLayout());
		countMode = new Combo(countGroup,SWT.SIMPLE| SWT.READ_ONLY);
		countMode.add("monitor");
		countMode.add("timer");
		countMode.select(0);
		countMode.addSelectionListener(this);

		Group preGroup = new Group(g,SWT.NONE);
		preGroup.setLayout(new FillLayout());
		preGroup.setText("Preset");
		preset = new Text(preGroup,SWT.SINGLE);
	}
	public void widgetSelected(SelectionEvent e) {
		Object o = e.getSource();
		if(o == clear){
			scanVars.removeAllElements();
			updateAll();
			return;
		}
		if( o ==  add){
			addScanVar();
			return;
		}
		if(o == scanMode ){
			switchCalcMode();
			return;
		}
		if(o == npScale){
			np = npScale.getSelection();
			scanTable.refreshAllRows();
			return;
		}
	}
	private void switchCalcMode() {
		calcMode = scanMode.getSelectionIndex();
		Vector newScanVars = new Vector();
		for(int i = 0; i < scanVars.size(); i++){
			ScanEntry o = (ScanEntry)scanVars.get(i);
			if(calcMode == NORMALSCAN){
				newScanVars.add(new NormalEntry(o));
			} else if(calcMode == RANGESCAN){
				newScanVars.add(new RangeEntry(o));
			} else {
				newScanVars.add(new CenterEntry(o));
			}
		}
		if(calcMode == RANGESCAN){
			npScale.setEnabled(false);
		} else {
			npScale.setEnabled(true);
		}
		scanVars = null;
		scanVars = newScanVars;
		scanTable.refreshAllRows();
	}
	private void addScanVar() {

		String path = var.getText();
		addScanVar(path);
	}
	protected void addScanVar(String path){
		ScanEntry se = null;

		ParameterNode pn = (ParameterNode)TreeUtil.nodeForPath(path);
		if(pn == null){
			/**
			 * This is a fix for normal sics device names being used as 
			 * scan variables. 
			 */
			pn = (ParameterNode)TreeUtil.nodeForProperty(
					TreeRegistry.getInstance().getRoot(), "sicsdev", path);
		}
		if(calcMode == NORMALSCAN){
			se = new NormalEntry(pn);
		} else if(calcMode == CENTERSCAN){
			se = new CenterEntry(pn);
		} else {
			// cannot happen
			se = new NormalEntry(pn);
		}
		scanVars.add(se);
		updateAll();
	}
	private void updateAll() {
		scanTable.setNumRowsInCollection(Math.max(1,scanVars.size()));
		scanTable.setMaxRowsVisible(5);
		scanTable.refreshAllRows();
		scrolli.setMinSize(editor.computeSize(SWT.DEFAULT, SWT.DEFAULT));
	}
	public void refresh(CompositeTable arg0, int row, Control r) {
		ScanTableRow str = (ScanTableRow)r;
		if(row < scanVars.size()){
			ScanEntry se = (ScanEntry)scanVars.get(row);
			str.var.setText(se.var.getName());
			se.calculate(np);
			str.start.setText(nf.format(se.getStart()));
			str.center.setText(nf.format(se.getCenter()));
			str.end.setText(nf.format(se.getEnd()));
			str.step.setText(nf.format(se.getStep()));
			str.setRow(row);
			str.setMama(this);
			se.setEdit((ScanTableRow)r);
		} else {
			str.var.setText("Undefined");
			str.start.setText("0");
			str.center.setText("0");
			str.end.setText("0");
			str.step.setText("0");
		}
	}
	public void nodeListChanged(TreeNodeList l) {
		final TreeNodeList txl = l;
		var.getDisplay().asyncExec(new Runnable(){
			public void run() {
				var.removeAll();
				FuncUtil.map(txl.getNodeStream(), new AddToCombi());
			}});
	}
	public void dispose(){
		tnl.removeNodeListListener(this);
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
