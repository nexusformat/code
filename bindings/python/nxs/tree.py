#!/usr/bin/env python
# This program is public domain

"""
Tree view for NeXus files.

Unlike the `nxs.napi` routines which implement the NeXus API directly, the
`nxs.tree` routines preload the entire file structure into memory and use
a natural syntax for navigating the data hierarchy.  Large datasets
are not read until they are needed, and may be read or written one
slab at a time.

There are a number of functions which operate on files::

    import nxs
    tree = nxs.load('file.nxs')   # loads a structure from a file
    nxs.save('copy.nxs', tree)    # saves a structure to a file
    nxs.dir('copy.nxs')           # display the contents of a file


The tree returned from load() has an entry for each group, field and
attribute.  You can traverse the hierarchy using the names of the
groups.  For example, tree.Histogram1.instrument.detector.distance
is a field containing the distance to each pixel in the detector.
NeXus attribute names are tagged with a leading 'A', so for example,
tree.Histogram1.instrument.detector.distance.Aunits contains the
units attribute for the detector distances.

Properties of the nodes in the tree are referenced by nx attributes.
Depending on the node type, different nx attributes may be available.

Nodes (class NXnode) have attributes shared by both groups and fields::
    * nxname     node name
    * nxclass    node class for groups, 'SDS' for fields
    * nxgroup    group containing the entry, or None for the root
    * nxattrs    dictionary of NeXus attributes for the node

Groups (class NXgroup) have attributes for accessing children::
    * nxentries  dictionary of entries within the group
    * nxcomponent('nxclass')  return group entries of a particular class
    * nxdir()    print the list of entries in the group
    * nxtree()   print the list of entries and subentries in the group
    * nxplot()   plot signal and axes for the group, if available

Fields (class SDS) have attributes for accessing data:
    * nxdims     dimensions of data in the field
    * nxtype     data type
    * nxdata     data in the field
    * nxdata_as('units')      data returned in particular units
    * nxslab     slab context for the field

Linked fields (class NXlink) have attributes for accessing the link::
    * nxlink     reference to the linked field

Unknown fields (class Unknown) are groups with a name that doesn't
start with 'NX'.  These groups are not read or written.

NeXus attributes (class NXattr) have a type and a value only::
    * nxtype     attribute type
    * nxdata     attribute data

Data can be stored in the NeXus file in a variety of units, depending
on which facility is storing the file.  This makes life difficult
for reduction and analysis programs which must know the units they
are working with.  Our solution to this problem is to allow you to
retrieve data from the file in particular units.  For example, if
detector distance is stored in the file using millimeters you can 
retrieve them in meters using::
    entry.instrument.detector.distance.nxdata_as('m')
See `nxs.unit` for more details on the unit formats supported.

The slab interface to field data works by opening the file handle
and keeping it open as long as the slab interface is needed.  This
is done in python 2.5 using the with statement.  Once the context
is entered, get() and put() methods on the node allow you to read
and write data a slab at a time.  For example::

    # Read a Ni x Nj x Nk array one vector at a time
    with node.nxslab:
        size = [1,1,node.nxdims[2]]
        for i in range(node.nxdims[0]):
            for j in range(node.nxdims[1]):
                value = node.nxslab.get([i,j,0],size) # Read counts

The equivalent can be done in Python 2.4 and lower using the context
functions __enter__ and __exit__::

    node.nxslab.__enter__()
    ... do the slab functions ...
    node.nxslab.__exit__()

You can traverse the tree by component class instead of component name.  
Since there may be multiple components of the same class in one group
you will need to specify which one to use.  For example, 
tree.NXentry[0].NXinstrument[0].NXdetector[0].distance references the 
first detector of the first instrument of the first entry.  Unfortunately, 
there is no guarantee regarding the order of the entries, and it may vary 
from call to call, so the above is of limited utility.

The nxplot() method for groups uses matplotlib to plot the data.  You
can replace this with your own plotter by setting nexus.NXgroup._plotter
to your own plotter class.  The plotter class has one method::

    plot(signal, axes, entry, title)

where signal is the field containing the data, axes are the fields
listing the signal sample points, entry is file/path within the file
to the data group and title is the title of the NXentry, if available.

The load() and save() functions are implemented within 
`nxs.tree.NeXusTree`, a subclass of `nxs.napi.NeXus` which allows 
all the usual API functions.  You can subclass NeXusTree with your
own version that defines, e.g., a NXmonitor() method to return an
NXmonitor object when an NXmonitor class is read.  Your NXmonitor
class should probably be a subclass of NXgroup.

You can also specialize the definitions for the basic types 
NXgroup(), NXattr(), SDS() and NXlink() if you want to make
radical changes to the returned structure.  The properties of 
these classes are closely coupled to the behaviour of the readfile
and writefile methods so refer to the source if you need to do this.
"""
__all__ = ['read', 'write', 'dir', 'NeXusTree']

