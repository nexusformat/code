import types
import cPickle
import time
import string

from nexus import *
#from nxplot import *
import nxtemplates


def checkTypes(pytype, nxtype):
	""" test if nexus type and python type match together
		- args : ...
	"""
	int_types 	= [NX_INT8, NX_UINT8, NX_INT16, NX_UINT16, NX_INT32, NX_UINT32]	
	float_types = [NX_FLOAT32, NX_FLOAT64]
	str_types 	= [NX_CHAR]
	
	if pytype == types.IntType:
		if not nxtype in int_types:
			print "types don't match"
			return 0
	elif pytype == types.LongType:
		if not nxtype in int_types:
			print "types don't match"
			return 0
	elif pytype == types.FloatType:
		if not nxtype in float_types:
			print "types don't match"
			return 0
	elif pytype == types.StringType:
		if not nxtype in str_types:
			print "types don't match"
			return 0
	else:	
		print "unknown type:", 
		return 0

	return 1	


# classes for the 3 different objects 

class NXattr:
	def __init__(self, parent=None, path=None, name="", nxtype=NX_CHAR, value=None):
		self.parent = parent
		
		if path != None:
			if type(path) == types.StringType:
				self.path = pathStringToList(path)	
			elif type(path)==types.ListType:
				self.path = path
			else:
				self.path = None
				
		if path == None:
			if parent != None:
				if type(parent.path)==types.ListType:
					self.path = parent.path.append((name,"NXattr"))
			else:
				self.path = [(name,"NXattr")]
		
		self.name = name
		self.type = nxtype

		if value != None:
			if type(value)==types.ListType or type(value)==types.TupleType:
				if len(value) > 0:
					if not checkTypes(type(value[0]), nxtype):
						print "error. types don't match"
						return None
			else:
				if not checkTypes(type(value), nxtype): 		
					print "error. simple types don't match"
					return None

		self.value = value		
		
		if type(value) == types.ListType or type(value) == types.TupleType or type(value) == types.StringType:
			self.length = len(value)
		else:	
			self.length = 1


	def getName(self):
		return self.name
	
	def getValue(self):
		return self.value
		
	def getType(self):
		return self.type
	
	def getLength(self):
		return self.length
		

	def setValue(self, value):	
		if type(value) == types.ListType  or type(value) == types.TupleType:
			if len(value) > 0:
				if not checkTypes(type(value[0]), type):
					print "error. types don't match"
					return 0
		else:
			if not checkTypes(type(value), type): 		
				print "error. types don't match"
				return 0
		
		self.value = value

		if type(value) == types.ListType or type(value) == types.TupleType or type(value) == types.StringType:
			self.length = len(value)
		else:	
			self.length = 1
		
		return 1



class NXelem:
	def __init__(self, parent=None, path=None, name="", nxtype=0, dims=None, attrs=None, data=None):
		if dims == None:
			dims = [-1]
		if attrs == None:
			attrs = {}
			
		self.parent = parent
		if path != None:
			if type(path) == types.StringType:
				self.path = pathStringToList(path)	
			elif type(path)==types.ListType:
				self.path = path
			else:
				self.path = None

		if path == None:
			if parent != None:
				if type(parent.path)==types.ListType:
					self.path = parent.path.append((name,"SDS"))
			else:
				self.path = [(name,"SDS")]
			
		self.nxtype = nxtype
		self.dims = dims
		
		self.name = name
		self.data = data
		
		self.attrs = attrs
		
	def getName(self):
		return self.name
	
	def getType(self):
		return self.nxtype
	
	def getDims(self):
		return self.dims

	def getRank(self):
		return len(self.dims)
		
	def getData(self):
		return self.data
		
	def getAttrs(self):
		return self.attrs

	def getAttr(self, attname):
		if attname in self.attrs:
			return self.attrs[attname]
		else:
			return None
	
	def getAttrValue(self, attname):
		if attname in self.attrs:
			return self.attrs[attname].getValue()
		return None
		
	
	
	def setData(self, data):
		#checkTypeAndDim(data)
		#set type and dim 
		#print "setting data for ", self.name, " ; values: " , data
		self.data = data

	def setAttr(self, attr):
		if self.path != None:
			groupclass = self.path[len(self.path)-1][1]
			if groupclass[0:2] != "NX":
				groupclass = self.path[len(self.path)-2][1]
				if groupclass[0:2] != "NX":
					pass
					#print "Error in elem path"
					#print "elem %s doesn't belong to a group. no validation possible \n-> validate when appending to group!!!!"%(self.name)
					#return 0
			if not attname in nxtemplates.group_types[groupclass].elems[self.name].attrs.keys():
				print "attr not valid !!" 
				return 0
				
		self.attrs[attname] = attr 
		return 1
	

	def setAttrValue(self, attname, attvalue, nxtype):
		attpath = None
		if self.path != None:
			groupclass = self.path[len(self.path)-1][1]
			if groupclass[0:2] != "NX":
				groupclass = self.path[len(self.path)-2][1]
				if groupclass[0:2] != "NX":
					pass
					#print "elem %s doesn't belong to a group. no validation possible \n-> validate when appending to group!!!!"%(self.name)
					#return 0
		
			
			if groupclass[0:2] == "NX":
				if not attname in nxtemplates.group_types[groupclass].elems[self.name].attrs.keys():
					print "attr not valid !!" 
					return 0
			attpath = self.path
			
		#atttype = guessNXType(attvalue)	
		self.attrs[attname] = NXattr(path= attpath, name=attname, value=attvalue, nxtype=nxtype)
		return 1
			
			



