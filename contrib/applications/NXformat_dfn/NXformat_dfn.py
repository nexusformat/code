#!/usr/bin/env python

import format_docbook
import format_xml
import StringIO
from xml.dom.minidom import parse

def copy_dictionary(orig):
    copy={}
    for key in orig.keys():
        copy[key]=orig[key]
    return copy

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
    print "This will format the file using the \"XML\" formatter unless specified."

    print ""
    print "GENERAL:"
    print " -d|--debug  Increase debug level."
    print " --docbook   Use the docbook formatter."
    print " --format    Specify the formatter to use. The options are \"xml\""
    print "             and \"docbook\". \"xml\" is the default."
    print " -h|--help   Print this help information."
    print " -o          Name of output file. If not specified uses STDOUT."
    print " --root      Specify the name of the definition."
    print " --xml       Use the xml formatter."

    print ""
    print "For XML formatter: (default extension is \"%s\")" \
          % format_xml.get_def_ext()
    print format_xml.get_command_line_doc()

    print ""
    print "For DOCBOOK formatter: (default extension is \"%s\")" \
          % format_docbook.get_def_ext()
    print format_docbook.get_command_line_doc()

def process_file(infile, outfile, options, DEBUG=1):
    # parse the xml file
    if DEBUG: print "Processing \"%s\"" % infile
    doc=parse(infile)
    if DEBUG: print "          ... done"

    # determine the input name
    try:
        root_name=options.pop("--root")
    except KeyError:
        root_name=get_root_name(infile)

    # determine the proper formatter
    try:
        format_option=options.pop("--format")
    except KeyError:
        format_option="xml"
    format_option=format_option.upper()

    # if output file name not supplied create the default one
    if not outfile:
        ext=""
        if(format_option=="XML"):
            ext=format_xml.get_def_ext()
        elif(format_option=="DOCBOOK"):
            ext=format_docbook.get_def_ext()
        outfile="%s.%s" % (root_name,ext)

    # open the output file or create the buffer to write to
    if DEBUG: print "Writing result to \"%s\"" % outfile
    if DEBUG<=0:
        buffer=open(outfile,"w")
    else:
        buffer=StringIO.StringIO()

    # get the appropriate formatter
    if DEBUG: print "Creating %s formatter" % format_option
    if format_option=="XML":
        formatter=format_xml.format_xml(root_name,options)
    elif format_option=="DOCBOOK":
        formatter=format_docbook.format_docbook(root_name,options)
    else:
        raise KeyError("no formatter named \"%s\"" % format_option)
    if DEBUG: print "          ... done"

    # format the DOM directly to the file buffer
    if DEBUG: print "Formatting information"
    formatter.format(doc,buffer)
    if DEBUG: print "          ... done"

    # if we are actually writting to STDOUT then print there now
    if DEBUG:
        string=buffer.getvalue()
        if(string):
            print string,
        else:
            print "EMPTY RESULT"

def main(infile, outfile, options, DEBUG=1):
    # deal with processing a single file
    if not infile.__class__==[].__class__:
        process_file(infile,outfile,options,DEBUG)
        return

    # confirm that we are working without an output file name
    if outfile:
        print "Cannot specify output filename in multiple file processing mode"
        sys.exit(-1)

    # deal with no output file specified
    for file in infile:
        options_copy=copy_dictionary(options)
        process_file(file,"",options_copy,DEBUG)

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
    infile=[]
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
            elif key=="--docbook": # use the docbook formatter
                options["--format"]="docbook"
            elif key=="--xml": # use the xml formatter
                options["--format"]="xml"
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
            infile.append(key)

    if infile.__len__()==1:
        infile=infile[0]

    # pass the command line options to main
    main(infile,outfile,options,debug)