from copy import copy, deepcopy
import numpy
import nxs.napi
import nxs.unit


class NeXusTree(nxs.napi.NeXus):
    """
    Structure-based interface to the NeXus file API.

    Usage::

      file = NeXusTree(filename, ['r','rw','w'])
        - open the NeXus file
      root = file.readfile()
        - read the structure of the NeXus file.  This returns a NeXus tree.
      file.writefile(root)
        - write a NeXus tree to the file.
      data = file.readpath(path)
        - read data from a particular path


    Example::

      nx = NeXusTree('REF_L_1346.nxs','r')
      tree = nx.readfile()
      for entry in tree.NXentry:
          process(entry)
      copy = NeXusTree('modified.nxs','w')
      copy.writefile(tree)

    Note that the large datasets are not loaded immediately.  Instead, the
    when the data set is requested, the file is reopened, the data read, and
    the file closed again.  open/close are available for when we want to
    read/write slabs without the overhead of moving the file cursor each time.
    The NXdata nodes in the returned tree hold the node values.

    """
    def readfile(self):
        """
        Read the nexus file structure from the file.  Reading of large datasets
        will be postponed.  Returns a tree of NXgroup, NXattr, SDS and
        NXlink nodes.
        
        Subclasses can provide methods for individual NeXus classes such
        as NXbeam or NXdata.  Brave users can also specialize NXgroup, 
        NXattr, SDS and NXlink methods.  
        """
        self.open()
        self.openpath("/")
        root = self._readgroup()
        self.close()
        root.nxgroup = None
        
        # Resolve links
        self._readlinks(root)
        return root

    def writefile(self, tree):
        """
        Write the nexus file structure to the file.  The file is assumed to
        start empty.

        Updating individual nodes can be done using the napi interface, with
        nx.handle as the nexus file handle.
        """
        self.open()
        links = []
        # Root node is special --- only write its children.
        # TODO: maybe want to write root node attributes?
        for entry in tree.nxentries.values():
            links += self._writegroup(entry, path="")
        self._writelinks(links)
        self.close()

    def readpath(self, path):
        """
        Read the data on a particular file path.

        Returns a numpy array containing the data, a python scalar, or a
        string depending on the shape and storage class.
        """
        self.open()
        self.openpath(path)
        return self.getdata()

    def _readattrs(self):
        """
        Return the attributes for the currently open group/data or for
        the file if no group or data object is open.
        """
        attrs = dict()
        for i in range(self.getattrinfo()):
            name,length,nxtype = self.getnextattr()
            value = self.getattr(name, length, nxtype)
            pair = self.NXattr(value,nxtype)
            attrs[name] = pair
            #print "read attr",name,pair.nxdata, pair.nxtype
        return attrs

    def _readdata(self,name,path):
        """
        Read a data node, returning SDS or NXlink depending on the
        nature of the node.
        """
        # Finally some data, but don't read it if it is big
        # Instead record the location, type and size
        path = path+"/"+name
        self.opendata(name)
        attrs = self._readattrs()
        if 'target' in attrs and attrs['target'].nxdata != path:
            # This is a linked dataset; don't try to load it.
            #print "read link %s->%s"%(path,attrs['target'].nxdata)
            data = NXlink(name,nxclass='SDS',attrs=attrs)
        else:
            dims,type = self.getinfo()
            if numpy.prod(dims) < 1000:
                value = self.getdata()
            else:
                value = None
            data = self.SDS(name,type,dims,attrs=attrs,value=value,
                               file=self,path=path)
        self.closedata()
        return data

    def _readchildren(self,n,path):
        children = {}
        for i in range(n):
            name,nxclass = self.getnextentry()
            #print "name,class,path",name,nxclass,path
            if nxclass == 'SDS':
                children[name] = self._readdata(name,path)
            elif nxclass.startswith('NX'):
                self.opengroup(name,nxclass)
                children[name] = self._readgroup()
                self.closegroup()
            else: # Bad entry; flag it but don't do anything
                children[name] = self.Unknown(name,nxclass=nxclass)
        return children

    def _readgroup(self):
        """
        Read the currently open group and all subgroups.
        """
        # TODO: does it make sense to read without recursing?
        # TODO: can we specify which NXclasses we are interested
        # in and skip those of different classes?
        n,_,nxclass = self.getgroupinfo()
        # getgroupinfo doesn't seem to return the correct path, so reconstruct
        path = '/' + '/'.join(self.path)
        name = path.split('/')[-1]
        attrs = self._readattrs()
        if 'target' in attrs and attrs['target'].nxdata != path:
            # This is a linked group; don't try to load it.
            #print "read group link %s->%s"%(attrs['target'].nxdata,path)
            group = self.NXlink(name,nxclass=nxclass,attrs=attrs)
        else:
            #print "read group",nxclass,"from",path
            children = self._readchildren(n,path)
            # If we are subclassed with a handler for the particular
            # NXentry class name use that constructor for the group
            # rather than the generic NXgroup class.
            if hasattr(self,nxclass):
                factory = getattr(self,nxclass)
            else:
                factory = self.NXgroup
            group = factory(name,nxclass,attrs=attrs,entries=children)
            # Build chain back structure
            for node in children.values():
                node.nxgroup = group
        return group
    
    def _readlinks(self, root):
        """
        Convert linked nodes into direct references.
        """
        for entry in root.nxentries.values():
            if isinstance(entry, NXlink):
                link = root
                for level in self._link_target.split('.'):
                    link = getattr(link,level)
                entry.nxlink = link

    # Allow subclasses to override 
    def NXattr(self, *args, **kw): return NXattr(*args, **kw)
    def SDS(self, *args, **kw): return SDS(*args, **kw)
    def NXgroup(self,*args,**kw): return NXgroup(*args, **kw)
    def NXlink(self, *args, **kw): return NXlink(*args, **kw)
    def Unknown(self, *args, **kw): return Unknown(*args, **kw)

    def _writeattrs(self,attrs):
        """
        Return the attributes for the currently open group/data or for
        the file if no group or data object is open.
        """
        for name,pair in attrs.iteritems():
            #print "write attrs",name,pair.nxtype,pair.nxdata
            self.putattr(name,pair.nxdata,pair.nxtype)

    def _writedata(self, data, path):
        """
        Write the given data node.

        NXlinks cannot be written until the linked group is created, so
        this routine returns the set of links that need to be written.
        Call writelinks on the list.
        """

        path = path + "/" + data.nxname
        #print 'write data',path

        # If the data is linked then
        if hasattr(data,'_link_target'):
            return [(path, data._link_target)]

        # Finally some data, but don't read it if it is big
        # Instead record the location, type and size
        #print "creating data",child.nxname,child.nxdims,child.nxtype
        if numpy.prod(data.nxdims) > 10000:
            # Compress the fastest moving dimension of large datasets
            slab_dims = numpy.ones(len(data.nxdims),'i')
            slab_dims[-1] = data.nxdims[-1]
            self.compmakedata(data.nxname, data.nxtype, data.nxdims,
                              'lzw', slab_dims)
        else:
            # Don't use compression for small datasets
            self.makedata(data.nxname, data.nxtype, data.nxdims)
        self.opendata(data.nxname)
        self._writeattrs(data.nxattrs)
        value = data.nxdata
        if value is not None: self.putdata(value)
        self.closedata()
        return []

    def _writegroup(self, group, path):
        """
        Write the given group structure, including the data.

        NXlinks cannot be written until the linked group is created, so
        this routine returns the set of links that need to be written.
        Call writelinks on the list.
        """
        path = path + "/" + group.nxname
        #print 'write group',path

        links = []
        self.makegroup(group.nxname, group.nxclass)
        self.opengroup(group.nxname, group.nxclass)
        self._writeattrs(group.nxattrs)
        if hasattr(group, '_link_target'):
            links += [(path, group._link_target)]
        for child in group.nxentries.values():
            if child.nxclass == 'SDS':
                links += self._writedata(child,path)
            elif hasattr(child,'_link_target'):
                links += [(path+"/"+child.nxname,child._link_target)]
            else:
                links += self._writegroup(child,path)
        self.closegroup()
        return links

    def _writelinks(self, links):
        """
        Create links within the NeXus file as indicated by the set of pairs
        returned by writegroup.
        """
        gid = {}

        # identify targets
        for path,target in links:
            gid[target] = None

        # find gids for targets
        for target in gid.iterkeys():
            #sprint "target",target
            self.openpath(target)
            # Can't tell from the name if we are linking to a group or
            # to a dataset, so cheat and rely on getdataID to signal
            # an error if we are not within a group.
            try:
                gid[target] = self.getdataID()
            except RuntimeError:
                gid[target] = self.getgroupID()

        # link sources to targets
        for path,target in links:
            if path != target:
                # ignore self-links
                parent = "/".join(path.split("/")[:-1])
                #print "link %s -> %s"%(parent,target)
                self.openpath(parent)
                self.makelink(gid[target])