class NXgroup:
	def __init__(self, parent=None, path=None, name="", nxclass="", attrs=None, elems=None, groups=None):
		self.parent = parent
		
		if path != None:
			if type(path) == types.StringType:
				self.path = pathStringToList(path)	
			elif type(path)==types.ListType:
				self.path = path
			else:
				self.path = None
				
		if path == None:
			if parent != None:
				if type(parent.path)==types.ListType:
					self.path = parent.path.append((name,nxclass))
			else:
				self.path = [(name,nxclass)]
				
		self.name = name
		self.nxclass = nxclass
		
		self.attrs  = attrs
		self.elems  = elems
		self.groups = groups
		
		
	def setAttrs(self, attrs):
		self.attrs = attrs
		
	def setElems(self, elems):
		self.elems = elems
	
	def setGroups(self, groups):
		self.groups = groups
		
	def getAttrs(self):
		return self.attrs

	def getElems(self):
		return self.elems
	
	def getGroups(self):
		return self.groups
		
	def getAttr(self, attrname):
		if attrname in self.attrs:
			if self.attrs[attrname] != None:
				if type(self.attrs[attrname]) == types.InstanceType:
					return self.attrs[attrname]
		return None				


	def getAttrValue(self, attname):
		pass

	def setAttrValue(self, attname, attvalue):
		pass


	def getElem(self, elemname):
		if elemname in elems:
			if elems[elemname] != None:
				if type(elems[elemname]) == types.InstanceType:
					return elems[elenname]
		return None			

	def getElemData(self, elemname):
		if elemname in elems:
			if elems[elemname] != None:
				if type(elems[elemname]) == types.InstanceType:
					return elems[elemname].getData()
		
	def setElemData(self, elemname, data):
		if elemname in elems:
			if elems[elemname] != None:
				if type(elems[elemname]) == types.InstanceType:
					elems[elemname].setData(data)
					return 1
		return 0			

	def getGroup(self, groupname):
		if groupname in groups:
			if groups[groupname] != None:
				if type(groups[groupname]) == types.InstanceType:
					return groups[groupname]
		return None			

		

