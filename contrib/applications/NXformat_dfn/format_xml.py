#!/usr/bin/env python

# default value for options
default_width=80
default_indent=2

def trim(string):
    """Trim leading and trailing whitespace off of the supplied string"""
    # default value for trimming
    begin=0
    end=string.__len__()-1 # index of last char is one less
                           # than string size
    # determine amount of leading whitespace
    while string[begin].isspace() and begin<end:
        begin=begin+1
    # determine amount of trailing whitespace
    while string[end].isspace() and end>begin:
        end=end-1
    # return shortened result
    if begin+1>=end:
        return ""
    else:
        return string[begin:end+1]

def remove_extra_spaces(string):
    """Condense all whitespace into ' '"""
    # first remove leading and trailing space
    temp_string=trim(string)

    # set up for loop
    is_space=False
    result=""

    # go through entire string, condensing all spaces into " "
    for letter in temp_string:
        if letter.isspace(): # look for a space
            if not is_space: # turn multiple spaces into one
                is_space=True
                result=result+" "
        else: # just copy the result
            is_space=False
            result=result+letter

    # remove the shortened result
    return result

def get_command_line_doc():
    result = " --width     Specify the width the the document for wrapping. This does not \n"+ \
             "             apply to open or close tags. The default is %d.\n" %default_width + \
             " --indent    Specify the number of spaces for each indentation level. The \n" + \
             "             default is %d." % default_indent
    return result

class format_xml(object):
    """Formatting object that goes through a DOM tree and puts it out
    as a space formatted XML document for easier reading in a simple
    text editor."""
    def __init__(self,main_name,options={},indent=2):
        # variable for tracking indent level
        self.__cur_indent=0

        # option for how wide text can go before wrapping
        try:
            self.__width=int(options.pop("--width"))
        except KeyError:
            self.__width=default_width

        # option for how many spaces to use for each indentation level
        try:
            self.__indent=int(options.pop("--indent"))
        except KeyError:
            self.__indent=default_indent

        # name of definition being formatted
        self.main_name=main_name

    def __setattr__(self,name,value):
        """This does not allow the user to set the value of the
        attributes. This is an immutable object"""
        if name=="width":
            raise AttributeError("Cannot set value of immutable \"width\"")
        if name=="indent":
            raise AttributeError("Cannot set value of immutable \"indent\"")
        if name=="cur_indent":
            raise AttributeError("Cannot set value of immutable \"cur_indent\"")
        self.__dict__[name]=value

    # allow access to the attribute values
    def __getattr__(self,name):
        if name=="width":
            return int(self.__width)
        if name=="indent":
            return int(self.__indent)
        if name=="cur_indent":
            return int(self.__cur_indent)
        return self.__dict__[name]

    def format_head(self):
        """Put a header on the resulting document"""
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"

    def format_foot(self):
        """Put a footer on the resulting document"""
        return ""

    def format_data(self,data):
        """Format the data for the output file"""
        # determine the indentation
        indent=self.__cur_indent*" "

        # create the idiot version
        result="%s%s\n" % (indent,data)

        # return the idiot version if it works
        if(result.__len__()<=self.__width):
            return result

        # generate an array of strings of the correct with for formatting
        str_array=[]
        while result.__len__()>self.__width:
            line=result[0:self.__width]
            while not line[-1].isspace():
                line=line[0:-1]
            str_array.append(line+"\n")
            result=indent+trim(result[line.__len__():])
        str_array.append(result+"\n")

        # convert the string array into a single string
        result=""
        for str in str_array:
            result=result+str

        return result

    def format_attributes(self,node):
        """Format the node attributes for the output file"""
        # return empty string if nothing found
        if not node.hasAttributes():
            return ""

        # put together a string of the formatted attributes
        string=""
        for key in node.attributes.keys():
            value=node.attributes[key].value
            string=string+(" %s=\"%s\"" % (key,value))

        return string

    def format_node(self,file,tag,node):
        """Format a node for the output file"""
        # format the attributes
        attr_string=self.format_attributes(node)
        # create the open tag
        file.write("%s<%s%s>\n" % ((" "*self.__cur_indent),tag,attr_string))
        # increase the indent level
        self.__cur_indent=self.__cur_indent+self.__indent
        # recurse down into the tree
        for child in node.childNodes:
            try:
                child_tag=child.tagName
                self.format_node(file,child_tag,child)
            except AttributeError:
                data=remove_extra_spaces(child.data)
                if(data.__len__()):
                    file.write(self.format_data(data))
        # decrease the indent level
        self.__cur_indent=self.__cur_indent-self.__indent
        # create the close tag
        file.write("%s</%s>\n" % ((" "*self.__cur_indent),tag))

    def format(self,doc,file): # take from NXconvert.HTMLelement
        """The public method for formating a DOM"""
        # put in the header
        file.write(self.format_head())
        # format the body
        for childNode in doc.childNodes:
            try:
                tag=childNode.tagName
                self.format_node(file,tag,childNode)
            except AttributeError:
                try:
                    file.write("<!--\n%s\n-->\n" % trim(childNode.data))
                except AttributeError:
                    pass
        # put in the footer
        file.write(self.format_foot())
