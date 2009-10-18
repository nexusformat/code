package org.nexusformat;

import java.io.File;
import java.io.IOException;

class NXconvert {
  private static final String NXCONVERT = "nxconvert";
  public static final String EXTENSION = ".reduced";
  private File rawfile;
  private File redfile;
  private int verbose;

  NXconvert(final String filename, final boolean keepTemp, final int verbose)
                                   throws IOException, InterruptedException {
    this.rawfile = new File(filename);
    this.redfile = File.createTempFile(this.rawfile.getName() + ".",
                                       EXTENSION);
    if (!keepTemp) {
      this.redfile.deleteOnExit();
    }
    this.verbose = verbose;
  }
  
  private void printStd(final String command, final String out,
                        final String err) {
    if (this.verbose > 1) {
      System.out.println(command);
      if (out.length() > 0) {
        System.out.print(out);
      }
      if (err.length() > 0) {
        System.out.print(err);
      }
    }
  }

  String convert() throws IOException, InterruptedException {
    if (verbose > 0) {
      System.out.println("Creating " + this.redfile.getAbsolutePath());
    }

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
    if (args.length != 1) {
      System.out.println("Must specify one input file");
      return;
    }
    try {
      NXconvert convert = new NXconvert(args[0], false, 1);
      convert.convert();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
