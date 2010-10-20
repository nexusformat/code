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
 * StreamGobbler.java
 *
 */
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
