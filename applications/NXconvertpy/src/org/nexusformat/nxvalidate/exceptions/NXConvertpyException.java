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
 * For further information, see <http://www.neutron.anl.gov/NeXus/>
 *
 * XMLTreeRenderer.java
 *
 */

package org.nexusformat.nxvalidate.exceptions;

/**
 *
 * @author Stephen Rankin
 */
public class NXConvertpyException extends Exception {

    /**
     * Creates a new instance of <code>NXConvertpyException</code> without detail message.
     */
    public NXConvertpyException() {
    }


    /**
     * Constructs an instance of <code>NXConvertpyException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public NXConvertpyException(String msg) {
        super(msg);
    }
}
