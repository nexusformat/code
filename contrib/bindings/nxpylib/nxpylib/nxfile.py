#!/usr/bin/python

import sys
import types
import cPickle
import time
from string import *

from nexus import *

import nxtemplates
from nxcomponents import *
from nxobjects import *
from nxclasses import *



class NXfile:
	FLAT = 0
	DEEP = 1
	
	def __init__(self, filename, mode):
		status, self.handle = NXopen(filename, mode)
		if status != 1:
			print "Error. can't open file: %s" %(filename)
		self.root = self.readRoot()	


	def readRoot(self):
		return self.readSingleGroup("root", "NXroot", self.FLAT, [("root","NXroot")])


	def getRoot(self):
		return self.root 
	
	
	def save(self):
		self.writeGroup(self.root)
	
	
		
	def readGroup(self, pathstr, mode=DEEP):
		pathlist = self.pathStringToList(pathstr)
		
		group = self.readGroupFromFile(pathlist, mode)
		
		if group ==0:
			group = None
			
		return group 
		
	
	
	def readGroupFromFile(self, pathlist, mode):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		if not self.pathValid(pathlist):
			return 0
			
		if not self.openPath(pathlist):
			return 0
		
		groupname  = pathlist[len(pathlist)-1][0]
		groupclass = pathlist[len(pathlist)-1][1]
		
		if groupclass[0:2] != "NX":
			print "invalid path string. "
			return 0
			
		group = self.readSingleGroup(groupname, groupclass, mode, pathlist)
		return group


	def readSingleGroup(self, groupname, groupclass, mode, pathlist):
		#print "pathlist init: ", pathlist
		group = None
		groups = {}
		elems  = {}
		attrs  = {}

		#first read attributes
		status = NXinitattrdir(self.handle)
		if status != 1:
			print "NXinitattrdir failed"
			return 0
			
		status, nattrs = NXgetattrinfo(self.handle)
		if status == 1:
			for i in range(nattrs):
				status,name,length,type = NXgetnextattr(self.handle)
				if status != 1:
					print "NXgetnextattr failed"
					continue
				status,value,type = NXgetattr(self.handle, name, type)
				if status != 1:
					print "NXgetattr failed"
					continue
					
				if nxtemplates.group_types[groupclass].hasAttr(name):
					attrs[name] = NXattr(path=pathlist, name=name, value=value, nxtype=type) 
				else:
					pass
					#print "group %s has no attribute %s	"%(groupclass, name)
					
		#now read data and groups
		status, nitems, namepath, nxclass = NXgetgroupinfo(self.handle)
		if status != 1:
			print "NXgetgroupinfo failed"
			return 0
		names = split(namepath, "/")
		name = names[len(names)-1]
		#print "getgroupinfo returned: ", status, nitems, name, nxclass
		status = NXinitgroupdir(self.handle)
		if status != 1:
			print "NXinitgroupdir failed"
			return 0
			
		#print "\n\n\nnitems: ", nitems
		for i in range(nitems):
			status,ename,nxclass,type = NXgetnextentry(self.handle)
			if status != 1:
				print "NXgetnextentry failed .unknown class %s"%(nxclass)
				continue
			if nxclass == "SDS":
				#got a Data Set
				if groupclass != "NXdata":
					if not nxtemplates.group_types[groupclass].hasElem(ename):
						print "group %s has no elem %s	"%(groupname, ename)
						continue
					
				status = NXopendata(self.handle, ename)
				if status !=1:
					print "opening dataset %s failed"%(ename)
					continue
							
				#get all attributes of data set
				elemattrs = {}
				status = NXinitattrdir(self.handle)
				if status !=1:
					print "NXinitattrdir failed"
					continue
				status, nattrs = NXgetattrinfo(self.handle)
				#print "NXgetattrinfo returned: ", status, nattrs
				if status !=1:	
					print "NXgetattrinfo failed"
					continue
					
				for j in range(nattrs):
					status,attname,length,atttype = NXgetnextattr(self.handle)
					if status != 1:
						print "NXgetnextattr failed"
						continue
					status,attvalue,atttype = NXgetattr(self.handle, attname, atttype)
					if status != 1:
						print "NXgetattr failed"
						continue
						
					if groupclass != "NXdata":
						if not nxtemplates.group_types[groupclass].elems[ename].hasAttr(attname):
							#print "elem %s has no attribute %s	"%(ename, attname)
							continue
							
					nlist=pathlist[:]
					nlist.append((ename,"SDS"))
					elemattrs[attname] = NXattr(path=nlist, name=attname, value=attvalue, nxtype=atttype)
					
							
							
					
				#get data set information
				status, rank, dims, elemtype = NXgetinfo(self.handle)		
				#print "NXgetinfo returned: ", status, rank, dims, elemtype
				
				if groupclass != "NXdata":
					#check dims
					temp_dims = nxtemplates.group_types[groupclass].elems[ename].dims
					if len(temp_dims) != len(dims):
						print "dimensions don't match, elem %s not loaded"%(ename)
						continue
					for i in range(len(temp_dims)):
						if temp_dims[i] >= 0:
							if dims[i] != temp_dims[i]:
								print "WARNING: dimension ranges don't match, elem %s loaded though."%(ename)
					#check type	
					#print elemtype, nxtemplates.group_types[groupclass].elems[ename].types
					if elemtype not in nxtemplates.group_types[groupclass].elems[ename].types:
						print "type of elem %s in group %s doesn't match DTD"%(ename, groupclass)
						continue
				# build path	
				nlist=pathlist[:]
				nlist.append((ename,"SDS"))
				elems[ename] = NXelem(path=nlist, name=ename, nxtype=elemtype, dims=dims, attrs=elemattrs)
				#get data of data set
				status, data = NXgetdata(self.handle)
				if status != 1:
					print "NXgetdata failed"
					continue
				#print "NXgetdata returned: ", status, data
				elems[ename].setData(data)
				
			elif nxclass[0:2] == "NX" and mode==self.DEEP:
				# Group 
				#print "step from %s into subgroup %s"% (groupclass, ename)
				if nxtemplates.group_types[groupclass].hasGroup(nxclass):
					if nxtemplates.group_types[groupclass].groups[nxclass].maxOccur < 2:
						found = 0
						for g in groups.keys():
							if groups[g].group.nxclass == nxclass:
								print "only one group of class %s allowed in groups of class %s"%(nxclass, groups[g].group.nxclass)		
								found = 1
								break
						if found == 1:
							continue
					#print "opening group: %s %s"%(ename, nxclass)
					status = NXopengroup(self.handle, ename, nxclass)
					if status ==1:
						#print "reading subbranch: %s"%(ename)
						nlist =	pathlist[:]
						nlist.append((ename,nxclass))
						groups[ename] = self.readSingleGroup(ename, nxclass, mode, nlist)
		
		#print "ok, now build group %s %s" %(groupname, groupclass)
		group = NXgroup(path=pathlist, name=groupname, nxclass=groupclass, attrs=attrs, elems=elems, groups=groups)
		status = NXclosegroup(self.handle)
		#print "group path check",group.path
		return nx_classes[groupclass](group=group)
	
	
	def readSubGroup(self, metagroup, subgroupid):
		subgrouptuple = split(subgroupid, ":", 2)
		subgrouppath = metagroup.group.path[:]
		subgrouppath.append((subgrouptuple[0], subgrouptuple[1]))
		return self.readGroupFromFile(subgrouppath, self.FLAT)
		
	
	def readElem(self, pathstr):
		pathlist = self.pathStringToList(pathstr)
