/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Stephen Rankin
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information, see <http://www.nexusformat.org/>
 *
 * UserSettings.java
 *
 */

package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Map;
import java.util.Properties;
import java.util.ResourceBundle;
import java.util.logging.Level;
import javax.swing.JOptionPane;
import java.util.logging.Logger;
/**
 *
 * @author Stephen Rankin
 */
public class UserSettings {

    private Properties props = null;
    private String nxconvertCommand = null;
    private ResourceBundle bundle = null;
    private boolean foundNXconvert = false;

    
    public UserSettings(){
        props = new Properties();
        bundle = ResourceBundle.getBundle(
                "org/nexusformat/nxvalidate/resources/nxvalidate");
    }

    public void loadUserSettings() throws FileNotFoundException, IOException {

        props = new Properties();

        Map<String, String> env = System.getenv();
        for (String envName : env.keySet()) {
            System.out.format("%s=%s%n", envName, env.get(envName));
        }

        File settings = new File(System.getProperty("user.home")
                + System.getProperty("file.separator")
                + ".nxvalidate.properties");

        if (settings.exists()) {
            props.load(new FileInputStream(settings));
            if (props.getProperty("nxconvert") != null) {
                
                if(chechExists(props.getProperty("nxconvert"))){
                    nxconvertCommand = props.getProperty("nxconvert");
                    foundNXconvert = true;
                }
                else{
                    defaultNXconvert();
                }
            }
            else{
                defaultNXconvert();
            }
        } else {
            settings.createNewFile();
            defaultNXconvert();
        }

    }

    public void saveUserSettings() {

        File settings = new File(System.getProperty("user.home")
                + System.getProperty("file.separator") + ".nxvalidate.properties");
        try {
            if (settings.exists()) {

                props.setProperty("nxconvert", nxconvertCommand);
                props.store(new FileOutputStream(settings), "NXvalidate");

            } else {
                settings.createNewFile();
                props.setProperty("nxconvert", nxconvertCommand);
                props.store(new FileOutputStream(settings), "NXvalidate");

            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    "saveUserSettings(): The settings file cannot be found.", ex);
        } catch (IOException ex) {
            Logger.getLogger(
                    NXvalidateFrame.class.getName()).log(Level.SEVERE,
                    "saveUserSettings(): The settings file IO error.", ex);
        }
    }

    public String getNXconvert(){
        return nxconvertCommand;
    }

    public void setNXconvert(String nxconvertCommand){
        this.nxconvertCommand = nxconvertCommand;
        if(testPath(nxconvertCommand)){
            foundNXconvert = true;
        }
        else{
            foundNXconvert = false;
        }
    }

    public boolean foundNXconvert(){
        return foundNXconvert;
    }

    private void defaultNXconvert(){
        OSValidator os = new OSValidator();

        if(os.isWindows()){
            nxconvertCommand = bundle.getString("defaultWindowsNXconvert");
        } else if(os.isMac()){
            nxconvertCommand = bundle.getString("defaultMacNXconvert");
        } else if(os.isUnix()){
            nxconvertCommand = bundle.getString("defaultUNIXNXconvert");
            if(!chechExists(nxconvertCommand)){
                nxconvertCommand = bundle.getString("defaultUNIXNXconvert2");
            }
        }

        if(!chechExists(nxconvertCommand)){
            nxconvertCommand = "nxconvert";
        }

        if(testPath(nxconvertCommand)){
            System.out.println("defaultNXconvert foundNXconvert:" + foundNXconvert);
            foundNXconvert = true;
        }

    }

    private boolean chechExists(String filename){

        File file = null;
        boolean exists = false;
        if(filename!=null){

            if(!filename.equals("")){
                file = new File(filename);
            }
            else{
                return false;
            }

            if(file.exists()){
                exists = true;
            }
            
        }
        return exists;

    }

    private boolean testPath(String path){

        boolean result = false;

        if(path == null){
            return result;
        }
        if(path.equals("")){
            return result;
        }
        
        try {
            // execute the command
            Runtime rt = Runtime.getRuntime();
            Process proc = rt.exec(path + " --help");
            result = true;
            return result;
        } catch (IOException ex) {
            return result;
        }

    }

}