class NXattr(object):
    """
    Attributes need to keep track of nxtype as well as attribute value.
    """
    def __init__(self,value=None,nxtype='char'):
        self.nxdata,self.nxtype = value,nxtype

    def __str__(self):
        return str(self.nxdata)

    def __repr__(self):
        if self.nxtype == 'char':
            return "NXattr('%s','%s')"%(self.nxdata,self.nxtype)
        else:
            return "NXattr(%s,'%s')"%(self.nxdata,self.nxtype)

class NXnode(object):
    """
    Abstract base class for elements in NeXus files.
    The individual objects may be either SDS data objects or groups.
    
    Children should be accessible directly as object attributes.
    NeXus attributes accessible using .Aname rather than .name.
    """
    nxclass = "unknown"
    nxname = "unknown"
    nxgroup = None

    def __str__(self):
        return "%s:%s"%(self.nxclass,self.nxname)

    def __repr__(self):
        return "NXnode('%s','%s')"%(self.nxclass,self.nxname)

    def _setattrs(self, attrs):
        for k,v in attrs.items():
            setattr(self, 'A'+k, v)
    def _attrs(self):
        return dict([(k[1:],v) 
                     for k,v in self.__dict__.items()
                     if isinstance(v,NXattr)])
    def _entries(self):
        return dict([(k,v) 
                     for k,v in self.__dict__.items()
                     if isinstance(v,NXnode) and not k.startswith('nx')])
    nxattrs = property(_attrs,doc="NeXus attributes for node")
    nxentries = property(_entries,doc="NeXus nodes within group")

    def _str_name(self,indent=0):
        if self.nxclass == 'SDS':
            return " "*indent+self.nxname+'\n'
        else:
            return " "*indent+self.nxname+':'+self.nxclass+'\n'

    def _str_value(self,indent=0):
        return ""

    def _str_attrs(self,indent=0):
        result = ""
        attrs = self.nxattrs
        names = attrs.keys()
        names.sort()
        for k in names:
            result += " "*indent+"@%s = %s\n"%(k,attrs[k].nxdata)
        return result

    def _str_tree(self,indent=0,attrs=False,recursive=False):
        """
        Print current node and possibly subnodes.
        """
        # Print node
        result = self._str_name(indent=indent)
        if attrs: result += self._str_attrs(indent=indent+2)
        result += self._str_value(indent=indent+2)
        # Print children
        entries = self.nxentries
        names = entries.keys()
        names.sort()
        if recursive:
            for k in names:
                result += entries[k]._str_tree(indent=indent+2,
                                               attrs=attrs, recursive=True)
        else:
            for k in names:
                result += entries[k]._str_name(indent=indent+2)
        return result
    
    def nxdir(self,attrs=False,recursive=False):
        """Print directory"""
        print self._str_tree(attrs=attrs,recursive=recursive)
    def nxtree(self,attrs=True):
        """Print directory tree"""
        print self._str_tree(attrs=attrs,recursive=True)

