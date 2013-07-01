/* http://www.allaboutbalance.com/disableprefs/ */

import java.util.prefs.AbstractPreferences;
import java.util.prefs.BackingStoreException;

public class DisabledPreferences extends AbstractPreferences {

    public DisabledPreferences() {
        super(null, "");
    }

    protected void putSpi(String key, String value) {

    }

    protected String getSpi(String key) {
        return null;
    }

    protected void removeSpi(String key) {

    }

    protected void removeNodeSpi() throws BackingStoreException {

    }

    protected String[] keysSpi() throws BackingStoreException {
        return new String[0];
    }

    protected String[] childrenNamesSpi()
            throws BackingStoreException {
        return new String[0];
    }

    protected AbstractPreferences childSpi(String name) {
        return null;
    }

    protected void syncSpi() throws BackingStoreException {

    }

    protected void flushSpi() throws BackingStoreException {

    }
}



