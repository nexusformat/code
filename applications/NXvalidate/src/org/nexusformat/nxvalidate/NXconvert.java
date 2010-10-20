/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Nexus Team
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
 * NXconvert.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.nexusformat.nxvalidate.exceptions.NXvalidateException;

public class NXconvert {

    private static final String NXCONVERT = "nxconvert";
    private static final String EXTENSION = ".reduced";
    private File rawfile;
    private File redfile;
    private NXproperties props = null;
    private String convertCommand = null;

    public NXconvert(final File rawfile, final boolean keepTemp,
            final String convertCommand)
            throws IOException, InterruptedException {

        this.rawfile = rawfile;
        redfile = File.createTempFile(rawfile.getName() + ".",
                EXTENSION);
        if (!keepTemp) {
            redfile.deleteOnExit();
        }

        this.convertCommand = convertCommand;

        props = new NXproperties();

    }

    private void printStd(final String command, final String out,
            final String err) {

        Logger.getLogger(NXconvert.class.getName()).log(Level.INFO, command);
        Logger.getLogger(NXconvert.class.getName()).log(Level.INFO, out);
        Logger.getLogger(NXconvert.class.getName()).log(Level.WARNING, err);

    }

    public File convert() throws IOException, InterruptedException, NXvalidateException {

        Logger.getLogger(NXconvert.class.getName()).log(
                Level.INFO, "Creating " + redfile.getAbsolutePath());

        String command = "";
        if(convertCommand!=null){
            command = convertCommand +
                " -d " + this.rawfile + " " + redfile;;
        }
        else{
            return null;
        }

        // execute the command
        Runtime rt = Runtime.getRuntime();
        Process proc = rt.exec(command);

        // get the output and error
        StreamGobbler out = new StreamGobbler(proc.getInputStream(), "OUTPUT");
        StreamGobbler err = new StreamGobbler(proc.getErrorStream(), "ERROR");
        out.start();
        err.start();
        int exitValue = proc.waitFor();

        // cleanup and return result
        printStd(command, out.getOutput(), err.getOutput());
        if (exitValue != 0) {
            StringBuilder buffer = new StringBuilder("Execute command \"");
            buffer.append(command);
            buffer.append("\" failed. Returned ");
            buffer.append(exitValue);
            throw new NXvalidateException(buffer.toString());
        }
        return redfile;
    }

    public static void main(String[] args) {
        
        if (args.length != 1) {
            Logger.getLogger(NXconvert.class.getName()).log(Level.SEVERE,
                    "You must specify one input file");
            return;
        }
        try {
            NXconvert convert = new NXconvert(new File(args[0]), false, args[1]);
            convert.convert();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
