
class NXattr_type:
	def __init__(self, name="", info="", desc="", minOccur=0, enums=None):	
		if enums == None:
			enums = []
			
		self.name = name
		self.enums = enums
		self.info = info
		self.desc = desc
		self.minOccur = minOccur
		self.maxOccur = 1


class NXelem_type:
	def __init__(self, name="", types=None, dims=None, info="", desc="", minOccur=0, enums=None, attrs=None):
		if enums == None: enums = []
		if types == None: types = []
		if dims == None: dims = []
		if attrs == None: attrs = {}
		
		self.name = name
		self.enums = enums
		self.types = types
		self.dims = dims
		self.info = info
		self.desc = desc
		self.minOccur = minOccur
		self.maxOccur = 1
		self.attrs = attrs
	
	def hasAttr(self, attrname):
		if self.attrs != None:
			if attrname in self.attrs.keys():
				return 1
		return 0
		
		

class NXgroup_type:
	def __init__(self, nxclass="", minOccur=0, maxOccur=1, groups=None, elems=None, attrs=None):
		if groups == None: groups = {}
		if elems == None: elems = {}
		if attrs == None: attrs = {}
		
		self.nxclass = nxclass
		self.minOccur = minOccur
		self.maxOccur = maxOccur
		self.groups = groups
		self.elems 	= elems
		self.attrs  = attrs
	
	def hasAttr(self, attrname):
		if self.attrs != None:
			if attrname in self.attrs.keys():
				return 1
		return 0
		
	def hasElem(self, elemname):
		if self.elems != None:
			if elemname in self.elems.keys():
				return 1
		return 0		
			
	def hasGroup(self, groupclass):	
		if self.groups != None:
			if groupclass in self.groups.keys():
				return 1
		return 0	


