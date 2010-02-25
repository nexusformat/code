#!/usr/bin/env python

"""
Form to convert NeXus XML definition files into a formatted HTML table
R. Osborn 2004/02/12
"""

import sys, os, re, cgi
from urllib import *
from xml.dom.minidom import *
import HTML

rootURL = "http://www.nexus.anl.gov/"
XMLurl = "http://www.nexus.anl.gov/classes/xml"
XMLdir = "/Users/osborn/Sites/nexus/classes/xml"
HTMLdir = "/Users/osborn/Sites/nexus/classes"
reComment = re.compile(r"\<\!--([\s\S]*?)--\>", re.DOTALL)
reValue = re.compile(r"([\s\S]*)[\?\*\+]")
reShort = re.compile(r"([\s\S]*){([\s\S]*)}", re.DOTALL)
reLong = re.compile(r"([\s\S]*){([\s\S]*)}.*{([\s\S]*)}", re.DOTALL)
reAttribute = re.compile(r"([\s\S]*){([\s\S]*)}", re.DOTALL)

def XMLhead(NXclass):
    "Return string for head of HTML page"
    file = open("NXheader.txt","r")
    header = file.read()
    file.close()
    title = NXclass+" Class"
    output = HTML.headblock(title, header, stylesheet="%s/nexus.css" % rootURL)
    return "%s\n%s" % (output, str(HTML.Header(title)))
        
def XMLcomments(text):
    "Return description of XML file contained in the initial comments"
    comment = cgi.escape(reComment.search(text).groups()[0])
    return str(HTML.Para(comment.replace('\n', '<br \>', 5)))

def XMLtags(doc):
    "Start table"
    table = HTML.Table(["15%", "15%", "50%", "20%"], width="90%",
                       cellspacing="0")
    row = HTML.Row()
    row.Add(HTML.HeaderCell("Name"))
    row.Add(HTML.HeaderCell("Type"))
    row.Add(HTML.HeaderCell("Description"))
    row.Add(HTML.HeaderCell("Attributes"))
    table.Add(row)
    for node in doc.childNodes[1].childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            table.Add(XMLtag(node))
    return str(table)

def XMLtag(node):
    "Write HTML formatting for each type of node"
    row = HTML.Row()
    try:
        node.tagName.index("NX")
        row.Add(HTML.Cell("&nbsp;"))
        url = node.tagName + ".html"
        row.Add(HTML.Cell(HTML.anchor(url, node.tagName)))
    except ValueError:
        row.Add(HTML.Cell(node.tagName))
        NXtype = "NX_CHAR"
        if node.hasAttributes():
            if "type" in node.attributes.keys():
                NXtype = node.attributes["type"].value
        row.Add(HTML.Cell(NXtype))
    if len(node.childNodes) > 0:
        cell = HTML.Cell()
        longSearch = reLong.search(node.childNodes[0].data)
        shortSearch = reShort.search(node.childNodes[0].data)
        valueSearch = reValue.search(node.childNodes[0].data)
        if longSearch:
            value, shortDescription, longDescription = longSearch.groups()
            value = value.strip()
            if value:
                cell.Add(cgi.escape(value))
                if shortDescription: 
                    cell.Add(HTML.br())
            cell.Add(cgi.escape(shortDescription.strip()))
            if shortDescription.strip() and longDescription.strip(): 
                cell.Add(HTML.br())
            cell.Add(HTML.Span(cgi.escape(longDescription.strip()), 
                               class_="small"))
        elif shortSearch:
            value, shortDescription = shortSearch.groups()
            value = value.strip()
            if value:
                cell.Add(cgi.escape(value))
                if shortDescription: 
                    cell.Add(HTML.br())
            cell.Add(cgi.escape(shortDescription.strip()))
        elif valueSearch:
            value, = valueSearch.groups()
            cell.Add(value.strip())
        row.Add(cell)
    else:
        row.Add(HTML.Cell("&nbsp;"))
    cell = HTML.Cell()
    try:
        node.tagName.index("NX")
        cell.Add("&nbsp;")
    except ValueError:
        text = ""
        for key in node.attributes.keys():
            value = node.attributes[key].value
            attributeSearch = reAttribute.search(value)
            if key <> "type":
                if text: text += HTML.br()
                text += key
                if attributeSearch:
                    value, shortDescription = attributeSearch.groups()
                    if value:
                        text += '="%s"' % value
                    text += "%s\n%s" % \
                    (HTML.br(), 
                     HTML.Span(cgi.escape(shortDescription), class_="small"))
                else:
                    text +='="%s"' % node.attributes[key].value
        if text:        
    	    cell.Add(text)
    	else:
    	    cell.Add("&nbsp;")
    row.Add(cell)
    return row
    
def XMLfile(fileName):
    "Return link to the original XML file"
    url = os.path.join(XMLurl, fileName)
    return str(HTML.Para("%s%s%s" 
                         % ("[", HTML.anchor(url, "Download "+fileName), "]"),
                         class_="center"))

def XMLfoot():
    "Return string for foot of HTML page"
    file = open("NXfooter.txt","r")
    footer = file.read()
    file.close()
    return HTML.foot(footer=footer)

def main():
    "Output XML file as formatted HTML"
    NXclass = sys.argv[1]
    fileName = sys.argv[1] + ".xml"
    file = open(os.path.join(XMLdir, fileName))
    text = file.read()
    file.close()
    file = open(os.path.join(HTMLdir, NXclass+".html"), "w")
    file.write(XMLhead(NXclass))
    file.write(XMLcomments(text))
    doc = parseString(text)
    doc.normalize()
    file.write(XMLtags(doc))
    file.write(XMLfile(fileName))
    file.write(XMLfoot())
    file.close()

if __name__ == "__main__":
    main()

