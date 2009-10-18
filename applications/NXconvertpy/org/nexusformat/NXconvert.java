package org.nexusformat;

import java.io.File;
import java.io.IOException;

class NXconvert {
  private static final String NXCONVERT = "nxconvert";
  public static final String EXTENSION = ".reduced";
  private static final Logger LOG = Logger.getInstance();

  private File rawfile;
  private File redfile;

  NXconvert(final String filename, final boolean keepTemp)
                                   throws IOException, InterruptedException {
    this.rawfile = new File(filename);
    this.redfile = File.createTempFile(this.rawfile.getName() + ".",
                                       EXTENSION);
    if (!keepTemp) {
      this.redfile.deleteOnExit();
    }
  }
  
  private void printStd(final String command, final String out,
                        final String err) {
	  LOG.info(command);
	  LOG.info(out);
	  LOG.warn(err);
  }

  String convert() throws IOException, InterruptedException {
	  LOG.info("Creating " + this.redfile.getAbsolutePath());

    // the command to run
    String command = NXCONVERT + " -d " + this.rawfile + " " + this.redfile;

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
    this.printStd(command, out.getOutput(), err.getOutput());
    if (exitValue != 0) {
      StringBuilder buffer = new StringBuilder("Execute command \"");
      buffer.append(command);
      buffer.append("\" failed. Returned ");
      buffer.append(exitValue);
      throw new IOException(buffer.toString());
    }
    return this.redfile.getAbsolutePath();
  }
  
  public static void main(String[] args) {
	  Logger.setLevel(Logger.DEBUG);
    if (args.length != 1) {
    	LOG.error("Must specify one input file");
      return;
    }
    try {
      NXconvert convert = new NXconvert(args[0], false);
      convert.convert();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
