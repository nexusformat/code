#!/usr/bin/python

import sys
import types
import cPickle
import string

import nxtemplates
import nxfile

from nxfile import *
from nexus import *



class NXgroupbase:
	def __init__(self, group=None, name="", nxclass="", attrs=None, elems=None, groups=None, path=None, parent=None):
		if group==None:
			self.group = nxfile.NXgroup(name=name, nxclass=nxclass, attrs=attrs, elems=elems, groups=groups, path=path, parent=parent)
		else:
			self.group = group
			
		self.nxclass = nxclass
		
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


	def getAttr(self, attr_name):
		if self.group != None:
			if attr_name in nxtemplates.group_types[self.nxclass].attrs.keys():
				if self.group.attrs != None:
					if attr_name in self.group.attrs.keys():
						return self.group.attrs[attr_name]
					else:	
						print "attr valid. but not in group"
				else:
					print "attr not valid for this group"
		return None	
	

	def getElem(self, elem_name):
		if self.group != None:
			if elem_name in nxtemplates.group_types[self.nxclass].elems.keys():
				if self.group.elems != None:
					if elem_name in self.group.elems.keys():
						return self.group.elems[elem_name]
					else:	
						print "Elem valid. but not in group"
				else:
					print "Elem not valid for this group"
		return None	

		
	def getGroup(self, group_name, group_class):
		if self.group != None:
			if group_class in nxtemplates.group_types[self.nxclass].groups.keys():
				if group_name in self.group.groups.keys():
					#for a in self.group.groups.keys():
						#print "group %s: %s"%(a, self.group.groups[a])
					#print self.group.groups[group_name]
					#print "returning the instance"
					return self.group.groups[group_name]
				else:
					print "%s valid. but not in group"%(group_name)
					#for a in self.group.groups.keys():
						#print "group it: ", self.group.groups[a]
			else:
				print "%s not valid for this group"%(group_class)
		else:
			print "self.group is None"
		return None	
	
	
	def setAttr(self, attr):
		if self.group != None:
			if attr.getName() in nxtemplates.group_types[self.nxclass].attrs.keys():
				if self.group.attrs== None:
					self.group.attrs={}
				self.group.attrs[attr.getName()] = attr
				return 1
		print "attr not part of GROUP!!! Concatenation FAILED"		
		return 0	
	
	
	def setElem(self, elem):
		if self.group != None:
			if elem.getName() in nxtemplates.group_types[self.nxclass].elems.keys():
				#check element dimensions
				temp_dims = nxtemplates.group_types[self.nxclass].elems[elem.getName()].dims
				if len(temp_dims) != len(elem.dims):
					print "dimensions don't match, elem %s not loaded"%(elem.getName())
					return 0
				for i in range(len(temp_dims)):
					if temp_dims[i] >= 0:
						if elem.dims[i] != temp_dims[i]:
							print "WARNING: dimension ranges don't match, elem %s loaded though."%(elem.getName())
						
				#check type	
				if elem.nxtype not in nxtemplates.group_types[self.nxclass].elems[elem.getName()].types:
					#print elem.nxtype, nxtemplates.group_types[self.nxclass].elems[elem.getName()].types
					print "type of elem %s in group %s doesn't match DTD"%(elem.getName(), self.nxclass)
					return 0
				if self.group.elems== None:
					self.group.elems={}
				self.group.elems[elem.getName()] = elem
				return 1
		print "elem not part of GROUP!!! Concatenation FAILED"		
		return 0	
		
	
	def setMetaGroup(self, group):
		if self.group!=None:
			if group.nxclass in nxtemplates.group_types[self.nxclass].groups.keys():
				if self.group.groups == None:
					self.group.groups = {}
				else:	
					if nxtemplates.group_types[self.nxclass].groups[group.nxclass].maxOccur < 2:
						for g in self.group.groups.keys():
							if self.group.groups[g].group.nxclass == group.nxclass:
								print "only one group of class %s allowed in groups of class %s"%(group.nxclass, self.group.groups[g].group.nxclass)
								return 0
				self.group.groups[group.group.name] = group
				return 1
		print "group is not a subgroup of this GROUP!!! Concatenation FAILED"		
		return 0			
	
	
	def setGroup(self, metagroup):
		return self.setMetaGroup(metagroup)


	def validate(self):
		# test if entrys of self.group correspond to nxtemplates.group_types[self.nxclass]
		# implement later
		pass
	
	
		




class NXdatabase(NXgroupbase):
	# need to subclass groupbase for NXdata entrys because the elems included may have arbitrary names 
	# -> no (standard) validation is possible  
	def __init__(self, group=None, name="", nxclass="", attrs=None, elems=None, groups=None, path=None, parent=None):
		NXgroupbase.__init__(self, group=group, name=name, nxclass=nxclass, attrs=attrs, elems=elems, groups=groups, path=path, parent=parent)
			
	def getElem(self, elem_name):
		if self.group != None:
			if self.group.elems != None:
				if elem_name in self.group.elems.keys():
					return self.group.elems[elem_name]
				else:	
					print "Elem not in group"
		return None	

		
	def getGroup(self, group_name, group_class):
		if self.group != None:
			if group_name in self.group.groups.keys():
				return self.group.groups[group_name]
			else:
				print "%s valid. but not in group"%(group_name)
		else:
			print "self.group is None"
		return None	
	
	
	def setElem(self, elem):
		if self.group != None:
			if self.group.elems== None:
				self.group.elems={}
			self.group.elems[elem.getName()] = elem
			return 1
		return 0	
		



class NXdata(NXdatabase):
	def __init__(self, group=None, name="", nxclass="NXdata", attrs=None, elems=None, groups=None, path=None, parent=None):
		NXdatabase.__init__(self, group=group, name=name, nxclass=nxclass, attrs=attrs, elems=elems, groups=groups, path=path, parent=parent)
		
	def setData(self, data):
		return self.setElem(data)

	def getData(self, data_name):
		return self.getElem(data_name)
		
	def setAxes(self, axlist):
		if axlist != None:
			if type(axlist) != types.ListType and type(axlist) != types.TupleType:
				print "please provide axes as list or tuple of axis instance objects"
				return 0
		for ax in axlist:
			self.setElem(ax)
		return 1	


	
	