class NXaxis(NXelem):
	def __init__(self, name="", nxdata=None, nxtype=NX_FLOAT32, dims=None, attrs=None, data=None):
		if dims == None: 	dims = []
		if attrs == None:	attrs = {}	
		if data == None: 	data = []
		NXelem.__init__(self, parent=None, path=None, name=name, nxtype=nxtype, dims=dims, attrs=attrs, data=data)
		self.nxdata = nxdata
		self.gridinfo = 0
		
	def setUnit(self, unit):
		#TODO: check if types match (maybe check in NXelem)
		self.setAttrValue("unit", unit)
		
	def setOffset(self, offset):
		self.setAttrValue("offset", offset)
	
	def setLongName(self, longname):
		self.setAttrValue("long_name", longname)
		
	def setPrimary(self, primary):	
		self.setAttrValue("primary", primary)
	
	def appendData(self, value, cache="yes"):
		#check if types correspond
		if len(self.data) >= self.dims[0]:
			print "warning: axis content exceeds dimension boundaries", len(self.data), self.dims[0]
			return 0
		self.data.append(value)
		if cache == "yes":
			if self.nxdata.axcachefile != "":
				self.saveAxData(value)
			if self.nxdata.nxcachefile !="":
				self.saveAxDataToNX(value)
		return 1

	def attachToNXData(self, nxdata):
		self.nxdata = nxdata
		nxdata.attachAxis(self)
	
	def getNXData(self):
		return self.nxdata
	

	def saveAxDataToNX(self, value):
		if self.nxdata == None:
			print "axis hasn't been assigned to nxdata"
			return 0
		return self.nxdata.saveAxDataToNX(self, value)	
	
	def saveAxData(self, value):
		if self.nxdata == None:
			print "axis hasn't been assigned to nxdata"
			return 0
		return self.nxdata.saveAxData(self, value)	

	def loadAxData(self):
		if self.nxdata == None:
			print "axis hasn't been assigned to nxdata"
			return 0
		return self.nxdata.loadAxData(self)	
		
	def loadAxDataFromNX(self):	
		if self.nxdata == None:
			print "axis hasn't been assigned to nxdata"
			return 0
		return self.nxdata.loadAxDataFromNX(self)	
		
		
	def restoreData(self, filename=None):
		if filename != None:
			if filename.endswith(".nxs"):  	
				self.nxdata.nxcachefile = filename
			elif filename.endswith(".nxt"):
				self.nxdata.axcachefile = filename
			else:
				print "improper cache file name" 
		if self.nxdata.nxcachefile != None and self.nxdata.nxcachefile != "":		
			self.loadAxDataFromNX()
		elif self.nxdata.axcachefile != None and self.nxdata.axcachefile != "":	
			self.loadAxData()	
	
	def trimAxDataToGridinfo(self):
		self.data = self.data[0:self.gridinfo]
	




class NXdataelem(NXelem):
	def __init__(self, path=None, name="", nxtype=NX_FLOAT32, dims=None, axnames="", attrs=None, data=None, cachefile="", nxcachefile="", axcachefile=""):
		if dims == None: 	dims = []
		if attrs == None:	attrs = {}	
		if data == None: 	data = []
		NXelem.__init__(self, parent=None, path=path, name=name, nxtype=nxtype, dims=dims, attrs=attrs, data=data)
		self.attrs["axes"] = NXattr(name="axes", nxtype=NX_CHAR, value=axnames)
		self.attrs["signal"] = NXattr(name="signal", nxtype=NX_CHAR, value="1")
		self.cachefile = cachefile
		self.axcachefile = axcachefile
		self.nxcachefile = nxcachefile
		self.axes = None
		
		#if nxcachefile!="":
		#	self.nxcachefile = nxcachefile
		#else:
		#	self.nxcachefile = self.newNXCacheFilename()
		self.nxhandle = None
		
		if len(dims) > 0:
			self.gridinfo = self.initGridInfo(0)
			#print "self.gridinfo:", self.gridinfo
			
		
	def initGridInfo(self, level):
		# provide info where to append data when using NeXus cache file
		# needed to determine the insertion position when using NXputslab
		l = []
		if level<len(self.dims)-1:
			for i in range(self.dims[level]):
				l.append(self.initGridInfo(level+1))
		else:		
			return 0
		return l	


	def newNXCacheFilename(self):
		# provide unique file name: just get actual time in seconds
		return self.name+time.ctime()+".nxs"
	
		
	def setCacheFile(self, cachefile):
		if cachefile == None:
			print "please provide a filename"
			return 0
			
		if type(cachefile) != types.StringType:
			print "setCacheFile:please pass a filename as string"
			return 0
		
		self.cachefile = cachefile
	
		
	def getCacheFile(self):
		return self.cachefile


	def cachefileExists(self):
		try:
			f = file(self.cachefile, 'r')
		except: 
			return 0
		return 1


	def setNXCacheFile(self, nxcachefile):
		if nxcachefile == None:
			print "please provide a filename"
			return 0
			
		if type(nxcachefile) != types.StringType:
			print "setCacheFile:please pass a filename as string"
			return 0
		
		self.nxcachefile = nxcachefile
		
		
	def getNXCacheFile(self):
		return self.nxcachefile
	
	
	def nxcachefileExists(self):
		try:
			f = file(self.nxcachefile, 'r')
		except: 
			return 0
		return 1
		
	
	def getAxCacheFile(self):
		return self.axcachefile


	def axcachefileExists(self):
		try:
			f = file(self.axcachefile, 'r')
		except: 
			return 0
		return 1


	def setAxCacheFile(self, axcachefile):
		if axcachefile == None:
			print "please provide a filename"
			return 0
			
		if type(axcachefile) != types.StringType:
			print "setCacheFile:please pass a filename as string"
			return 0
		
		self.axcachefile = axcachefile
	
	
	def setAxes(self, axnames):
		if "axis" in self.attrs.keys():
			self.attrs["axis"].setValue(axnames)
		else:	
			self.attrs["axis"] = NXattr(name="axis", nxtype=NX_CHAR, value=axnames)
	
	def attachAxis(self, axisobj):
		if self.axes == None:
			self.axes={}
		self.axes[axisobj.name] = axisobj
		#print "axis data:", self.axes[axisobj.name].data
		
		
	def setUnit(self, unit):
		#TODO: check if types match (maybe check in NXelem)
		self.setAttrValue("unit", unit)
		
	def setOffset(self, offset):
		self.setAttrValue("offset", offset)
	
	def setLongName(self, longname):
		self.setAttrValue("long_name", longname)
		
	def setPrimary(self, primary):	
		self.setAttrValue("primary", primary)
	
	
	#def plotData(self, axname, coords=None):
	#	p = NXplot()
		# transform data into list
	#	if type(coords) != types.TupleType and type(coords) != types.ListType:
	#		print "coordinates must be passed as tuple or list of integers"
	#		return 0
			
	#	if len(coords) != len(self.dims)-1:
	#		print "unsufficient coordinate information."
	#		return 0
	
	#	for i in range(len(coords)):
	#		if coords[i] >= self.dims[i+1]:
	#			print "coordinates are out of bounds"
	#			return 0
