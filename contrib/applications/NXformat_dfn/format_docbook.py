#!/usr/bin/env python

from format_xml import *

def get_def_ext():
    return "docbook"

class format_docbook(format_xml):
    """Formatting object that goes through a DOM tree and puts it out
    as a space formatted XML document surrounded by docbook tagsfor
    easier reading in a simple text editor."""

    def format_head(self):
        """Put a header on the resulting document"""
        result="<example id=\"%s.xml\"><title>" % self.main_name
        result=result+"<filename>%s.xml</filename></title>\n" % self.main_name
        result=result+"<programlisting role=\"XML\">\n"
        result=result+"<![CDATA["
        return result

    def format_foot(self):
        """Put a footer on the resulting document"""
        return "]]>\n"+"</programlisting>\n"+"</example>\n"
