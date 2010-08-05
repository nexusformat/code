/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
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
    private File nxconvertFile = null;
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
                + ".nxconvertpy.properties");

        if (settings.exists()) {
            props.load(new FileInputStream(settings));
            if (props.getProperty("nxconvert") != null) {
                
                if(chechExists(props.getProperty("nxconvert"))){
                    nxconvertFile = new File(props.getProperty("nxconvert"));
                    foundNXconvert = true;
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
                + System.getProperty("file.separator") + ".nxconvertpy.properties");
        try {
            if (settings.exists()) {

                props.setProperty("nxconvert", nxconvertFile.getAbsolutePath());
                props.store(new FileOutputStream(settings), "ISIS NXConvertpy");

            } else {
                settings.createNewFile();
                props.setProperty("nxconvert", nxconvertFile.getAbsolutePath());
                props.store(new FileOutputStream(settings), "ISIS NXConvertpy");

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

    public File getNXconvert(){
        return nxconvertFile;
    }

    public void setNXconvert(File nxconvertFile){
        this.nxconvertFile = nxconvertFile;
        if(nxconvertFile.exists()){
            foundNXconvert = true;
        }
    }

    public boolean foundNXconvert(){
        return foundNXconvert;
    }

    private void defaultNXconvert(){
        OSValidator os = new OSValidator();

        String command = null;

        if(os.isWindows()){
            command = bundle.getString("defaultWindowsNXconvert");
        } else if(os.isMac()){
            command = bundle.getString("defaultMacNXconvert");
        } else if(os.isUnix()){
            command = bundle.getString("defaultUNIXNXconvert");
            if(!chechExists(command)){
                command = bundle.getString("defaultUNIXNXconvert2");
            }
        }

        if(command!=null){
            if(!command.equals("")){
                nxconvertFile = new File(command);
            }
        }

        if(nxconvertFile==null){
            nxconvertFile = new File("nxconvert");
        }

        if(nxconvertFile.exists()){
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

            if(file.exists()){
                exists = true;
            }
            
        }
        return exists;

    }

}
