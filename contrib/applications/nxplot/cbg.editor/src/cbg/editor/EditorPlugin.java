package cbg.editor;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.jface.preference.IPreferenceStore;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.ImageRegistry;
import org.eclipse.swt.graphics.Image;
import org.eclipse.ui.editors.text.EditorsUI;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.eclipse.ui.texteditor.ChainedPreferenceStore;
import org.osgi.framework.BundleContext;

import cbg.editor.rules.ColorManager;

/**
 * The main plugin class to be used in the desktop.
 */
public class EditorPlugin extends AbstractUIPlugin {
    public static final String STATUS_CATEGORY_MODE = "cbg.editor.modeStatus";

    public ColorManager getColorManager() {
        if (colorManager == null) {
            colorManager = new ColorManager(getPreferenceStore());
        }
        return colorManager;
    }

    public ColoringEditorTools getEditorTools() {
        if (editorTools == null) editorTools = new ColoringEditorTools();
        return editorTools;
    }

    // The shared instance.
    private static EditorPlugin plugin;
    // Resource bundle.
    private ResourceBundle resourceBundle;
    private ColoringEditorTools editorTools;
    private ColorManager colorManager;
    private IPreferenceStore fCombinedPreferenceStore;

    /**
     * Returns the shared instance.
     */
    public static EditorPlugin getDefault() {
    	//In Eclipse 3.1.X this can be null
    	if(plugin == null) {
    		new EditorPlugin();
    	}
        return plugin;
    }

    /**
     * Returns the workspace instance.
     */
    public static IWorkspace getWorkspace() {
        return ResourcesPlugin.getWorkspace();
    }

    public EditorPlugin() {
        super();
        plugin = this;
    }
    
    /**
     * Returns the string from the plugin's resource bundle, or 'key' if not
     * found.
     */
    public static String getResourceString(String key) {
        ResourceBundle bundle = EditorPlugin.getDefault().getResourceBundle();
        try {
            return bundle.getString(key);
        } catch (MissingResourceException e) {
            return key;
        }
    }

    public static ImageDescriptor getImageDescriptor(String imageName) {
        String iconPath = "icons/";
        try {
            URL installURL = Platform.getBundle("cbg.editor").getEntry("/");
            URL url = new URL(installURL, iconPath + imageName);
            return ImageDescriptor.createFromURL(url);
        } catch (MalformedURLException e) {
            // should not happen
            return ImageDescriptor.getMissingImageDescriptor();
        }
    }

    /**
     * Returns the plugin's resource bundle,
     */
    public ResourceBundle getResourceBundle() {
        return resourceBundle;
    }

    protected void initializeImageRegistry(ImageRegistry reg) {
        super.initializeImageRegistry(reg);
        reg.put("box", getImageDescriptor("boxIcon.gif").createImage());
    }

    public static Image getImage(String handle) {
        return getDefault().getImageRegistry().get(handle);
    }

    public static void log(String message, Throwable e) {
        getDefault().getLog().log(new Status(IStatus.ERROR, getPluginId(), IStatus.OK, message, e)); //$NON-NLS-1$
    }
    
    public static String getPluginId() {
        return "cbg.editor";
    }

    public IPreferenceStore getCombinedPreferenceStore() {
        if (fCombinedPreferenceStore == null) {
            IPreferenceStore generalTextStore= EditorsUI.getPreferenceStore(); 
            fCombinedPreferenceStore= new ChainedPreferenceStore(new IPreferenceStore[] { getPreferenceStore(), generalTextStore });
        }
        return fCombinedPreferenceStore;
    }
    
    public void start(BundleContext context) throws Exception {
    	super.start(context);
    }
}
