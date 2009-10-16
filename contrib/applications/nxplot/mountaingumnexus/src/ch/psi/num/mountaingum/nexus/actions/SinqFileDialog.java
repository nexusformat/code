/**
 * This is the dialog for loading SINQ files.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.actions;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Calendar;
import java.util.HashSet;
import java.util.Properties;
import java.util.StringTokenizer;

import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.sys.SysRegistry;

public class SinqFileDialog extends Dialog {
	private String filename;
	private Combo instrument;
	private Combo year;
	private Text numor;
	private HashSet killSet;
	
	private static String savedInst = "savedInst";
	private static String savedYear = "savedYear";
	private static String savedNumor = "savedNumor";
	
	protected SinqFileDialog(Shell parentShell) {
		super(parentShell);
		killSet = new HashSet();
		killSet.add("local");
		killSet.add("test");
		killSet.add("sansli");
		killSet.add("tasp");
	}

	protected Control createDialogArea(Composite parent) {
		Composite root = (Composite)super.createDialogArea(parent);
		root.setLayout(new GridLayout());
		
		Group instGroup = new Group(root,SWT.NONE);
		instGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		instGroup.setText("Select Instrument");
		instGroup.setLayout(new GridLayout());
		instrument = new Combo(instGroup,SWT.SINGLE);
		instrument.setLayoutData(new GridData(GridData.FILL_BOTH));
		URL ulli = Platform.getBundle("MountainGumApplication").getEntry("sics.config");
		Properties config  = new Properties();
		try{
			config.load(ulli.openStream());
		}catch(IOException e){
			SysRegistry.getLogger().error("Failed to load SICS properties");
			return root;
		}
		String prop = config.getProperty("instrumentlist");
		if(prop == null){
			SysRegistry.getLogger().error("Failed to load SICS Properties");
			return root;
		}
		StringTokenizer st = new StringTokenizer(prop.trim(),",",false);
		String inst;
		while(st.hasMoreTokens()){
			inst = st.nextToken();
			if(inst.indexOf("sim") < 0 && !killSet.contains(inst)){
				if(inst.indexOf("rita") >= 0){
					instrument.add("rita2");
				} else {
					instrument.add(inst);
				}
			}
		}
		instrument.select(0);
		
		Group yearGroup  = new Group(root,SWT.NONE);
		yearGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		yearGroup.setText("Select Year");
		yearGroup.setLayout(new GridLayout());
		year = new Combo(yearGroup, SWT.SINGLE);
		year.setLayoutData(new GridData(GridData.FILL_BOTH));
		Calendar cal = Calendar.getInstance();
		int yy = cal.get(Calendar.YEAR);
		for(int y = yy; y > 1996; y--){
			year.add(Integer.toString(y));
		}
		year.select(0);
		
		Group numGroup  = new Group(root,SWT.NONE);
		numGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		numGroup.setText("Data File Number");
		numGroup.setLayout(new GridLayout());
		numor = new Text(numGroup, SWT.SINGLE);
		numor.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		numor.setEditable(true);
		numor.setText("0");
		
		IEclipsePreferences pref = new InstanceScope().getNode("mountaingumnexus");
		String val = pref.get(savedInst, null);
		if(val != null){
			instrument.setText(val);
		}
		val = pref.get("savedYear", null);
		if(val != null){
			year.setText(val);
		}
		val = pref.get("savedNumor", null);
		if(val != null){
			numor.setText(val);
		}
		
		
		return root;
	}

	public String getFilename() {
		return filename;
	}

	protected void okPressed() {
		filename = SinqFile.getSinqFile(instrument.getText(),
				Integer.parseInt(year.getText()), 
				Integer.parseInt(numor.getText()), ".hdf");

		if(filename == null){
			filename = SinqFile.getSinqFile(instrument.getText(),
					Integer.parseInt(year.getText()), 
					Integer.parseInt(numor.getText()), ".dat");
		}
		IEclipsePreferences pref = new InstanceScope().getNode("mountaingumnexus");
		pref.put(savedInst, instrument.getText());
		pref.put(savedYear, year.getText());
		pref.put(savedNumor, numor.getText());
		try{
			pref.flush();
		}catch(Exception e){}
		
		super.okPressed();
	}

}
