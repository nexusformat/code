/**
 * A utility class for doing eclipse RCP specific stuff.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 * 
 * Added printElementsForPoint(Sting point), Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.ui.eclipse;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.IExtensionPoint;
import org.eclipse.core.runtime.IExtensionRegistry;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.IViewPart;
import org.eclipse.ui.IViewReference;
import org.eclipse.ui.PlatformUI;

import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.sys.SysRegistry;

public class RCPUtil {
	static int debug = 0;
	
	public static void printExtensions(){
		IExtensionRegistry reg = Platform.getExtensionRegistry();
		IExtensionPoint p[] = reg.getExtensionPoints();
		for(int i = 0; i < p.length; i++){
			System.out.println(p[i].getLabel() + ", " + p[i].getUniqueIdentifier());
		}
	}
	/**
	 * get an executable obejct for a registered extension
	 * @param point The extension point name
	 * @param id The id parameter of the sedired class
	 * @return A suitable Object or null on failure
	 */
	public static Object getExtensionObject(String point, String id){
		Object o = null;
		if(id == null){
			return null;
		}
		IConfigurationElement[] decl = Platform.getExtensionRegistry().getConfigurationElementsFor(point);
		for(int i = 0; i < decl.length; i++){
			IConfigurationElement e = decl[i];
			if(debug == 1){
				String[] atts = e.getAttributeNames();
				for(int j = 0; j < atts.length; j++){
					System.out.println(atts[j] + " = " + e.getAttribute(atts[j]));
				}
			}
			if(id.equals(e.getAttribute("id"))){
				try{
					o = e.createExecutableExtension("className");
				}catch(CoreException eva){
					UIRegistry.getAdapter().displayError("Failed to load class for " + id);
				}
			}
		}
		return o;
	}
	/**
	 * print all the element attributes available for the specified extension point
	 * @param point The extension point to print for.
	 */
	public static void printElementsForPoint(String point){
		IConfigurationElement[] decl = Platform.getExtensionRegistry().getConfigurationElementsFor(point);
		for(int i = 0; i < decl.length; i++){
			IConfigurationElement e = decl[i];
			System.out.println(e.getName());
			String[] atts = e.getAttributeNames();
			for(int j = 0; j < atts.length; j++){
				System.out.println(atts[j] + " = " + e.getAttribute(atts[j]));
			}
		}
	}
	/**
	 * get a NodeEditor 
	 * @param id The ID of the node editor
	 * @return A suitable NodeEDitor or null when unavailable
	 */
	public static NodeEditor getNodeEditor(String id){
		return (NodeEditor)getExtensionObject("ch.psi.num.mountaingum.ui.eclipse.NodeEditor", id);
	}
	public static NodeEditor getNodeEditor(String id, String defaultID){
		NodeEditor ne = getNodeEditor(id);
		if(ne == null){
			SysRegistry.getLogger().error("Failed to load NodeEditor with id " + id);
		} else{
			return ne;
		}
		ne = getNodeEditor(defaultID);
		if(ne == null){
			System.out.println("Very serious malconfiguration: no default editor");
			System.exit(1);
		}
		return ne;
	}
	/**
	 * locate a view with a give ID in the current perspective.
	 * @param id The ID of the view to locate
	 * @return A suitable ViewPart or null whne not found
	 */
	public static IViewPart findView(String id){
		//Blubs.blabs().fart().gurk().nass().trocken().hugo().lieselotte().fish().vogel().....
		return PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage().findView(id);
	}
	/**
	 * locate a view with a give ID in the current perspective.
	 * @param id The ID of the view to locate
	 * @return A suitable ViewPart or null whne not found
	 */
	public static IViewPart findView(String id, String secID){
		//Blubs.blabs().fart().gurk().nass().trocken().hugo().lieselotte().fish().vogel().....
		IViewReference ref =  PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage().findViewReference(id,secID);
		if(ref != null){
			return ref.getView(true);
		} else {
			return null;
		}
	}
}
