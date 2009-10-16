/**
 * A static class to access and adpater to the UI. To be used by lower level code.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui;

public class UIRegistry {
	private static UIAdapter adapter = null;
	
	public static void setAdapter(UIAdapter a){
		adapter = a;
	}
	public static UIAdapter getAdapter(){
		if(adapter == null){
			adapter = new UIAdapter();
		}
		return adapter;
	}
}