class NXslab_context(object):
    """
    Context manager for NeXus fields.
    
    The context manager opens the file and positions the cursor to the
    correct field.  Data can then be read or written using the get() and
    put() methods.  See SDS for details on how to use this class.
    """
    
    def __init__(self, file, path, unit_converter):
        self._file = file
        self._path = path
        self._converter = unit_converter

    def __enter__(self):
        """
        Open the datapath for reading slab by slab.
        """
        self._close_on_exit = not self._file.isopen
        self._file.open() # Force file open even if closed
        self._file.openpath(self._path)
        return self

    def __exit__(self, *args):
        """
        Close the file associated the data after reading.
        """
        if self._close_on_exit:
            self._file.close()

    def get(self, offset, size, units=""):
        """
        Get a slab from the data array.

        Offsets are 0-origin.  Shape can be inferred from the data.
        Offset and shape must each have one entry per dimension.
        
        If units are specified, convert the values to the given units
        before returning them.

        Raises ValueError if this fails.

        Corresponds to NXgetslab(handle,data,offset,shape)
        """
        value = self._file.getslab(offset,size)
        return self._converter(value,units)

    def put(self, data, offset):
        """
        Put a slab into the data array.

        Offsets are 0-origin.  Shape can be inferred from the data.
        Offset and shape must each have one entry per dimension.

        Raises ValueError if this fails.

        Corresponds to NXputslab(handle,data,offset,shape)
        """
        self._file.putslab(data, offset, data.shape)
        
