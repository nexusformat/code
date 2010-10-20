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
 * NXvalidateException.java
 *
 */

package org.nexusformat.nxvalidate.exceptions;

/**
 *
 * @author Stephen Rankin
 */
public class NXvalidateException extends Exception {

    /**
     * Creates a new instance of <code>NXvalidateException</code> without detail message.
     */
    public NXvalidateException() {
    }


    /**
     * Constructs an instance of <code>NXvalidateException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public NXvalidateException(String msg) {
        super(msg);
    }

    public NXvalidateException(String msg, Throwable cause) {
        super(msg,cause);
    }

    public NXvalidateException(Throwable cause) {
        super(cause);
    }

}
