#!/usr/bin/python
import sys
import libxml2
import string
import os
import time
import types

from nexus import *
from nxtemplatecomponents import *



class NXfactory:

	def __init__(self, metagroup=None, nxstructfile="", nxclassfile="", dictclassfile=""):
		if metagroup == None:
			self.metagroup = NXgroup_type("", 0, 0, None, None, None)
		else:	
			self.metagroup = metagroup
		self.nxclassfile = nxclassfile
		self.nxstructfile = nxstructfile
		self.dictclassfile = dictclassfile
		self.buildTypeDict()	
		

	def readFromXMLTemplate(self, template, filename):
		if not self.fileExists(template):
			print "error: template file %s found" %(template)
			return 0
		
		doc = libxml2.parseFile(template)
		
		if doc.name != template:
			print "doc.name failed"
			sys.exit(1)

		root = doc.children
		while root.type=="comment":
			root = root.next
			
		self.metagroup.nxclass = root.name

		status, nxhandle = NXopen(filename, NXACC_CREATE5)
		#print "opening filename", filename
		
		nxhandle = self.parseTemplateRootAttrs(root, nxhandle)
		nxhandle = self.parseTemplateNode(root.children, nxhandle)
		
		doc.freeDoc()
	
		status = NXflush(nxhandle)
		if status != 1:
			print "warning: flushing NeXus file failed"
		status = NXclose(nxhandle)
		if status != 1:
			print "warning: closing Nexus file failed" 
	
	
	def parseTemplateRootAttrs(self, item, nxhandle):
		if item.properties != None:
			for att in item.properties:
				if att.type == "attribute":
					# got an attribute
					#todo: convert attcontent to type !!!!
					#too complicated -> where to get type info !?!?
					#always treat attributes as strings
					#alternatively: check if string may be converted to float or int
					NXputattr(nxhandle, att.name, self.convertXMLToNXAtt(att.content), NX_CHAR)
		return nxhandle
	
	
	def parseTemplateNode(self, item, nxhandle):
		""" running through the xml tree using libxml2"""
		while item != None:
			if item.type == "element":
				if item.name[0:2] == "NX":
					if item.properties != None:
						for att in item.properties:
							if att.type == "attribute":
								if att.name != "name":
									#print "putting a group element attribute", att.name, att.content 
									NXputattr(nxhandle, att.name, att.content, NX_CHAR)	
								
				if item.name[0:2] == "NX": #NXgroup
					groupname = item.name
					for att in item.properties:
						if att.type == "attribute":
							if att.name == "name":
								groupname = att.content
					#make group
					#print "making group", groupname, item.name
					status = NXmakegroup(nxhandle, groupname, item.name)
					if status != 1:
						print "creating group %s of class %s failed"%(groupname, item.name)
						return 0
					#open group 
					#print "try to open group"
					status = NXopengroup(nxhandle, groupname, item.name)
					if status != 1:
						print "opening group %s of class %s failed"%(groupname, item.name)
						return 0
					#print "stepping a level deeper", item.name, groupname  	
					nxhandle = self.parseTemplateNode(item.children, nxhandle)
					if nxhandle == 0:
						return 0
					
					#print "closing group"
					NXclosegroup(nxhandle)

				else:  #SDS
					#print "name", item.name
					try: 
						elemcontent = item.children.listGetString(item.doc, 1)
					except:
						elemcontent = None
						
					attrs = None
					elemtype = NX_CHAR
					elemdims = []
					if item.properties != None:
						for att in item.properties:
							if att.type == "attribute":
								attcontent = att.content
								if att.name == "type":
									dimstring = att.content
									if string.find(attcontent, "[")>-1: 
										dimstring = attcontent[string.find(attcontent, "[")+1:string.find(attcontent[0], "]")]
										if string.find(dimstring, ",")>-1:
											tdims = map(string.strip, string.split(dimstring, ","))
											for i in range(len(tdims)):
												if tdims[i] != "":
													if tdims[i][0] >= "0" and tdims[i][0] <="9":
														elemdims.append(string.atoi(tdims[i]))
													else:
														elemdims.append(tdims[i])	
										else:
											elemdims = [int(dimstring)] 
									else:
										elemdims = None
											
									if string.find(attcontent, "[")>-1: 
										cutstring = attcontent[string.find(attcontent, "["):string.find(attcontent, "]")+1]
										if cutstring != "":
											typestring = string.replace(attcontent, cutstring, "")
										elemtype = self.StringToNXType(self.correctTypeString(typestring))
									else:
										elemtype = self.StringToNXType(self.correctTypeString(attcontent))
										
										
									if elemdims == None or dimstring=="":
										if elemtype == NX_CHAR:
											elemdims = [len(elemcontent)]
										else:
											elemdims = [1]
										
									break	
					if elemtype == NX_CHAR and elemdims==[]:
						elemdims = [len(elemcontent)]
						
					#make data set
					status = NXmakedata(nxhandle, item.name, elemtype, len(elemdims), elemdims)
					if status != 1:
						print "making data set %s failed. aborting"%(item.name)
						return 0
					#open dataset
					status = NXopendata(nxhandle, item.name)
					if status != 1:
						print "opening data set %s failed. aborting"%(item.name)
						return 0
					#put data
					if elemcontent != None:
						#print "putting data:", elemcontent
						status = NXputdata(nxhandle, self.convertXMLToNXData(elemcontent, elemdims, elemtype))
						if status != 1:
							print "putting data to dataset %s failed. aborting"%(item.name)
							return 0
						#print "data put into ", item.name, self.convertXMLToNXData(elemcontent, elemdims, elemtype)
					else:
						print "elemcontent of ", item.name, " was None"
					
					if item.properties != None:
						for att in item.properties:
							if att.type == "attribute":
								if att.name != "type":
									#print "'%s'"%(att.name)
									#print "putting an attribute", att.name, att.content 
									NXputattr(nxhandle, att.name, att.content, NX_CHAR)
					status = NXclosedata(nxhandle)
					if status != 1:
						print "closing dataset %s failed"%(item.name)
						return 0
			#print " "			
			item = item.next			
		return nxhandle	



	def convertXMLToNXData(self, xmldata, dims, type):
		int_types 	= [NX_INT8, NX_UINT8, NX_INT16, NX_UINT16, NX_INT32, NX_UINT32]	
		float_types = [NX_FLOAT32, NX_FLOAT64]
		str_types 	= [NX_CHAR]
		
		totaldimcounts = 1
		for i in range(len(dims)):
			totaldimcounts = totaldimcounts*dims[i]
			
		if type in int_types:
			ints = []
			int_toks = string.split(xmldata)
			for i in range(len(int_toks)):
				ints.append(int(int_toks[i]))
				
			if totaldimcounts != len(ints):
				print "warning: count of int data elems does not match dimensions", totaldimcounts, len(ints), xmldata
				
			return ints
			
		elif type in float_types:	
			floats = []
			float_toks = string.split(xmldata)
			for i in range(len(float_toks)):
				floats.append(float(float_toks[i]))
			
			if totaldimcounts != len(floats):
				print "warning: count of float data elems does not match dimensions", totaldimcounts, len(floats), xmldata

			return floats	
			
		elif type in str_types:
			return xmldata
	
	
	
	def checkTypes(self, pytype, nxtype):
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
	
	
	def convertXMLToNXAtt(self, xmlvalue):
		return xmlvalue



	def parseMetaDTD(self, xmlfilename):	
		"""as the name supposes: just a test class for the xml python bindings of gnome"""
		#open xml file and build dom tree
		doc = libxml2.parseFile(xmlfilename)
		if doc.name != xmlfilename:
			print "doc.name failed"
			sys.exit(1)

		root = doc.children
		while root.type=="comment":
			root = root.next
			
		self.metagroup.nxclass = root.name
		self.getRootAttrs(root)
		self.parseNxNode(root.children)
		doc.freeDoc()


	def getRootAttrs(self, item):
		rootattrs = None
		elemtype = NX_CHAR
		if item.properties != None:
			for att in item.properties:
				if att.type == "attribute":
				# got an attribute
					print "ATTRIBUTE: ", att.name
					attcontent = att.content

					if att.name == "type":
						#extract dims and type
						if string.find(attcontent, "{")>-1: 
							dims = attcontent[string.find(attcontent, "{")+1:string.find(attcontent, "}")]
						elemtype = attcontent
						continue	
					else:
						print "attcontent..................................:",	attcontent
						if rootattrs == None:
							rootattrs = {}
						
						if string.find(attcontent, "{")>-1:
							info = attcontent[string.find(attcontent, "{")+1:string.find(attcontent, "}")]
							desc = attcontent[string.rfind(attcontent, "{")+1:string.rfind(attcontent, "}")]
							info = string.replace(info, "'", "\\'")
							desc = string.replace(desc, "'", "\\'")
							if string.find(attcontent, "{")==string.rfind(attcontent, "{"):
								desc = ""
						else:
							info = ""
							desc = ""
						print "info: ", info
						print "desc: ", desc
						#strip description and info
						if info != "":
							attcontent = string.replace(attcontent, info, "")
						if desc != "":
							attcontent = string.replace(attcontent, desc, "")
						attcontent = string.replace(attcontent, "{}", "")
						#extract specifier 
						minOccur=1
						print "attcontent before specext: ", attcontent
						if len(attcontent) > 0:
							if attcontent[len(attcontent)-1] == "?":
								attcontent = attcontent[:len(attcontent)-1]
								minOccur=0
							print "attcontent after specext: ", attcontent
							#extract enums/default values
							print "attcontent before mapping: ", attcontent
							if string.find(attcontent, "|")>-1:
								enums = map(string.strip, string.split(attcontent, "|"))
							else:
								enums = [attcontent]
						else:
							enums = None

						if enums == [""] or enums == "":
							enums = None

						attr = NXattr_type(name=att.name, info=info, desc=desc , minOccur=minOccur, enums=enums)
						rootattrs[att.name] = attr
		self.metagroup.attrs = rootattrs			
		


	def parseNxNode(self, item):
		""" running through the xml tree using libxml2"""

		while item != None:
			if item.type == "element":
				print "element: ", item.name

				grouptype = 0
				if item.properties != None:
					for att in item.properties:
						if att.type == "attribute":
							if att.name == "type":
								classtype = att.content+".xml"
								if classtype in self.nxfiles:
									grouptype = att.content
									break
									
				if item.name[0:2] == "NX" or grouptype != 0:
				#Got A Group
					# adapt groups stored as typed elems
					itemname = item.name
					print "item: ", itemname
					if grouptype != 0:
						itemname = grouptype	
						if self.metagroup.groups == None:
							self.metagroup.groups = {}
						else:
							# if it was stored previously simply continue and do nothing 
							# (not needed -> it simply gets overwritten by the same value)
							#if itemname in self.metagroup.groups.keys():
							#	item = item.next
							#	continue
							self.metagroup.groups[itemname] = NXgroup_type(itemname, 0,0,None,None,None)
						#if we got an NXtype elem we can not assume that no other elem with same type will follow
						# so set max occurence to arbitrary	 
							self.metagroup.groups[itemname].maxOccur = 2
					#got a group -> create group entry
					# search for {} constructs -> remove them
					# init groups
					else:
						occSpec = item.children.listGetString(item.doc, 1)
						if string.find(occSpec, "{")>-1:
							info = occSpec[string.find(occSpec, "{"):string.find(occSpec, "}")+1]
							desc = occSpec[string.rfind(occSpec, "{"):string.rfind(occSpec, "}")+1]
							if info != "":
								occSpec = string.replace(occSpec, info, "")
							if desc != "":	
								occSpec = string.replace(occSpec, desc, "")
						if itemname != self.metagroup.nxclass:	
							if self.metagroup.groups == None:
								self.metagroup.groups = {}
						
							self.metagroup.groups[itemname] = NXgroup_type(itemname, 0,0,None,None,None)
							if occSpec == "*":
								self.metagroup.groups[itemname].minOccur = 0
								self.metagroup.groups[itemname].maxOccur = 2
							elif occSpec == "+":
								self.metagroup.groups[itemname].minOccur = 1
								self.metagroup.groups[itemname].maxOccur = 2
							elif occSpec == "?":
								self.metagroup.groups[itemname].minOccur = 0
								self.metagroup.groups[itemname].maxOccur = 1
							elif occSpec == "":
								self.metagroup.groups[itemname].minOccur = 1
								self.metagroup.groups[itemname].maxOccur = 1
							else:
								print "unrecognized specifier %s. Maybe not a group?"%(occSpec) 	
						
							
				else:
				#Got a Data Set	
					print "got a Data Set"
					
