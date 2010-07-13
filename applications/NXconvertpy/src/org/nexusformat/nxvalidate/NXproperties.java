/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.nexusformat.nxvalidate;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Properties;
import java.util.ResourceBundle;

/**
 *
 * @author ser65
 */
public class NXproperties {

    private ResourceBundle classpathProps = null;
    private ResourceBundle userProps = null;
    private Properties systemProps = null;
    private String userHome = null;

    public NXproperties() throws MalformedURLException
    {

       classpathProps  =
               ResourceBundle.getBundle("org/nexusformat/nxvalidate/resources/nxvalidate");

       systemProps = System.getProperties();

       //First we see if there is a user specific prop file.
       userHome = systemProps.getProperty("user.home");

       String propsFileName =
               userHome + File.pathSeparator + ".NXConvertpy.properties";

       File propsFile = new File(propsFileName);

       URL resourceURL = null;

       if(propsFile.exists()){

           try {

               resourceURL = propsFile.getParentFile().toURI().toURL();

           } catch (MalformedURLException e) {

              throw e;
           }

           URLClassLoader urlLoader =
                   new URLClassLoader(new java.net.URL[]{resourceURL});

           userProps = ResourceBundle.getBundle(".NXConvertpy.properties",
                java.util.Locale.getDefault(), urlLoader );

       }

    }

    public String getProperty(String name){

        String prop = null;

        //If we are using the user props
        if(userProps != null)
        {

            if(userProps.containsKey(name))
            {
                prop = userProps.getString(name);
            }
            
        }
        //If we are using the default props.
        else if(prop == null)
        {
            if(classpathProps.containsKey(name))
            {
                prop = classpathProps.getString(name);
            }
        }

        return prop;

    }

}
