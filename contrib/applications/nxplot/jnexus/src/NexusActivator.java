/**
 * This is the activator class for the jnexus plugin. Its purpose is to 
 * load all the shared libraries required for jnexus, depending on the OS.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2000
 */
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;

import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Plugin;
import org.osgi.framework.BundleContext;


public class NexusActivator extends Plugin {

	public void start(BundleContext context) throws Exception {
		String line, os, libfile;
		super.start(context);
		
		os = System.getProperty("os.name");
		if(os.indexOf("Windows") >= 0){
			libfile = "windoof" + System.getProperty("os.arch") + ".lib";
		} else if(os.indexOf("Mac OS X") >= 0){
			libfile = "macosx.lib";
		} else {
			libfile = os + System.getProperty("os.arch") + ".lib";
			libfile = libfile.replace(" ", "_");	
		}
		System.out.println("Trying to load libraries from " + libfile);
		URL ulli = Platform.getBundle("jnexus").getEntry(libfile);
		if(ulli == null){
			System.out.println(libfile + " library information file not found");
			return;
		}
		BufferedReader in = new BufferedReader(new InputStreamReader(ulli.openStream()));
		while((line = in.readLine()) != null){
			System.out.println(System.mapLibraryName(line));
			try{
				System.loadLibrary(line);
			}catch(Exception e){
				System.out.println("Failed to load library: " + line);
			}
		}
		in.close();
	}

}
