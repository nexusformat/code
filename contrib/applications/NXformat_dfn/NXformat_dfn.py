#!/usr/bin/env python

import format_xml
import StringIO
from xml.dom.minidom import parse

class Options:
    def __init__(self,indent=2,width=80):
        self.cur_indent=0
        self.indent=indent
        self.width=width

def get_root_name(file):
    """Determine the root of the filename assuming that it either ends
    with '.xml' or is already a root"""
    ext=".xml"
    if file.endswith(ext):
        return file[:-1*ext.__len__()]
    else:
        return file

def print_usage(command,level=0):
    print "USAGE:",command,"[options] <definition file>"
    print "      where <definition file> is the name of file to format"
    if(level<1): # done after simple usage statement
        return
    print ""
    print "This will format the file and print the the result to STDOUT using the \"XML\""
    print "formatter unless specified otherwise."

    print ""
    print "GENERAL:"
    print " -d|--debug  Increase debug level."
    print " --format    Specify the formatter to use. The options are \"xml\"."
    print "             \"xml\" is the default."
    print " -h|--help   Print this help information."
    print " -o          Name of output file. If not specified uses STDOUT."
    print " --root      Specify the name of the definition."

    print ""
    print "For XML formatter:"
    print format_xml.get_command_line_doc()

def main(infile, outfile, options, DEBUG=1):
    # parse the xml file
    if DEBUG: print "Processing \"%s\"" % infile
    doc=parse(infile)
    if DEBUG: print "          ... done"

    # determine the input name
    try:
        root_name=options.pop("--root")
    except KeyError:
        root_name=get_root_name(infile)

    # open the output file or create the buffer to write to
    if outfile:
        buffer=open(outfile,"w")
    else:
        buffer=StringIO.StringIO()

    # get the appropriate formatter
    try:
        format_option=options.pop("--format")
    except KeyError:
        format_option="xml"
    format_option=format_option.upper()
    if DEBUG: print "Creating %s formatter" % format_option
    if format_option=="XML":
        formatter=format_xml.format_xml(root_name,options)
    else:
        raise KeyError("no formatter named \"%s\"" % format_option)
    if DEBUG: print "          ... done"

    # format the DOM directly to the file buffer
    if DEBUG: print "Formatting information"
    formatter.format(doc,buffer)
    if DEBUG: print "          ... done"

    # if we are actually writting to STDOUT then print there now
    if not outfile:
        string=buffer.getvalue()
        if(string):
            print string,
        else:
            print "EMPTY RESULT"

if __name__ == "__main__":
    # sys module only needed for command line operation
    import sys

    # remove the program name from the command line arguments
    progname=sys.argv[0]
    sys.argv.remove(progname)

    # confirm that there are enough arguments to even bother
    if sys.argv.__len__()<1:
        print_usage(progname)
        sys.exit(-1)

    # parse the command line options
    options={}
    debug=0
    outfile=""
    while sys.argv.__len__()>0:
        key=sys.argv[0]
        sys.argv=sys.argv[1:]
        if key.startswith("-"):
            if key=="--help" or key=="-h": # print help
                print_usage(progname,100)
                sys.exit(0)
            elif key=="--debug" or key=="-d": # increase debug level
                debug=debug+1
            else:
                # get key and value
                index=key.index("=")
                if(index>=0):
                    value=key[index+1:]
                    key=key[:index]
                elif(sys.argv.__len__()>0):
                    value=sys.argv[0]
                    sys.argv=sys.argv[1:]
                # check for special keys
                if key=="-o": # output file selected
                    outfile=value
                else: # everything else goes into the options slop bucket
                    options[key]=value
        else:
            infile=key
        print options

    # pass the command line options to main
    main(infile,outfile,options,debug)
