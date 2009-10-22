/**
 * This class defines the menu bar for the NXplot application
 * 
 * copyright:GPL
 * 
 * Mark Koennecke, October 2009
 */
package org.nexusformat.nxplot;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.ui.IWorkbenchActionConstants;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.actions.ActionFactory;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;
import org.eclipse.ui.application.ActionBarAdvisor;
import org.eclipse.ui.application.IActionBarConfigurer;

import ch.psi.num.mountaingum.nexus.actions.*;
/**
 * An action bar advisor is responsible for creating, adding, and disposing of
 * the actions added to a workbench window. Each window will be populated with
 * new actions.
 */
public class ApplicationActionBarAdvisor extends ActionBarAdvisor {

	// Actions - important to allocate these only in makeActions, and then use
	// them
	// in the fill methods. This ensures that the actions aren't recreated
	// when fillActionBars is called with FILL_PROXY.
	private IWorkbenchAction exitAction, about;
	private IAction open, save;
	
	public ApplicationActionBarAdvisor(IActionBarConfigurer configurer) {
		super(configurer);
	}

	protected void makeActions(final IWorkbenchWindow window) {
		// Creates the actions and registers them.
		// Registering is needed to ensure that key bindings work.
		// The corresponding commands key bindings are defined in the plugin.xml
		// file.
		// Registering also provides automatic disposal of the actions when
		// the window is closed.

		exitAction = ActionFactory.QUIT.create(window);
		register(exitAction);
		
		about = ActionFactory.ABOUT.create(window);
		
		open = new ActionDelegateProxy("Open" ,new OpenNexus());
		register(open);
		save = new ActionDelegateProxy("Save" ,new SaveNexus());
		register(save);

	}

	protected void fillMenuBar(IMenuManager menuBar) {
		MenuManager fileMenu = new MenuManager("&File",
				IWorkbenchActionConstants.M_FILE);
		menuBar.add(fileMenu);
		fileMenu.add(open);
		fileMenu.add(save);
		fileMenu.add(exitAction);
		
		MenuManager helpMenu = new MenuManager("&Help",
				IWorkbenchActionConstants.HELP_END);
		menuBar.add(helpMenu);
		helpMenu.add(about);
		
	}
}