class SDS(NXnode):
    """
    NeXus data node SDS.

    Operations are querying type and dimensions, reading the entire
    data block or reading a single slab.

    Attributes:
    
    nxname - dataset name
    nxclass - SDS (Scientific Data Set)
    nxattrs - attributes for the data
    nxgroup - group containing the data
    nxdims - array dimensions (numpy shape)
    nxtype - data type (numpy dtype)
    nxdata - data values
    nxdata_as('units')  - data values in particular units
    nxslab - context manager for reading/writing slabs

    Example:
    
    # Read the entire array
    node.nxdata      # The dataset in stored units
    node.nxdata_as(units="mm")    # read the entire dataset as millimeters

    # Read a Ni x Nj x Nk array one vector at a time
    with node.nxslab:
        size = [1,1,node.nxdims[2]]
        for i in range(node.nxdims[0]):
            for j in range(node.nxdims[1]):
                value = node.nxslab.get([i,j,0],size) # Read counts


    """
    def __init__(self,name,dtype='',shape=[],file=None,path=None,
                 attrs=None,value=None,group=None):
        self._file = file
        self._path = path
        self._value = value
        self.nxclass = "SDS" # Scientific Data Set
        self.nxname = name
        self.nxtype = dtype
        self.nxdims = shape
        self.nxgroup = group
        # Convert NeXus attributes to python attributes
        self._setattrs(attrs)
        if 'units' in attrs:
            units = attrs['units'].nxdata
        else:
            units = None
        self._converter = nxs.unit.Converter(units)
        self.nxslab = NXslab_context(file, path, self._converter)

    def __str__(self):
        """
        If value is loaded, return the value as a string.  If value is
        not loaded, return the empty string.  Only the first view values
        for large arrays will be printed.
        """
        if self._value is not None:
            return str(self._value)
        return ""

    def _str_value(self,indent=0):
        v = str(self)
        if '\n' in v:
            v = '\n'.join([(" "*indent)+s for s in v.split('\n')])
        return v+'\n'

    def _str_tree(self,indent=0,attrs=False,recursive=False):
        dims = 'x'.join([str(n) for n in self.nxdims])
        #return "%s(%s)"%(self.nxtype, dims)
        v = str(self)
        if '\n' in v or v == "":
            v = "%s(%s)"%(self.nxtype, dims)
        v = " "*indent + "%s = %s"%(self.nxname, v)+'\n'
        if attrs: v += self._str_attrs(indent=indent+2)
        return v

    def nxdata_as(self, units=""):
        """
        Read the data in particular units.
        """
        if self._value is None:
            self._value = self._file.readpath(self._path)

        return self._converter(self._value,units)

    nxdata = property(nxdata_as,doc="The data in default units; see also read() and slab()")

