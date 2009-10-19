package org.nexusformat.nxvalidate;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Thread for consuming streams.
 */
public final class StreamGobbler extends Thread {
  /** The stream that will be read from in the thread. */
  private InputStream stream;

  /** A label for the stream. */
  private String type;

  /** Everything written to the stream. */
  private StringBuffer buffer;

  /**
   * @param stream The stream to consume.
   * @param type The type of the stream.
   */
  public StreamGobbler(final InputStream stream, final String type) {
    this.stream = stream;
    this.type = type;
    this.buffer = new StringBuffer();
  }

  /**
   * @return The type of the stream.
   */
  public String getType() {
    return this.type;
  }

  /** {@inheritDoc} */
  public void run() {
    try {
      InputStreamReader isr = new InputStreamReader(this.stream);
      BufferedReader br = new BufferedReader(isr);
      String line = null;
      while ((line = br.readLine()) != null) {
        buffer.append(line);
        buffer.append("\n");
      }
    }
    catch (IOException ioe) {
      ioe.printStackTrace();
    }

  }

  /**
   * @return Everything that was written to the stream.
   */
  public String getOutput() {
    return buffer.toString();
  }
}