# the following implementation is quite quick and dirty
# it should to be cleaned and split up into functions and modules sometimes					
					
					attrs = None
					elemtypes = [NX_CHAR]
					elemdims = []
					if item.properties != None:
						for att in item.properties:
							if att.type == "attribute":
							# got an attribute
								attcontent = att.content
								print "ATTRIBUTE: ", att.name
								if att.name == "type":
									print "got a type attribute ......................."
									#extract dims and type
									# 1) check paranthesis
									# 		1.1) -1 for arbitrary dimension value (-> : or blank)
									#	   1.2) -2 for arbitrary dimension number
									#		1.3) n for determined dimensions
									#		1.4) no paranthesis mean [1]
									# 2) 
									elemtypes = []
									type_tokens = []
									if string.find(attcontent, "|")>-1:
										type_tokens = map(string.strip, string.split(attcontent, "|"))
									else:
										type_tokens = [attcontent]
									
									#get dim information from first type selection, extract and anaylse dimstring
									#assumption -> '...' stands for arbitrary values -> adapt it for NXelem_type dims field
									dimstring = type_tokens[0]
									if string.find(type_tokens[0], "[")>-1: 
										dimstring = type_tokens[0][string.find(type_tokens[0], "[")+1:string.find(type_tokens[0], "]")]
										if string.find(dimstring, ",")>-1:
											tdims = map(string.strip, string.split(dimstring, ","))
											for i in range(len(tdims)):
												if tdims[i]==":":
													elemdims.append(-1)
												elif tdims[i] != "":
													if tdims[i][0] >= "0" and tdims[i][0] <="9":
														elemdims.append(string.atoi(tdims[i]))
													elif tdims[i] =="..." or tdims[i]==":...":	
														elemdims = [-2]
													else:
														elemdims.append(tdims[i])	
												elif tdims[i] == "":		
													elemdims.append(-1)
										else:
											elemdims = [-1] 
									else:
										elemdims = None
											
									if string.find(dimstring, "...")>-1:
										elemdims = ["..."]
									
									if dimstring ==":":	
										elemdims = [-1]
											
									
									#get types out of the strings
									for i in range(len(type_tokens)):
										tcontent = type_tokens[i]
										if string.find(tcontent, "[")>-1: 
											cutstring = tcontent[string.find(tcontent, "["):string.find(tcontent, "]")+1]
											if cutstring != "":
												typestring = string.replace(tcontent, cutstring, "")
											elemtypes.append(self.StringToNXType(self.correctTypeString(typestring)))
										else:
											elemtypes.append(self.StringToNXType(self.correctTypeString(tcontent)))
										
										
									if dimstring == "":
										if NX_CHAR in elemtypes:
											elemdims=[-1]
										else:
											elemdims=[1]
										
									if elemdims == None:
										if NX_CHAR in elemtypes:
											elemdims=[-1]
										else:
											elemdims=[1]
										
									continue	
									
								else: # no 'type' att -> normal attribute
									
									print "got a NONEtype attribute .......................: ", att.name
									if attrs == None:
										attrs = {}
									info =""
									desc =""
									if string.find(attcontent, "{")>-1:
										info = attcontent[string.find(attcontent, "{")+1:string.find(attcontent, "}")]
										info = string.replace(info, "'", "\\'")
										desc = attcontent[string.rfind(attcontent, "{")+1:string.rfind(attcontent, "}")]
										desc = string.replace(desc, "'", "\\'")
									if string.find(attcontent, "{")==string.rfind(attcontent, "{"):
										desc = ""
									else:
										info = ""
										desc = ""
									print "info: ", info
									print "desc: ", desc
									#strip description and info
									attcontent = att.content
									if info != "":
										attcontent = string.replace(attcontent, info, "")
									if desc != "":
										attcontent = string.replace(attcontent, desc, "")
									attcontent = string.replace(attcontent, "{}", "")	
									#extract specifier 
									minOccur=1
									if len(attcontent)>0: 
										if attcontent[len(attcontent)-1] == "?":
											attcontent = attcontent[:len(attcontent)-2]
											minOccur=0
										#extract enums/default values
										print "attcontent before mapping: ", attcontent
										if string.find(attcontent, "|")>-1:
											enums = map(string.strip, string.split(attcontent, "|"))
										else:
											enums = [attcontent]
									else:				
										enums = None
									if enums == [""] or enums == "":
										enums = None
										
									attr = NXattr_type(name=att.name, info=info, desc=desc , minOccur=minOccur, enums=enums)
									attrs[att.name] = attr
						
						#if no type attr is specified -> NX_CHAR[] is assumed
						found = 0
						for att in item.properties:		
							if att.type == "attribute":
								if att.name == "type":
									found = 1
						if found == 0:		
							elemtypes = [NX_CHAR]
							elemdims = [-1]
						else:
							if att.content == "ISO8601":
								elemtypes = [NX_CHAR]
								elemdims = [-1]
							
					else: #item.properties == None
						elemtypes = [NX_CHAR]
						elemdims = [-1]
						
						
					elemcontent = item.children.listGetString(item.doc, 1)
					if	self.metagroup.elems == None:
						self.metagroup.elems = {}
					
					if string.find(elemcontent, "{")>-1:
						info = elemcontent[string.find(elemcontent, "{")+1:string.find(elemcontent, "}")]
						desc = elemcontent[string.rfind(elemcontent, "{")+1:string.rfind(elemcontent, "}")]
						info = string.replace(info, "'", "\\'")
						desc = string.replace(desc, "'", "\\'")
						info = string.replace(info, "\n", " ")
						desc = string.replace(desc, "\n", " ")
						if string.find(elemcontent, "{")==string.rfind(elemcontent, "{"):
							desc = ""
					else:
						info = ""
						desc = ""
					print "info: ", info
					print "desc: ", desc
					#strip description and info
					if info != "":
						elemcontent = string.replace(elemcontent, info, "")
					if desc != "":
						elemcontent = string.replace(elemcontent, desc, "")

					elemcontent = string.replace(elemcontent, "{}", "")	
					#extract specifier 
					minOccur=1
					if len(elemcontent)>0:
						if elemcontent[len(elemcontent)-1] == "?" or elemcontent[len(elemcontent)-1] == "*":
							elemcontent = elemcontent[:(len(elemcontent)-1)]
							minOccur=0
						#extract enums/default values
						if string.find(elemcontent, "|")>-1:
							enums = map(string.strip, string.split(elemcontent, "|"))
						else:
							enums = [elemcontent]
					else:
						enums = None

					if enums == [""] or enums=="":
						enums = None
						
					print "types for elem ", item.name,"= " , elemtypes	
					print "dims for elem ", item.name,"= " , elemdims	
					elem = NXelem_type(name=item.name, types=elemtypes, dims=elemdims, info=info, desc=desc, minOccur=minOccur, enums=enums, attrs=attrs)
					
					self.metagroup.elems[item.name] = elem
			item = item.next 	

	
	
		
	def correctTypeString(self, typestring):
		if typestring == "NX_FLOAT":
			return "NX_FLOAT32"
		elif typestring == "NX_UINT":
			return "NX_UINT32"
		elif typestring=="NX_INT":
			return "NX_INT32"
		elif typestring=="NX_BOOLEAN":
			return "NX_UINT8"
		elif typestring=="ISO8601":
			return "NX_CHAR"
		elif typestring=="NXlog":
			return "NX_CHAR"
		elif typestring=="NXgeometry":
			return "NX_CHAR"
		elif typestring=="NXnote":
			return "NX_CHAR"
		elif typestring=="NXenvironment":
			return "NX_CHAR"
		elif typestring=="NXdata":
			return "NX_CHAR"
		elif typestring=="NXbeam":
			return "NX_CHAR"
		elif typestring=="NXorientation":
			return "NX_CHAR"
		elif typestring=="NX_BINARY":
			return "NX_CHAR"
		else:	
			return typestring

	
	def buildTypeDict(self):
		self.typedict = {}
		self.typedict["NX_CHAR"] 		= NX_CHAR
		self.typedict["NX_FLOAT32"] 	= NX_FLOAT32
		self.typedict["NX_FLOAT64"] 	= NX_FLOAT64
		self.typedict["NX_UINT8"] 		= NX_UINT8
		self.typedict["NX_INT8"] 		= NX_INT8
		self.typedict["NX_UINT16"] 	= NX_UINT16
		self.typedict["NX_INT16"] 		= NX_INT16
		self.typedict["NX_UINT32"] 	= NX_UINT32
		self.typedict["NX_INT32"] 		= NX_INT32
		
	
	def StringToNXType(self, typestring):
		return self.typedict[typestring]
	

	
	def createNXStruct(self):
		f = file(self.nxstructfile, "w")
		self.writeFileHeader(f)
		f.close()
	
	
	def appendNXStruct(self):
		f = file(self.nxstructfile, "a")
		self.writeAttsToFile(f)
		self.writeElemsToFile(f)
		self.writeGroupsToFile(f)
		self.writeMetaGroupToFile(f)
		f.close()


	def writeFileHeader(self, f):
		f.write("from nexus import *")
		f.write("\n");
		f.write("from nxtemplatecomponents import *")
		f.write("\n");
		f.write("\n");
		f.write("group_types = {}\n")
		f.write("\n");
	
	def writeAttsToFile(self, f):
		if self.metagroup.attrs != None:
			f.write(self.metagroup.nxclass+"_attrs = {}\n")
			for a in self.metagroup.attrs.keys():
				f.write(self.metagroup.nxclass + "_attrs['" + 
							self.metagroup.attrs[a].name+"'] = NXattr_type("+
							"name='"				+ self.metagroup.attrs[a].name +
							"', info = '"		+ self.metagroup.attrs[a].info +
							"', desc = '"		+ self.metagroup.attrs[a].desc +
							"', minOccur = "	+ repr(self.metagroup.attrs[a].minOccur) +
							", enums = "		+ repr(self.metagroup.attrs[a].enums) +
							")\n")
		f.write("\n");
		
		
	def writeElemsToFile(self, f):
		if self.metagroup.elems != None:
			f.write(self.metagroup.nxclass+"_elems = {}\n")
			for a in self.metagroup.elems.keys():
				if self.metagroup.elems[a].attrs != None:
					f.write("\n")
					f.write(self.metagroup.elems[a].name+"_attrs = {}\n")
					for att in self.metagroup.elems[a].attrs.keys():
						f.write(self.metagroup.elems[a].name + "_attrs['" + 
									self.metagroup.elems[a].attrs[att].name+"'] = NXattr_type("+
									"name='"				+ self.metagroup.elems[a].attrs[att].name +
									"', info = '"		+ self.metagroup.elems[a].attrs[att].info +
									"', desc = '"		+ self.metagroup.elems[a].attrs[att].desc +
									"', minOccur = "	+ repr(self.metagroup.elems[a].attrs[att].minOccur))
						if self.metagroup.elems[a].attrs[att].enums != None:				
							f.write(", enums = "		+ repr(self.metagroup.elems[a].attrs[att].enums)) 
						else:
							f.write(", enums = None")			
						f.write(")\n")
					
					
				f.write(self.metagroup.nxclass + "_elems['" + 
							self.metagroup.elems[a].name+"'] = NXelem_type("+
							"name='"				+ self.metagroup.elems[a].name +
							"', types = "		+ repr(self.metagroup.elems[a].types) +
							",  dims = "		+ repr(self.metagroup.elems[a].dims) +
							", info = '"		+ self.metagroup.elems[a].info +
							"', desc = '"		+ self.metagroup.elems[a].desc +
							"', minOccur = "	+ repr(self.metagroup.elems[a].minOccur))
				if self.metagroup.elems[a].enums != None:				
					f.write(", enums = "		+ repr(self.metagroup.elems[a].enums)) 
				else:
					f.write(", enums = None")			
				if self.metagroup.elems[a].attrs != None: 
					f.write(",  attrs = " + self.metagroup.elems[a].name+"_attrs")
				else:
					f.write(",  attrs = None")
				f.write(")\n")
		
		
		
	def writeGroupsToFile(self, f):
		if self.metagroup.groups != None:
			f.write(self.metagroup.nxclass+"_groups = {}\n")
			for a in self.metagroup.groups.keys():
				f.write(self.metagroup.nxclass + "_groups['" + 
							self.metagroup.groups[a].nxclass+"'] = NXgroup_type("+
							"nxclass='"				+ self.metagroup.groups[a].nxclass +
							"', minOccur = "		+ repr(self.metagroup.groups[a].minOccur) +
							",  maxOccur = "		+ repr(self.metagroup.groups[a].maxOccur) + 
							", attrs = None" +
							", elems = None" +
							", groups = None"+ 
							")\n")
			f.write("\n")


	def writeMetaGroupToFile(self,f):
		f.write("group_types['"+self.metagroup.nxclass+"'] = NXgroup_type(nxclass = '"+self.metagroup.nxclass+
												"', minOccur= "+repr(self.metagroup.minOccur)+
												", maxOccur= "+repr(self.metagroup.maxOccur))
		if self.metagroup.attrs != None:
			f.write(", attrs= "+self.metagroup.nxclass+"_attrs")
		else:
			f.write(", attrs= None")
		if self.metagroup.elems != None:	
			f.write(", elems= "+self.metagroup.nxclass+"_elems")
		else:
			f.write(", elems= None")
		if self.metagroup.groups != None:	
			f.write(", groups= "+self.metagroup.nxclass+"_groups")
		else:	
			f.write(", groups=None")
		f.write(")\n")
		f.write("\n")
		f.write("\n")
	
	
	def clearMetaGroup(self):
		""" need to clear all entrys for next scan """
		self.metagroup = NXgroup_type("", 0, 0, None, None, None)


	def createNXClasses(self):
		f = file(self.nxclassfile, "w")
		self.writeNXClassesHeader(f)
		f.close()
	
	def createNXDict(self):
		f = file(self.dictclassfile, "w")
		self.writeNXClassDictHeader(f)
		f.close()
		
	def appendNXClass(self):
		f = file(self.nxclassfile, "a")
		self.writeNXClass(f)
		f.close()
	
	def appendNXDict(self):
		f = file(self.dictclassfile, "a")
		self.writeNXClassDict(f)
		f.close()

	def writeNXClassesHeader(self,f):
		f.write("import types\n")
		f.write("import nxtemplates\n")
		f.write("import nxfile\n")
		f.write("from nexus import *\n")
		f.write("from nxbaseclass import *\n")
		f.write("\n\n")

	def writeNXClassDictHeader(self,f):	
		f.write("from nxclasses import *\n")
		f.write("\n\n\n\n\n")
		f.write("nx_classes={}")
		f.write("\n")


	def writeNXClass(self,f):
		# for NXdata a special handling exists
		if self.metagroup.nxclass == "NXdata":
			return 0
			
		f.write("\n\n\n")
		f.write("class "+self.metagroup.nxclass+"(NXgroupbase):\n")
		f.write("\t\"\"\" auto-generated by metaDTD processor \"\"\"\n\n")
		f.write("\tdef __init__(self, group=None, name=\"\", nxclass=\""+self.metagroup.nxclass+"\", attrs=None, elems=None, groups=None, path=None, parent=None):\n")
		f.write("\t\tNXgroupbase.__init__(self, group=group, name=name, nxclass=nxclass, attrs=attrs, elems=elems, groups=groups, path=path, parent=parent)\n\n")
		if self.metagroup.groups != None:
			for a in self.metagroup.groups.keys():
				f.write("\tdef get"+self.metagroup.groups[a].nxclass+"(self, name):\n")
				f.write("\t\treturn NXgroupbase.getGroup(self, name, \""+self.metagroup.groups[a].nxclass+"\")\n\n")
				f.write("\tdef read"+self.metagroup.groups[a].nxclass+"(self, name, file):\n")
				f.write("\t\tsubgroupid = name+\":\"+\""+self.metagroup.groups[a].nxclass+"\"\n")
				f.write("\t\treturn self.readSubGroup(subgroupid, file)\n\n")
			
		if self.metagroup.elems != None:
			for e in self.metagroup.elems.keys():
				f.write("\tdef get_"+self.metagroup.elems[e].name+"(self):\n")
				f.write("\t\treturn self.getElem(\""+self.metagroup.elems[e].name+"\")\n\n")
				f.write("\tdef set_"+self.metagroup.elems[e].name+"(self, elem):\n")
				f.write("\t\treturn self.setElem(elem)\n\n")
		
		if self.metagroup.attrs != None:
			for a in self.metagroup.attrs.keys():
				f.write("\tdef getattr_"+self.metagroup.attrs[a].name+"(self):\n")
				f.write("\t\treturn self.getAttr(\""+self.metagroup.attrs[a].name+"\")\n\n")
				f.write("\tdef setattr_"+self.metagroup.attrs[a].name+"(self, elem):\n")
				f.write("\t\treturn self.setAttr(elem)\n\n")
				
			
	def writeNXClassDict(self,f):
		f.write("nx_classes['"+self.metagroup.nxclass+"']="+ self.metagroup.nxclass +"\n")

	def setFiles(self, files):
		self.nxfiles = files
		
	def checkXML(self, filename): 
		return filename.endswith(".xml")

	def fileExists(self, filename):
		try:
			f = file(filename, 'r')
		except: 
			return 0
		return 1
		
		
def main(args):
	""" the entry point for testig purposes"""
	
	nxf = NXfactory(	nxstructfile="nxtemplates.py", 
							nxclassfile="nxclasses.py",
							dictclassfile="nxobjects.py")
	nxf.createNXStruct()
	nxf.createNXClasses()

			
	files = os.listdir("./metaDTD")
	metaDTDs = filter(nxf.checkXML, files)
	nxf.setFiles(metaDTDs)

	for metafile in metaDTDs:
		nxf.parseMetaDTD("./metaDTD/"+metafile)
		nxf.appendNXStruct()
		nxf.appendNXClass()
		nxf.clearMetaGroup()

	nxf.createNXDict()
	for metafile in metaDTDs:
		nxf.parseMetaDTD("./metaDTD/"+metafile)
		nxf.appendNXDict()
		nxf.clearMetaGroup()

if __name__=="__main__":
	main(sys.argv)
	