#
#		if type(coords[0]) != types.IntType:
#			print "coords represent indices. please provide them as integers"
#			return 0
#		
#		data = self.data
#		for i in range(len(coords)):
#			while len(data) <= coords[i]:
#				data.append([])
#			data = data[coords[i]]
#	
#		if axname not in self.axes.keys():
#			print "nxdata does not contain axes with name %s"%(axname)
#			return 0
#		
#		p.plotData(data, self.axes[axname].data)
		
		
	def appendData(self, value, coords=None, cache="yes"):
		#check if types correspond
		if coords == None:
			data = self.data
			for i in range(len(self.dims)-1):
				if len(data) <= 0:
					data.append([])
				data = data[0]
			if len(data) >= self.dims[len(self.dims)-1]:	
				#print "oh:", len(data), self.dims[len(self.dims)-1]
				print "warning: data exceeds dimension boundary"
				return 0
			data.append(value)
			if cache=="yes":
				if len(self.dims) <=1:
					coord = None
				else:	
					coord = []
					for i in range(len(self.dims)-1):
						coord.append(0)
				if self.cachefile!="":		
					self.saveData(value, coord)
				if self.nxcachefile!="":	
					self.saveDataToNX(value, coord)

		else:
			if type(coords) != types.TupleType and type(coords) != types.ListType:
				print "coordinates must be passed as tuple or list of integers"
				return 0
				
			if len(coords) != len(self.dims)-1:
				print "unsufficient coordinate information."
				return 0
		
			for i in range(len(coords)):
				if coords[i] >= self.dims[i+1]:
					print "coordinates are out of bounds"
					return 0

			if type(coords[0]) != types.IntType:
				print "coords represent indices. please provide them as integers"
				return 0
			
			data = self.data
			for i in range(len(coords)):
				while len(data) <= coords[i]:
					data.append([])
				data = data[coords[i]]
			
			if len(data) >= self.dims[len(self.dims)-1]:	
				print "warning: data exceeds dimension boundary"
				return 0
			data.append(value)
			if cache=="yes":
				if self.cachefile != "":
					self.saveData(value, coords)
				if self.nxcachefile != "":	
					self.saveDataToNX(value, coords)
		return 1


		
	# find a mechanism to save only the newly appended data and not the complete set.
	# disk cache problem: where to append data, when only one datum is set to a single entry
	# solution -> save data with coordinate information !!
	# always append to end of file.-> when reloading -> read coordinates -> reconstruct array.
	# define conventions:
	# when new data is stored -> new file (one file per object)
	# in header store date and user and name of file

	def saveDataToNX(self, value, coords):
		if not self.nxcachefileExists():
			#print "opening file"
			status, self.nxhandle = NXopen(self.nxcachefile, NXACC_CREATE5)
			status = NXmakegroup(self.nxhandle, "entry", "NXentry")
			status = NXopengroup(self.nxhandle, "entry", "NXentry")
			status = NXmakegroup(self.nxhandle, "data", "NXdata")
			status = NXopengroup(self.nxhandle, "data", "NXdata")
			
			if len(self.dims) > 1:
				gridinfo_rank = len(self.dims)-1
				gridinfo_dims = self.dims[0:len(self.dims)-1]
			else:	
				#in case of one-dim data we need only a single field to store gridinfo
				gridinfo_rank=1
				gridinfo_dims=[1]
				
			status = NXmakedata(self.nxhandle, self.name+"_gridinfo", NX_INT32, gridinfo_rank, gridinfo_dims)
			status = NXmakedata(self.nxhandle, self.name, self.nxtype, len(self.dims), self.dims)
			status = NXopendata(self.nxhandle, self.name)
			#print "DEBUG coords:", coords

			#write some header info here
		else:
			if self.nxhandle != None:
				status = NXinitgroupdir(self.nxhandle)
				status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
				found = 0
				for i in range(nitems):
					status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
					if name == self.name and nxclass == "SDS":
						status = NXopendata(self.nxhandle, self.name)
						found = 1
				if found == 0:
					status = NXflush(self.nxhandle)
					status = NXclose(self.nxhandle)
					status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
					status = NXopengroup(self.nxhandle, "entry", "NXentry")
					status = NXopengroup(self.nxhandle, "data", "NXdata")
					status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
					found = 0
					for i in range(nitems):
						status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
						if name == self.name and nxclass == "SDS":
							status = NXopendata(self.nxhandle, self.name)
							found = 1
					if found == 0:
						status = NXmakedata(self.nxhandle, self.name, self.nxtype, len(self.dims), self.dims)
						status = NXopendata(self.nxhandle, self.name)
			else:
				status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
				status = NXopengroup(self.nxhandle, "entry", "NXentry")
				status = NXopengroup(self.nxhandle, "data", "NXdata")
				status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
				found = 0
				for i in range(nitems):
					status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
					if name == self.name and nxclass == "SDS":
						status = NXopendata(self.nxhandle, self.name)
						found = 1
				if found == 0:
					#print "DEBUG creating:", self.name, self.nxtype, len(self.dims), self.dims
					status = NXmakedata(self.nxhandle, self.name, self.nxtype, len(self.dims), self.dims)
					status = NXopendata(self.nxhandle, self.name)


		if coords == None:
			if self.gridinfo >= self.dims[len(self.dims)-1]:	
				print "warning: data exceeds dimension boundary", self.gridinfo, self.dims[len(self.dims)-1]
				return 0
			startslab = []
			sizeslab=[]
			for i in range(len(self.dims)):
				startslab.append(0)
				sizeslab.append(1)
			startslab[len(startslab)-1]= self.gridinfo	
			status = NXputslab(self.nxhandle, [value], startslab, sizeslab)	
			self.gridinfo = self.gridinfo+1
		else:
			if type(coords) != types.TupleType and type(coords) != types.ListType:
				print "coordinates must be passed as tuple or list of integers"
				return 0
				
			if len(coords) != len(self.dims)-1:
				print "unsufficient coordinate information."
				return 0
			
			for i in range(len(coords)):
				if coords[i] >= self.dims[i+1]:
					print "coordinates are out of bounds"
					return 0

			if type(coords[0]) != types.IntType:
				print "coords represent indices. please provide them as integers"
				return 0
			
			#go through array, but only to the forelast subarray 
			#(this ist because we need the handle, in order to change the value later)
			
			info = self.gridinfo
			for i in range(len(coords)-1):
				info = info[coords[i]]
			
			# now get last subarrray value
			infoval = info[coords[len(coords)-1]]
			if infoval >= self.dims[len(self.dims)-1]:	
				print "warning: data exceeds dimension boundary", infoval, self.dims[len(self.dims)-1]
				return 0
			
			#construct slab array 
			#append all coords, then append value of the gridinfo in the corresponding coords position
			startslab = []
			sizeslab=[]
			for i in range(len(coords)):
				startslab.append(coords[i])
				sizeslab.append(1)
			startslab.append(infoval)
			sizeslab.append(1)
			status = NXputslab(self.nxhandle, [value], startslab, sizeslab)	
			#now we need the previously mentioned handle in order to modify the subarray object
			#just increment value by one	
			info[coords[len(coords)-1]] = infoval + 1
			
		status = NXclosedata(self.nxhandle)
		status = NXflush(self.nxhandle)
	
		# now write gridinfo
		status = NXinitgroupdir(self.nxhandle)
		status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
		gridinfoname = self.name+"_gridinfo"
		found = 0
		for i in range(nitems):
			status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
			if name == gridinfoname and nxclass == "SDS":
				status = NXopendata(self.nxhandle, gridinfoname)
				found = 1
		if found == 0:
			if len(self.dims) > 1:
				gridinfo_rank = len(self.dims)-1
				gridinfo_dims = self.dims[0:len(self.dims)-1]
			else:	
				#in case of one-dim data we need only a single field to store gridinfo
				gridinfo_rank=1
				gridinfo_dims=[1]
			status = NXmakedata(self.nxhandle, gridinfoname, NX_INT32, gridinfo_rank, gridinfo_dims)
			status = NXopendata(self.nxhandle, gridinfoname)
		startslab = []
		sizeslab=[]
		if coords != None:
			for i in range(len(coords)):
				startslab.append(coords[i])
				sizeslab.append(1)
				info_value = info[coords[len(coords)-1]]
		else:
			startslab = [0]
			sizeslab = [1]
			info_value = self.gridinfo
		#print "DEBUG gridinfo:", info_value
		status = NXputslab(self.nxhandle, info_value, startslab, sizeslab)	
		status = NXclosedata(self.nxhandle)
		status = NXflush(self.nxhandle)
		return 1
			
			
	
	def saveData(self, value, coords):
		if not self.cachefileExists():
			f = file(self.cachefile, "w")
			f.write("HEADER:\n")
			f.write("put some information here: date of creation, user name etc.\n")
			f.write("\n")
			f.write("\n")
			f.write("START:\n")
		else:
			f = file(self.cachefile, "a")
			
		f.write(repr(coords))
		f.write(",")
		f.write(repr(value))
		f.write("\n")
		f.write(cPickle.dumps(coords))
		f.write("\n")
		f.write(cPickle.dumps(value))
		f.write("\n")
		f.close()
		
		
	def loadDataFromNX(self):
		if not self.nxcachefileExists():
			print "cachefile does not exist. unable to load the data"
			return 0
		status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
		status = NXopengroup(self.nxhandle, "entry", "NXentry")
		status = NXopengroup(self.nxhandle, "data", "NXdata")
		status = NXopendata(self.nxhandle, self.name)
		if status != 1:
			print "failed to open dataset %s. it does not seem to exist"%(self.name)
			return 0
		status,rank,dims,nxtype = NXgetinfo(self.nxhandle)
		if nxtype != self.nxtype:
			print "types don't match. cannot load data set"
			return 0
		if dims != self.dims:
			print "dimensions don't match. cannot load data set"
			return 0

		# CAUTION: -> get data gets the hole array -> we need only the array of size gridinfo ->
		# FIX IT
		# we must either work with NXgetslab or just cut the array according to gridinfo 
		status, self.data = NXgetdata(self.nxhandle)			
		status = NXclosedata(self.nxhandle)

		status = NXopendata(self.nxhandle, self.name+"_gridinfo")
		if status != 1:
			print "failed to open gridinfo for dataset %s"%(self.name)
			print "WARNING: gridinfo will be reset to zero"
			self.gridinfo = self.initGridInfo(0)
			return 0
		status,rank,dims,nxtype = NXgetinfo(self.nxhandle)
		if dims != self.dims[0:len(self.dims)-1]:
			print "dimensions don't match. cannot load data set"
			return 0
		
		status, self.gridinfo = NXgetdata(self.nxhandle)			
		status = NXflush(self.nxhandle)
		status = NXclose(self.nxhandle)
		self.nxhandle = None
		#now trim data array according to gridinfo
		self.trimDataToGridinfo(self.data, self.gridinfo, 0)
		return 1
	
	
	def trimDataToGridinfo(self, data, info, level):
		if level<len(self.dims)-1:
			for i in range(len(data)):
				if type(info[i]) == types.IntType:
					data[i] = data[i][0:info[i]]
				else:	
					self.trimDataToGridinfo(data[i], info[i], level+1)
	
	
	def loadData(self):
		if not self.cachefileExists():
			print "cachefile does not exist. unable to load the data"
			return 0
		f = file(self.cachefile, "r")	
		s = ""
		while s[0:5] != "START":
			s =f.readline()
		
		s = f.readline()
		while s != "":
			coords_string = ""
			s = "none"
			while s[len(s)-2] != ".":
				s = f.readline()
				coords_string = coords_string+s
			coord = cPickle.loads(coords_string[:len(coords_string)-1]) 
			
			data_string = ""
			s = "none"
			while s[len(s)-2] != ".":
				s = f.readline()
				data_string = data_string+s
			data = cPickle.loads(data_string[:len(data_string)-1]) 
			self.appendData(data, coords=coord, cache="no")
			s = f.readline()
		return 1
	
	
	def restoreData(self, filename=None):
		if filename != None:
			if filename.endswith(".nxt"):
				self.cachefile=filename
			elif filename.endswith(".nxs"):  	
				self.nxcachefile = filename
			else:
				print "improper cache file name" 
		if self.nxcachefile != None and self.nxcachefile != "":		
			self.loadDataFromNX()
		elif self.cachefile != None and self.cachefile != "":	
			self.loadData()	


	def saveAxData(self, axis, value):
		# need a mechanism to store the ax name with the pickled data
		# just to be independed of how the data is written to the file
		if not self.axcachefileExists():
			f = file(self.axcachefile, "w")
			f.write("HEADER:\n")
			f.write("put some information here: date of creation, user name etc.\n")
			f.write("\n")
			f.write("\n")
			f.write("START:\n")
		else:
			f = file(self.axcachefile, "a")
			
		f.write(axis.name)
		f.write("\n")
		f.write(cPickle.dumps(value))
		f.write("\n")
		f.close()
		
	
	def loadAxData(self, axis):
		if not self.axcachefileExists():
			print "axis cachefile does not exist. unable to load the axis data"
			return 0
		f = file(self.axcachefile, "r")	
		s = ""
		while s[0:5] != "START":
			s =f.readline()
		
		s = f.readline()
		while s != "":
			if s[:len(s)-1] == axis.name: 
				data_string = ""
				s = "none"
				while s[len(s)-2] != ".":
					s = f.readline()
					data_string = data_string+s
				data = cPickle.loads(data_string[:len(data_string)-1]) 
				axis.appendData(data, cache="no")
			s = f.readline()
		return 1
	
	
	def saveAxDataToNX(self, axis, value):
		if not self.nxcachefileExists():
			status, self.nxhandle = NXopen(self.nxcachefile, NXACC_CREATE5)
			status = NXmakegroup(self.nxhandle, "entry", "NXentry")
			status = NXopengroup(self.nxhandle, "entry", "NXentry")
			status = NXmakegroup(self.nxhandle, "data", "NXdata")
			status = NXopengroup(self.nxhandle, "data", "NXdata")
			#status = NXmakedata(self.nxhandle, axis.name+"_gridinfo", NX_INT32, 1, [1])
			status = NXmakedata(self.nxhandle, axis.name, axis.nxtype, 1, axis.dims)
			status = NXopendata(self.nxhandle, axis.name)
			
			#write some header info here
		else:
			if self.nxhandle != None:
				status = NXinitgroupdir(self.nxhandle)
				status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)

				found = 0
				for i in range(nitems):
					status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
					if name == axis.name and nxclass == "SDS":
						status = NXopendata(self.nxhandle, axis.name)
						found = 1
				if found == 0:
					status = NXflush(self.nxhandle)
					status = NXclose(self.nxhandle)
					status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
					status = NXopengroup(self.nxhandle, "entry", "NXentry")
					status = NXopengroup(self.nxhandle, "data", "NXdata")
					status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
					found = 0
					for i in range(nitems):
						status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
						if name == axis.name and nxclass == "SDS":
							status = NXopendata(self.nxhandle, axis.name)
							found = 1
					if found == 0:
						status = NXmakedata(self.nxhandle, axis.name, axis.nxtype, 1, axis.dims)
						status = NXopendata(self.nxhandle, axis.name)
			else:
				status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
				status = NXopengroup(self.nxhandle, "entry", "NXentry")
				status = NXopengroup(self.nxhandle, "data", "NXdata")
				status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
				found = 0
				for i in range(nitems):
					status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
					if name == axis.name and nxclass == "SDS":
						status = NXopendata(self.nxhandle, axis.name)
						found = 1
				if found == 0:
					status = NXmakedata(self.nxhandle, axis.name, axis.nxtype, 1, axis.dims)
					status = NXopendata(self.nxhandle, axis.name)

		if axis.gridinfo >= axis.dims[0]:	
			print "warning: ax data exceeds dimension boundary", axis.gridinfo, axis.dims[0]
			return 0
		startslab= [axis.gridinfo]
		sizeslab=[1]
		
		status = NXputslab(self.nxhandle, [value], startslab, sizeslab)
		if status:
			axis.gridinfo = axis.gridinfo+1
		status = NXputattr(self.nxhandle, "cursor", axis.gridinfo, NX_INT32)	
		status = NXclosedata(self.nxhandle)		
		
		#status, nitems, path, nxclass = NXgetgroupinfo(self.nxhandle)
		#status = NXinitgroupdir(self.nxhandle)
		#gridinfoname = axis.name+"_gridinfo"
		#found = 0
		#for i in range(nitems):
		#	status, name, nxclass, nxtype = NXgetnextentry(self.nxhandle)
		#	if name == gridinfoname and nxclass == "SDS":
		#		status = NXopendata(self.nxhandle, gridinfoname)
		#		found = 1
		#if found == 0:
		#	status = NXmakedata(self.nxhandle, gridinfoname, NX_INT32, 1, [1])
		#	status = NXopendata(self.nxhandle, gridinfoname)
		#status = NXputdata(self.nxhandle, axis.gridinfo)	
		#status = NXclosedata(self.nxhandle)
		return 1

	
	def loadAxDataFromNX(self, axis):
		if not self.nxcachefileExists():
			print "cachefile does not exist. unable to load the data"
			return 0
		status, self.nxhandle = NXopen(self.nxcachefile, NXACC_RDWR)
		status = NXopengroup(self.nxhandle, "entry", "NXentry")
		status = NXopengroup(self.nxhandle, "data", "NXdata")
		
		status = NXopendata(self.nxhandle, axis.name)
		if status!=1:
			print "axis data not found in nxfile %s"%(self.nxcachefile)
			return 0
		status,rank,dims,nxtype = NXgetinfo(self.nxhandle)
		if nxtype != axis.nxtype:
			print "types don't match. cannot load data set"
			return 0
		if dims != axis.dims:
			print "dimensions don't match. cannot load data set"
			return 0
	
		status, axis.data = NXgetdata(self.nxhandle)		
		status, axis.gridinfo, type = NXgetattr(self.nxhandle, "cursor", NX_INT32)
		if status != 1:
			print "axis gridinfo not found in nxfile %s"%(self.nxcachefile)
			print "WARNING: initializing to Null -> ax data gets overwritten"
			axis.gridinfo = 0
		status = NXclosedata(self.nxhandle)
		
		#status = NXopendata(self.nxhandle, axis.name+"_gridinfo")
		#if status!=1:
		#	print "axis gridinfo not found in nxfile %s"%(self.nxcachefile)
		#	print "WARNING: initializing to Null -> ax data gets overwritten"
		#	axis.gridinfo = 0
		#	return 0
		#status,rank,dims,nxtype = NXgetinfo(self.nxhandle)
		#if nxtype != NX_INT32:
		#	print "types don't match. cannot load data set"
		#	return 0
		#if dims != [1]:
		#	print "dimensions don't match. cannot load data set"
		#	return 0
	
		#status, axis.gridinfo = NXgetdata(self.nxhandle)		
		#status = NXflush(self.nxhandle)
		#status = NXclose(self.nxhandle)
		
		self.nxhandle = None
		axis.trimAxDataToGridinfo()
		return 1


def pathStringToList(pathstr):
	pathlist = string.split(pathstr, "/")
	for i in range(len(pathlist)):
		pair = string.split(pathlist[i], ":", 2)
		if len(pair)>1:
			pathlist[i] = (pair[0], pair[1])
		else:	
			pathlist[i] = (pair[0], "")
	return pathlist	
