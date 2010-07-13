package org.nexusformat.nxvalidate;

abstract class AbstractNXSnode {
    private String name;

    AbstractNXSnode(final String name) {
	this.name = name;
    }

    final protected void setName(final String name) {
	this.name = name;
    }

    final public String getName() {
	return this.name;
    }
}