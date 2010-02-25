#!env python

"""
Form to convert NeXus XML definition files into formatted HTML
R. Osborn 2002/4/6
"""

import cgitb; cgitb.enable()
import cgi, os, re
from StringIO import *
from urllib import *
from xml.dom.minidom import *

rootURL = "http://127.0.0.1/nexus/"
XMLdir = rootURL+"xml"
homeLink = '<a href="'+rootURL+'">NeXus Home Page</a>'
glossaryLink = '<a href="'+rootURL+'NeXus_contents.html#Glossary">NeXus Glossary</a>'
instrumentLink = '<a href="'+rootURL+'NeXus_instruments.html">NeXus Instruments</a>'
metaLink = '<a href="'+rootURL+'NeXus_metaformat.html">NeXus Meta-DTD Format</a>'
reComment = re.compile(r"\<\!--([\s\S]*)--\>")

def FormatStartTag(node):
    "Write element name and attributes as a start tag"
    buffer = StringIO()
    try:
        node.tagName.index("NX")
        class_ = "group"
    except ValueError:
        class_ = "data"
    buffer.write('<span class="%s"><b>' % class_)
    buffer.write(cgi.escape('<'))
    try:
        node.tagName.index("NX")
        buffer.write('<a href="/cgi-bin/NXconvert.py?file='+node.tagName+'.xml">')
        buffer.write(node.tagName)
        buffer.write('</a>')
    except ValueError:
        buffer.write(node.tagName)
    if node.hasAttributes(): 
        buffer.write('</b></span>')
        buffer.write(FormatAttributes(node))
        buffer.write('<span class="%s"><b>' % class_)
    buffer.write(cgi.escape('>'))
    buffer.write('</b></span>\n')
    return buffer.getvalue()

def FormatEndTag(node):
    "Write element name as an end tag"
    buffer = StringIO()
    try:
        node.tagName.index("NX")
        class_ = "group"
    except ValueError:
        class_ = "data"
    buffer.write('<span class="%s"><b>' % class_)
    buffer.write(cgi.escape('</'+node.tagName+'>'))
    buffer.write('</b></span>\n')
    return buffer.getvalue()

def FormatAttributes(node):
    "Write element attributes"
    buffer = StringIO()
    buffer.write('<span class="attribute">')
    keys = node.attributes.keys()
    for key in keys:
        buffer.write(' '+key+'="<span class="attributevalue">')
        buffer.write(node.attributes[key].value)
        buffer.write('</span>"')
    buffer.write('</span>')
    return buffer.getvalue()

def FormatXMLTag():
    "Write XML definition tag"
    buffer = StringIO()
    buffer.write('<span class="comment">')
    buffer.write(cgi.escape('<?xml version="1.0" ?>'))
    buffer.write('</span>\n')
    return buffer.getvalue()

def HTMLelement(node, indent="", 
                addindent="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"):
    "Write HTML formatting for each type of node"
    buffer = StringIO()
    if node.nodeType == node.DOCUMENT_NODE:
        for childNode in node.childNodes:
            buffer.write(HTMLelement(childNode,indent,addindent))
    elif node.nodeType == node.ELEMENT_NODE:
        buffer.write('<br \>'+indent+FormatStartTag(node))
        for childNode in node.childNodes:
            buffer.write(HTMLelement(childNode,indent+addindent,addindent))
        if len(node.childNodes)>1: buffer.write('<br>'+indent)
        buffer.write(FormatEndTag(node))
    elif node.nodeType == node.TEXT_NODE:
        text = node.data.strip()
        if len(text)>0:
            buffer.write('<font color="#990099">')
            buffer.write(text)
            buffer.write('</font>\n')
    return buffer.getvalue()

def HTMLhead(title):
    "Return string for head of HTML page"
    buffer = StringIO()
    buffer.write('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"\n')
    buffer.write('"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">\n')
    buffer.write('<html xmlns="http://www.w3.org/1999/xhtml">\n')
    buffer.write('<head>\n')
    buffer.write('<title>'+title+'</title>\n')
    buffer.write('<style type="text/css">\n')
    buffer.write('/* <![CDATA[ */\n')
    buffer.write('.xmltag {color: #999999}\n')
    buffer.write('.comment {color: #000066}\n')
    buffer.write('.group {color: #FF0000}\n')
    buffer.write('.data {color: #000099}\n')
    buffer.write('.datavalue {color: #FF00FF}\n')
    buffer.write('.attribute {color: #660033}\n')
    buffer.write('.attributevalue {color: #339900}\n')
    buffer.write('/* ]]> */\n')
    buffer.write('</style>\n')
    buffer.write('</head>\n')
    buffer.write('<body>\n')
    buffer.write('<p align="center">')
    buffer.write('<a href="../nexus/"><img align=top src="../nexus/nexus.gif" border=0 width=597 height=90></a>\n')
    buffer.write('</p>\n')
    buffer.write('<h1>'+title+'</h1>\n')
    buffer.write('<hr \>\n')
    buffer.write(FormatXMLTag())
    return buffer.getvalue()
        
def HTMLcomments(text):
    "Return string for foot of HTML page"
    buffer = StringIO()
    commentSearch = reComment.search(text)
    if commentSearch:
        comment = cgi.escape(commentSearch.group())
        buffer.write('<span class="comment">')
        buffer.write('<br>'+comment.replace('\n','<br \>'))
        buffer.write('</span>\n')
        return buffer.getvalue()
    else:
        pass

def HTMLfile(file_name):
    "Return string for foot of HTML page"
    buffer = StringIO()
    buffer.write('<hr \>\n')
    buffer.write('<p class="center">\n')
    buffer.write('[<a href="http://www.neutron.anl.gov/nexus/xml/'+file_name+'">')
    buffer.write('Download '+file_name+'</a>]\n')
    buffer.write('</p>\n')
    return buffer.getvalue()

def HTMLfoot(links):
    "Return string for foot of HTML page"
    buffer = StringIO()
    buffer.write('<hr \>\n')
    buffer.write('<p align="center">\n')
    buffer.write('['+' | '.join(links)+']\n')
    buffer.write('</p>\n')
    buffer.write('</body>\n')
    buffer.write('</html>\n')
    return buffer.getvalue()

def ConvertToHTML(text):
    "Convert XML text into formatted HTML"
    doc = parseString(text)
    return HTMLelement(doc)

def main():
    "Output XML file as formatted HTML"
    try:
        form = cgi.FieldStorage()
        print 'Content-type: text/html'
        print
        if form.has_key("file"):
            file_name = form["file"].value
            file = urlopen(os.path.join(XMLdir,file_name))
            text = file.read()
            file.close()
            print HTMLhead(file_name)
            print HTMLcomments(text)
            doc = parseString(text)
            doc.normalize()
            print HTMLelement(doc)
            print HTMLfile(file_name)
            print HTMLfoot([homeLink,glossaryLink,instrumentLink,metaLink])
    except:
        cgi.print_exception()

main()

