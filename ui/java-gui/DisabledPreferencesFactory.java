/* http://www.allaboutbalance.com/disableprefs/ */

import java.util.prefs.PreferencesFactory;
import java.util.prefs.Preferences;

public class DisabledPreferencesFactory implements PreferencesFactory {

    public Preferences systemRoot() {
        return new DisabledPreferences();
    }

    public Preferences userRoot() {
        return new DisabledPreferences();
    }
}



