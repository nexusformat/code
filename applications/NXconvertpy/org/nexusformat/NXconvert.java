package org.nexusformat;

import java.io.IOException;

class NXconvert {
  private static final String NXCONVERT = "nxconvert";
  private String command;
  private String rawfile;
  private String redfile;
  private int verbose;

  NXconvert(final String filename) {
    this(filename, 0);
  }

  NXconvert(final String filename, final int verbose) {
    this.rawfile = filename;
    this.redfile = filename + ".reduced";
    this.verbose = verbose;
  }

  private void printStd(final String command, final String out,
                        final String err) {
    if (this.verbose > 0) {
      
    }
  }

  void convert() throws IOException, InterruptedException {
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
  }
}
