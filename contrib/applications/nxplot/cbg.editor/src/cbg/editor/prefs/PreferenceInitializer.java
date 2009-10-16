package cbg.editor.prefs;

import org.eclipse.core.runtime.preferences.AbstractPreferenceInitializer;
import org.eclipse.jface.preference.IPreferenceStore;

import cbg.editor.ColoringSourceViewerConfiguration;
import cbg.editor.EditorPlugin;
import cbg.editor.rules.ColorManager;

public class PreferenceInitializer extends AbstractPreferenceInitializer {

    public PreferenceInitializer() {
        super();
    }

    public void initializeDefaultPreferences() {
        IPreferenceStore store = EditorPlugin.getDefault().getPreferenceStore();
        ColorManager.initDefaultColors(store);
        store.setDefault(ColoringSourceViewerConfiguration.SPACES_FOR_TABS, false);
        store.setDefault(ColoringSourceViewerConfiguration.PREFERENCE_TAB_WIDTH, 4);
    }

}