#		pathlist = split(pathstr, "/")
#		for i in range(len(pathlist)):
#			pair = split(pathlist[i], ":", 2)
#			pathlist[i] = (pair[0], pair[1])
			
		elem = self.readElemFromFile(pathlist)
		if elem == 0:
			elem = None
		return elem 
	
	
	
	def readElemFromFile(self, pathlist):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		elemname  = pathlist[len(pathlist)-1][0]
		elemclass = pathlist[len(pathlist)-1][1]
		
		if elemclass != "SDS":
			print "invalid path string. "
			return 0
		
		if not self.pathValid(pathlist):
			return 0
			
		if not self.openPath(pathlist):
			return 0

		groupname  = pathlist[len(pathlist)-2][0]
		groupclass = pathlist[len(pathlist)-2][1]
			
		attrs  = {}

		#first read attributes
		status = NXinitattrdir(self.handle)
		status, nattrs = NXgetattrinfo(self.handle)
		#print "number of attributes %d"%nattrs
		if status == 1:
			for i in range(nattrs):
				status,attname,length,type = NXgetnextattr(self.handle)
				if status == 1:
					status,value,type = NXgetattr(self.handle, attname, type)
					if status == 1:	
						if nxtemplates.group_types[groupclass].elems[elemname].hasAttr(attname):
							attrs[attname] = NXattr(path=pathlist, name=attname, value=value, nxtype=type) 
						else:
							print "elem %s has no attribute %s	"%(elemname, attname)
					else:
						print "NXerror getting attribute value"
				else:
					print "NXerror fetching next attribute"
		
		#get data set information
		status, rank, dims, elemtype = NXgetinfo(self.handle)		
		#print "NXgetinfo returned: ", status, rank, dims, elemtype

		#check element dimensions
		temp_dims = nxtemplates.group_types[groupclass].elems[elemname].dims
		if len(temp_dims) != len(dims):
			print "dimensions don't match, elem %s not loaded"%(elemname)
			return 0
		#check type
		#print elemtype, nxtemplates.group_types[groupclass].elems[elemname].types
		if elemtype not in nxtemplates.group_types[groupclass].elems[elemname].types:
			print "type of elem %s in group %s doesn't match DTD"%(elemname, groupclass)
			return 0

		for i in range(len(temp_dims)):
			if temp_dims[i] >= 0:
				if dims[i] != temp_dims[i]:
					print "WARNING: dimension ranges don't match, elem %s loaded though."%(elemname)
					
		elem = NXelem(path=pathlist, name=elemname, nxtype=elemtype, dims=dims, attrs=attrs)
		
		#get data of data set
		status, data = NXgetdata(self.handle)
		#print "NXgetdata returned: ", status, data
		if status == 1:
			elem.setData(data)
		
		status = NXclosedata(self.handle)
		return elem
		
	

	def readData(self, pathstr):
		pathlist = self.pathStringToList(pathstr)
