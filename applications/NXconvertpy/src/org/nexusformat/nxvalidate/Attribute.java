package org.nexusformat.nxvalidate;

import java.util.Vector;

public class Attribute extends AbstractNXSnode {
    private static final String ATTR = "ATTR:";
    private String value;

    public Attribute(final String name, final String value) {
	super(name);
	this.value = value;
    }

    public String getValue() {
	return this.value;
    }

    public String toString() {
	return ATTR + this.getName() + ":" + this.value;
    }
}