class NXlink(NXnode):
    """
    NeXus linked node.
    
    The real node will be accessible by following the nxlink attribute.
    """
    def __init__(self,name,nxclass="",attrs={},group=None):
        self.nxclass = nxclass
        self.nxname = name
        self.nxlink = None  # This will be filled at the end of read
        self.nxgroup = None 
        # Convert NeXus attributes to python attributes
        self._setattrs(attrs)
        self._link_target = attrs['target'].nxdata
    def __str__(self):
        return "NXlink(%s)"%(self._link_target)
    def _str_tree(self,indent=0,attrs=False,recursive=False):
        return " "*indent+self.nxname+' -> '+self._link_target+'\n'

def centers(signal, axes):
    """
    Return the centers of the axes regardless if the axes contain
    bin boundaries or centers.
    """
    def findc(axis, dimlen):
        if len(axis.nxdata) == dimlen+1:
            return (axis.nxdata[:-1] + axis.nxdata[-1:])/2
        else:
            assert len(axis.nxdata) == dimlen
            return axis.nxdata
    return [findc(a,signal.nxdims[i]) for i,a in enumerate(axes)]

def label(field):
    """
    Construct a label for a data field suitable for use on a graph axis.
    """
    if hasattr(field,'Along_name'):
        return field.Along_name.nxdata
    else:
        return "%s (%s)"%(field.nxname,field.Aunits.nxdata)

def imshow_irregular(x,y,z):
    import pylab
    ax = pylab.gca()
    im = pylab.mpl.image.NonUniformImage(ax, extent=(x[0],x[-1],y[0],y[-1]))
    im.set_data(x,y,z)
    ax.images.append(im)
    ax.set_xlim(x[0],x[-1])
    ax.set_ylim(y[0],y[-1])
    pylab.gcf().canvas.draw_idle()

class PylabPlotter(object):
    """
    Matplotlib plotter object for NeXus data nodes.
    """
    def plot(self, signal, axes, entry, title, **opts):
        import pylab
        pylab.cla()

        # Find the centers of the bins for histogrammed data
        axis_data = centers(signal, axes)
        title = entry + '\n' + title

        #One-dimensional Plot
        if len(signal.nxdims) == 1:
            if signal.Aunits.nxdata == 'counts':
                myopts=copy(opts)
                myopts.setdefault('fmt','o')
                myopts.setdefault('linestyle','None')
                pylab.errorbar(axis_data[0], signal.nxdata, 
                               numpy.sqrt(signal.nxdata), **myopts)
            else:
                pylab.scatter(axis_data[0], signal.nxdata, **opts)
            pylab.xlabel(label(axes[0]))
            pylab.ylabel(label(signal))
            pylab.title(title)
 
        #Two dimensional plot
        elif len(signal.nxdims) == 2:
            #gridplot = pylab.pcolormesh
            gridplot = imshow_irregular
            if signal.Aunits.nxdata == 'counts':
                gridplot(axis_data[1], axis_data[0], 
                         numpy.log10(signal.nxdata+1), **opts)
            else:
                gridplot(axis_data[1], axis_data[0], 
                         signal.nxdata, **opts)
            pylab.xlabel(label(axes[1]))
            pylab.ylabel(label(axes[0]))
            pylab.title(title)

        # No support for higher dimensions yet
        else:
            raise RuntimeError, "Cannot plot a dataset of rank 3 or greater."
    @staticmethod
    def show():
        import pylab
        pylab.show()    