#		pathlist = split(pathstr, "/")
#		for i in range(len(pathlist)):
#			pair = split(pathlist[i], ":", 2)
#			pathlist[i] = (pair[0], pair[1])
			
		if not self.pathValid(pathlist):
			return 0
		
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		elemname  = pathlist[len(pathlist)-1][0]
		elemclass = pathlist[len(pathlist)-1][1]
		
		if elemclass != "SDS":
			print "invalid path string. "
			return 0
		
		if not self.openPath(pathlist):
			return 0

		#get data of data set
		status, data = NXgetdata(self.handle)
		#print "NXgetdata returned: ", status, data
		if status != 1:
			return status
			
		status = NXclosedata(self.handle)
		return data



	def readAttr(self, pathstr):
		pathlist = self.pathStringToList(pathstr)
#		pathlist = split(pathstr, "/")
#		for i in range(len(pathlist)):
#			pair = split(pathlist[i], ":", 2)
#			if len(pair)>1:
#				pathlist[i] = (pair[0], pair[1])
#			else:	
#				pathlist[i] = (pair[0], "")
		
		attr = self.readAttrFromFile(pathlist)
		
		if attr == 0:
			attr = None
			
		return attr



	def readAttrFromFile(self, pathlist):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		attname  = pathlist[len(pathlist)-1][0]
		pathlist = pathlist[:len(pathlist)-1]
		
		if not self.pathValid(pathlist):
			return 0
			
		if not self.openPath(pathlist):
			return 0

		parentname  = pathlist[len(pathlist)-1][0]
		parentclass = pathlist[len(pathlist)-1][1]

		groupname  = pathlist[len(pathlist)-2][0]
		groupclass = pathlist[len(pathlist)-2][1]
		
		attr = None
		
		#first get type
		status = NXinitattrdir(self.handle)
		status, nattrs = NXgetattrinfo(self.handle)
		if status == 1:
			for i in range(nattrs):
				status,aname,length,type = NXgetnextattr(self.handle)
				if status == 1:
					if aname == attname:
						break
				else:
					print "cannot determine attr type "
					break
					
		# now read attribute				
		status,value,type = NXgetattr(self.handle, attname, attname)
		if status == 1:
			if parentclass == "SDS":
				if not nxtemplates.group_types[groupclass].elems[parentname].hasAttr(attname):
					print "elem %s has no attribute %s	"%(parentname, attname)
					return 0
			elif parentclass[0:2]=="NX":
				if not nxtemplates.group_types[parentclass].hasAttr(attname):
					print "group %s has no attribute %s	"%(parentclass, attname)
					return 0
			attr = NXattr(path=pathlist, name=attname, value=value, nxtype=type) 
		else:
			print "NXerror getting attribute value"
		
		status = NXclosedata(self.handle)
		return attr
	
		

	def writeGroup(self, metagroup, mode=DEEP):
		return self.writeGroupToFile(metagroup.group, mode)


	def writeGroupToFile(self, group, mode):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		if group.path[len(group.path)-1][1][0:2] != "NX":
			return 0
			
		if not self.pathValid(group.path):
			print "path is NOT valid\n\n"
			return 0
		
		parentpath = group.path[:len(group.path)-1]
		if not self.openPath(parentpath):
			return 0
			
		return self.writeSingleGroup(group, mode)
	
	
	def writeSingleGroup(self, group, mode):
		# first check if group already exists ...
		#print "path:", group.path
		groupname = group.path[len(group.path)-1][0]
		groupclass = group.path[len(group.path)-1][1]

		if groupclass != "NXroot":
			status = NXinitgroupdir(self.handle)
			status, nitems, path, parent_nxclass = NXgetgroupinfo(self.handle)
			found = 0
			for i in range(nitems):
				status, name, nxclass, nxtype = NXgetnextentry(self.handle)
				if nxclass == groupclass:
					#check if group exists
					if name == groupname:
						status = NXopengroup(self.handle, groupname, groupclass)
						found = 1
					else:	
						# check maxxoccur
						if nxtemplates.group_types[parent_nxclass].groups[nxclass].maxOccur < 2:
							print "only one group of class %s allowed in groups of class %s"%(nxclass, parent_nxclass)	
							return 0
			if found == 0:
				status = NXmakegroup(self.handle, groupname, groupclass)
				if status != 1:
					print "NXmakegroup failed" 
					return 0
				status = NXopengroup(self.handle, groupname, groupclass)
				if status != 1:
					"NXopengroup failed"
					return 0
			
		#first write attributes
		if group.attrs != None:
			for attname in group.attrs.keys():
				attval = group.attrs[attname].getValue()
				atttype = group.attrs[attname].getType()
				attlength = group.attrs[attname].getLength()
			
				if attval != None:
					status = NXputattr(self.handle, attname, attval, atttype) 
					if status !=1:
						print "NXputattr failed: ", attname, attval, atttype
	

	   
		#now write data and groups
		if group.elems != None:
			for elemname in group.elems.keys():
				elemdata = group.elems[elemname].getData()
				elemtype = group.elems[elemname].getType()
				elemrank = group.elems[elemname].getRank()
				elemdims = group.elems[elemname].getDims()
				#get rid of -1 in dims
				for i in range(len(elemdims)):
					if elemdims[i] == -1:
						elemdims[i] = 1024
				#check if elem already exists
				status = NXinitgroupdir(self.handle)
				status, nitems, path, parent_nxclass = NXgetgroupinfo(self.handle)
				found = 0
				for i in range(nitems):
					status, name, nxclass, nxtype = NXgetnextentry(self.handle)
					#print "nxclass", name, nxclass
					if nxclass == "SDS":
						#check if group exists
						if name == elemname:
							status = NXopendata(self.handle, elemname)
							found = 1
							break
				if found == 0:
					#print "elem wasn't found -> create it", elemname, elemtype, elemrank, elemdims
					status = NXmakedata(self.handle, elemname, elemtype, elemrank, elemdims)
					if status != 1:
						print "couldn't create Data set", elemname, elemtype, elemrank, elemdims
						continue
					status = NXopendata(self.handle, elemname)
					if status != 1:
						print "NXopendata failed"
						continue
					
				#data set must be open now, so put data and attributes
				status = NXputdata(self.handle, elemdata)
				if status != 1:
					print "failed to set data"
			
				#now for the attributes 
				if group.elems[elemname].attrs != None:
					if len(group.elems[elemname].attrs) > 0:
						for attname in group.elems[elemname].attrs.keys():
							attval = group.elems[elemname].attrs[attname].getValue()
							atttype = group.elems[elemname].attrs[attname].getType()
							attlength = group.elems[elemname].attrs[attname].getLength()
				
							if attval != None:
								#print "setting attribute"
								status = NXputattr(self.handle, attname, attval, atttype) 
								if status !=1:
									print "NXputattr failed: ", attname, attval, atttype
		ret = 1		
		#ok, now for the subgroups
		if mode==self.DEEP:
			if group.groups != None:
				for groupname in group.groups.keys():
					#print "writing of group ",groupname
					#print "instance ",group.groups[groupname].group
					ret = self.writeSingleGroup(group.groups[groupname].group, mode)	
					if ret != 1:
						print "writing of group %s failed"%(groupname)
						ret = 0
		status = NXclosegroup(self.handle)
		return ret
	

	def writeElem(self, elem):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		if elem.path[len(elem.path)-1][1] != "SDS":
			elem.path.append((elem.getName(), "SDS"))	
			
		if not self.pathValid(elem.path):
			print "path is NOT valid\n\n"
			return 0
		
		parentpath = elem.path[:len(elem.path)-1]
		if not self.openPath(parentpath):
			return 0
		
		elemname = elem.getName()
		elemdata = elem.getData()
		elemtype = elem.getType()
		elemrank = elem.getRank()
		elemdims = elem.getDims()
		
		#check if elem already exists
		status = NXinitgroupdir(self.handle)
		status, nitems, path, parent_nxclass = NXgetgroupinfo(self.handle)
		found = 0
		for i in range(nitems):
			status, name, nxclass, nxtype = NXgetnextentry(self.handle)
			if nxclass == "SDS":
				#check if group exists
				if name == elemname:
					status = NXopendata(self.handle, elemname)
					found = 1
					break
		if found == 0:
			status = NXmakedata(self.handle, elemname, elemtype, elemrank, elemdims)
			if status != 1:
				print "couldn't create Data set", elemname, elemtype, elemrank, elemdims
				return 0
			status = NXopendata(self.handle, elemname)
			
		if status != 1:
			return 0
			
		#data set must be open now, so put data and attributes
		status = NXputdata(self.handle, elemdata)
		if status != 1:
			print "failed to set data"
			return 0
		
		#now for the attributes 
		for attname in elem.attrs.keys():
			attval = elem.attrs[attname].getValue()
			atttype = elem.attrs[attname].getType()
			attlength = elem.attrs[attname].getLength()
			#print "attlength: ", attlength	
			if attval != None:
				status = NXputattr(self.handle, attname, attval, atttype) 
				if status !=1:
					print "NXputattr failed: ", attname, attval, atttype
					return 0
		return 1			
		


	def writeAttr(self, attr):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		if not self.pathValid(attr.path):
			return 0
			
		if not self.openPath(attr.path):
			return 0

		#write attribute
		attval = attr.getValue()
		atttype = attr.getType()
		attlength = attr.getLength()
		
		if attval != None:
			status = NXputattr(self.handle, attname, attval, atttype) 
			if status !=1:
				print "NXputattr failed: ", attname, attval, atttype
				return 0
		return 1
		
		
	def writeData(self, elem):
		if self.handle == None:
			print "Error: handle hasn't been initialized yet"
			return 0
		
		if elem.path[len(elem.path)-1][1] != "SDS":
			elem.path.append((elem.getName(), "SDS"))	
			
		if not self.pathValid(elem.path):
			print "path is NOT valid\n\n"
			return 0
		
		parentpath = elem.path[:len(elem.path)-1]
		if not self.openPath(parentpath):
			return 0
		
		elemname = elem.getName()
		elemdata = elem.getData()
		
		#check if elem already exists
		status = NXopendata(self.handle, elemname)
		if status != 1:
			return 0
			
		#data set must be open now, so put data and attributes
		status = NXputdata(self.handle, elemdata)
		if status != 1:
			print "failed to set data"
			return 0
		
		return 1			
	


	def pathValid(self, pathlist):
		# TODO check root !!!!
		if len(pathlist) == 1 and pathlist[0][1] != "NXroot":
			if not nxtemplates.group_types["NXroot"].hasGroup(pathlist[0][1]):
				return 0
		for i in range(len(pathlist)-1):
			if pathlist[i+1][1][0:2] == "NX":
				if not nxtemplates.group_types[pathlist[i][1]].hasGroup(pathlist[i+1][1]):
					return 0
			elif pathlist[i+1][1] =="SDS":
				if not nxtemplates.group_types[pathlist[i][1]].hasElem(pathlist[i+1][0]):
					return 0
			elif pathlist[i+1][1] == "":
				if pathlist[i][1][0:2] == "NX":
					if not nxtemplates.group_types[pathlist[i][1]].hasAttr(pathlist[i+1][0]):
						return 0
				elif pathlist[i][1] == "SDS":
					if not nxtemplates.group_types[pathlist[i-1][1]].elems[pathlist[i][0]].hasAttr(pathlist[i+1][0]):
						return 0
				else:
					return 0
			else:
				return 0
		return 1			
					
				
		
	def openPath(self, path):
		if not self.closePath():
			return 0
			
		path2 = path[1:]	
		for groupitem in path2:
			if groupitem[1][0:2]=="NX":
				status = NXopengroup(self.handle, groupitem[0], groupitem[1])
			elif groupitem[1] == "SDS":
				status = NXopendata(self.handle, groupitem[0])
			else:
				return 0

			if status != 1:
				return 0
					
		return 1		
	
	
	def closePath(self):
		status, nitems, name, nxclass = NXgetgroupinfo(self.handle)	
		while nxclass!="NXroot":
			if nxclass == "SDS":
				if not NXclosedata(self.handle):
					return 0
			else:	
				if not NXclosegroup(self.handle):
					return 0
			status,numelems, name, nxclass = NXgetgroupinfo(self.handle)
		return 1
	
	
	def pathStringToList(self, pathstr):
		pathlist = split(pathstr, "/")
		for i in range(len(pathlist)):
			pair = split(pathlist[i], ":", 2)
			if len(pair)>1:
				pathlist[i] = (pair[0], pair[1])
			else:	
				pathlist[i] = (pair[0], "")
		return pathlist	
	

	def close(self):
		status = NXflush(self.handle)
		stauts = NXclose(self.handle)
		return status

