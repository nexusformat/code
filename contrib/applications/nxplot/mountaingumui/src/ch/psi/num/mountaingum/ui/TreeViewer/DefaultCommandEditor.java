/**
 * This is the default command processor which shows a table of 
 * parameters and controls for editing them.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 * 
 * Modified for revised tree editor
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeNodeList;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;

public class DefaultCommandEditor implements CommandNodeEditor {
	private CommandNode comNode;

	private class ParModify implements KeyListener, UpdateListener, FocusListener{

		private ParameterNode target;
		private Text editor;
		private boolean initialized;
		ParModify(ParameterNode target, Text editor){
			String data = null;
			
			this.target = target;
			this.editor = editor;
			initialized = false;
			
			data = target.getProperty("commandValue");
			if(data == null){
				data = target.getValue().toString();
			}
			if(data != null){
				editor.setText(data);
				initialized = true;
			} else {
				editor.setText("No initial value");
				target.addUpdateListener(this);
			}
			editor.addFocusListener(this);
		}
		public void keyPressed(KeyEvent e) {
			if(e.character == '\r' || e.character == '\n'){
				target.setProperty("commandValue",editor.getText());
			}
		}
		public void keyReleased(KeyEvent e) {
			// Not needed
		}
		public void nodeStatusChanged(TreeNode node) {
			// nothing to do here
		}
		public void nodeUpdated(TreeNode n) {
			if(!initialized){
				final String data = target.getValue().toString();
				if(data != null){
					initialized = true;
					editor.getDisplay().asyncExec(new Runnable(){
						public void run(){
							editor.setText(data);
						}
					});
				}
			}
		}
		public void focusGained(FocusEvent e) {
			// nothing to do
		}
		public void focusLost(FocusEvent e) {
			target.setProperty("commandValue",editor.getText());
		}
	};
	private class ComboModify implements SelectionListener {
		private ParameterNode p;
		private Combo c;
		
		ComboModify(ParameterNode p, Combo c){
			this.p = p;
			this.c = c;
		}

		public void widgetDefaultSelected(SelectionEvent e) {
			doSelect();
		}

		private void doSelect() {
			p.setProperty("commandValue",c.getText());
		}

		public void widgetSelected(SelectionEvent e) {
			doSelect();
		}
	}
	private class MakePar implements IFunc {

		private Composite parent;
		MakePar(Composite parent){
			this.parent = parent;
		}
		public Object apply(Object o) {
			int idx;
			if(o instanceof ParameterNode){
				TreeNodeList tnl = null;
				ParameterNode valChild;
				String values = null;
				
				ParameterNode pn = (ParameterNode)o;
				if(pn.isHidden()){
					return null;
				}
				Label a = new Label(parent,SWT.LEFT);
				a.setText(pn.getName());
				a.setBackground(parent.getDisplay().getSystemColor(SWT.COLOR_WHITE));
				valChild = (ParameterNode)TreeUtil.findChild(pn, "values");
				if(valChild != null){
					values = valChild.getValue().toString();
				} else {
					values = pn.getProperty("values");
				}
				String argtype = pn.getProperty("argtype");
				if(argtype != null){
					tnl = TreeRegistry.getInstance().getNodeList(argtype);
				} 
				GridData gd = new GridData(GridData.FILL_HORIZONTAL);
				if(values != null){
					Combo combi = new Combo(parent,SWT.SIMPLE);
					String sel[] = values.split(",");
					for(int i = 0; i < sel.length; i++){
						combi.add(sel[i]);
					}
					String val = pn.getValue().toString();
					if(val != null){
						idx = combi.indexOf(val);
					} else {
						idx = 0;
					}
					combi.select(Math.max(idx, 0));
					combi.setLayoutData(gd);
					combi.addSelectionListener(new ComboModify(pn,combi));
				} else if(argtype != null && tnl != null){
					Combo combi = new Combo(parent,SWT.SIMPLE);
					combi.setLayoutData(gd);
					ComboControl cc = new ComboControl(combi,pn,tnl);
				} else {
					Text eddi = new Text(parent,SWT.LEFT);
					eddi.setText(pn.getValue().toString());
					eddi.setLayoutData(gd);
					eddi.addKeyListener(new ParModify(pn,eddi));
				}
				SysRegistry.getLogger().trace("Doing parameter " + pn.getName());
			}
			return null;
		}
	};
	public Object makeEditor(Composite uiData, TreeNode n)throws Exception {
		Composite master = new Composite(uiData,SWT.NONE);
		master.setLayout(new GridLayout());
		Composite table = new Composite(master, SWT.NONE);
		table.setBackground(table.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		GridLayout gl = new GridLayout();
		gl.numColumns = 2;
		table.setLayout(gl);
		table.setLayoutData(new GridData(GridData.FILL_BOTH));
		FuncUtil.map(new ChildStream(n), new MakePar(table));
		comNode = (CommandNode)n;
		return master;
	}
	private class CopyPar implements IFunc {
		public Object apply(Object o) {
			ParameterNode p = (ParameterNode)o;
			if(p.isHidden()){
				return null;
			}
			String comVal = p.getProperty("commandValue");
			if(comVal != null){
				p.setValue(comVal);
			}
			return null;
		}
	}
	public void enqueue() {
		FuncUtil.map(new ChildStream(comNode), new CopyPar());
		comNode.enqueue();
	}
	public void save() {
		FuncUtil.map(new ChildStream(comNode), new CopyPar());
	}
	public void start() {
		FuncUtil.map(new ChildStream(comNode), new CopyPar());
		comNode.start();
	}
}
