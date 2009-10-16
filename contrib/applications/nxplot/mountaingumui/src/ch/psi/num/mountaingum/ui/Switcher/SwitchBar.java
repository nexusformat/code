/**
 * This is a little utility class which configures the Toolbar for switching 
 * between perspectives.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.Switcher;

import java.util.Iterator;

import org.eclipse.jface.action.ActionContributionItem;
import org.eclipse.jface.action.ToolBarManager;

public class SwitchBar {
	
	public static void configureBar(ToolBarManager tooli){
		String perspectiveID;
		ActionContributionItem aci;

		tooli.add(new SwitchWinControl());
		Iterator it = SwitchRegistry.getIDList().iterator();
		while(it.hasNext()){
			perspectiveID = (String)it.next();
			aci = new ActionContributionItem(new SwitchAction(perspectiveID));
			aci.setMode(ActionContributionItem.MODE_FORCE_TEXT);
			tooli.add(aci);
		}
	}	
}
