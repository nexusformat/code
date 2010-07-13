package org.nexusformat.nxvalidate;

import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NXconvert {

    private static final String NXCONVERT = "nxconvert";
    private static final String EXTENSION = ".reduced";
    private File rawfile;
    private File redfile;
    private NXproperties props = null;

    public NXconvert(final File rawfile, final boolean keepTemp)
            throws IOException, InterruptedException {

        this.rawfile = rawfile;
        redfile = File.createTempFile(rawfile.getName() + ".",
                EXTENSION);
        if (!keepTemp) {
            redfile.deleteOnExit();
        }

        props = new NXproperties();

    }

    private void printStd(final String command, final String out,
            final String err) {

        Logger.getLogger(NXconvert.class.getName()).log(Level.INFO, command);
        Logger.getLogger(NXconvert.class.getName()).log(Level.INFO, out);
        Logger.getLogger(NXconvert.class.getName()).log(Level.WARNING, err);

    }

    public File convert() throws IOException, InterruptedException {

        Logger.getLogger(NXconvert.class.getName()).log(
                Level.INFO, "Creating " + redfile.getAbsolutePath());

        // the command to run
        String command = props.getProperty(NXCONVERT) +
                " -d " + this.rawfile + " " + redfile;

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
            throw new IOException(buffer.toString());
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
            NXconvert convert = new NXconvert(new File(args[0]), false);
            convert.convert();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
