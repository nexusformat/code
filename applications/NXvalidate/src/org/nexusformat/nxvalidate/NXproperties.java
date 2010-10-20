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
 * NXproperties.java
 *
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
 * @author Stephen Rankin
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
               userHome + File.pathSeparator + ".NXvalidate.properties";

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

           userProps = ResourceBundle.getBundle(".NXvalidate.properties",
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