class NXgroup(NXnode):
    """
    NeXus group node.  NXgroup nodes have no data associated with them,
    but they do have attributes and children.
    """
    # Plotter to use for nxplot calls
    _plotter = PylabPlotter()

    def __init__(self,name, nxclass=NXnode.nxclass,
                 attrs={},entries={}, group=None):
        self.nxclass = nxclass
        self.nxname = name
        self.nxgroup = group
        # Convert children and NeXus attributes to python attributes
        self._setattrs(attrs)
        for k,v in entries.items():
            setattr(self, k, v)
    def _str_value(self,indent=0):
        return ""

    def __getattr__(self, key):
        """
        Provide direct access to nodes via nxclass name.
        """
        if key.startswith('NX'):
            return self.nxcomponent(key)
        raise KeyError(key+" not in "+self.nxclass+":"+self.nxname)

    def nxcomponent(self, nxclass):
        """
        Find all child nodes that have a particular class.
        """
        return [E for name,E in self.nxentries.items() if E.nxclass==nxclass]

    def nxplot(self, **opts):
        """
        Plot data contained within the group.
        """
        
        # Find a plottable signal
        for node in self.nxentries.values():
            if 'signal' in node.nxattrs:
                signal = node
                break
        else:
            raise RuntimeError('No plottable signal')
        
        # Find the associated axes
        if hasattr(signal,'Aaxes'):
            axes = [getattr(self,a) for a in signal.Aaxes.nxdata.split(':')]
        else:
            raise RuntimeError('Axes attribute missing from signal')

        # Construct title
        path = []
        node = self
        subtitle = ''
        while node.nxgroup is not None:
            if node.nxclass == 'NXentry':
                subtitle = node.title.nxdata
            path = [node.nxname] + path
            node = node.nxgroup
        path = [node.Afile_name.nxdata] + path
        entry = "/".join(path)

        # Plot with the available plotter
        self._plotter.plot(signal, axes, entry, subtitle)

class Unknown(NXnode):
    """
    Unknown group type; class does not start with NX or SDS.
    """
    def __init__(self, name, nxclass=NXnode.nxclass):
        self.nxname = name
        self.nxclass = nxclass
    def __repr__(self):
        return "Unknown('%s','%s')"%(self.nxname,self.nxclass)

# File level operations
def read(filename, mode='r'):
    """
    Read a NeXus file, returning a tree of nodes
    """
    file = NeXusTree(filename,mode)
    tree = file.readfile()
    file.close()
    return tree

def write(filename, tree, format='w5'):
    """
    Write a NeXus file from a tree of nodes
    """
    file = NeXusTree(filename, format)
    file.writefile(tree)

def dir(file):
    """
    Read and summarize the named nexus file.
    """
    tree = readfile(file)
    tree.nxtree()

def demo(argv):
    """
    Process command line commands in argv.  argv should contain
    program name, command, arguments, where command is one of
    the following:
        copy fromfile.nxs tofile.nxs
        ls f1.nxs f2.nxs ...
    """
    if len(argv) > 1:
        op = argv[1]
    else:
        op = 'help'
    if op == 'ls':
        for f in argv[2:]: dir(f)
    elif op == 'copy' and len(argv)==4:
        tree = read(argv[2])
        write(argv[3], tree)
    elif op == 'plot' and len(argv)==4:
        tree = read(argv[2])
        for entry in argv[3].split('.'):
            tree = getattr(tree,entry)
        tree.nxplot()
        tree._plotter.show()
            
    else:
        usage = """
usage: %s cmd [args]
    copy fromfile.nxs tofile.nxs
    ls *.nxs
    plot file.nxs entry.data
        """%(argv[0],)
        print usage

if __name__ == "__main__":
    import sys
    demo(sys.argv)

