#!/usr/bin/python

import sys

from nxfile import *

	

def main(args):
	""" the entry point for testing purposes """
	
	#----------------------------------------------------------
	# EXAMPLE 1:  BUILD GROUP TREE FROM SCRATCH AND FILL NXDATA
	print "EXAMPLE 1:  BUILD GROUP TREE FROM SCRATCH AND FILL NXDATA"
	print "========================================================="

	#create a new nexus file from scratch
	f = NXfile("nxtest.nxs", NXACC_CREATE5)

	root = f.getRoot()
	
	creator = NXattr(name="creator", nxtype=NX_CHAR , value="hartmut.gilde@frm2.tum.de")
	root.setattr_creator(creator)
	root.setAttr(creator)
	
	#create some nexus groups 
	myentry 	= NXentry(name="entry")
	mysample = NXsample(name="sample")
	mydata 	= NXdata(name="data")
	
	#append subgroups/determine group hirarchy	
	myentry.setGroup(mysample)
	myentry.setGroup(mydata)
	root.setGroup(myentry)
	#create data entry
	#nxcachefile is the name of the cache file to be created, if omitted or empty, no data will be cached
	dataElem = NXdataelem(name="counts", dims=[3,5,10], nxtype=NX_INT32, nxcachefile="")
	
	#append data entry to group
	mydata.setData(dataElem)
	#add some data
	for i in range(dataElem.dims[0]):
		for j in range(dataElem.dims[1]):
			for k in range(dataElem.dims[2]):
				print "writing ",i+j+k, " to position: ",i,j,k
				dataElem.appendData(i+j+k, [i,j])
	
	#write back to nexus file 
	f.save()  # this is equivalent to  f.writeGroup(root)

	#close nexus file
	f.close()
	#clear objects
	myentry = None
	mysample = None
	mydata = None
	print "\n\n"


	#---------------------------------------------------
	#EXAMPLE 2: OPEN EXISTING FILE AND PUT SOME ELEMENTS
	print "EXAMPLE 2: OPEN EXISTING FILE, GET AND PUT SOME ELEMENTS/ATTRIBUTES"
	print "==================================================================="
	#reopen nexus file
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	
	root = f.getRoot()
	nexus_version = root.getattr_NeXus_version()
	print "nexus version: ", nexus_version.getValue()

	file_time = root.getattr_file_time()
	print "file_time:", file_time.getValue()
	
	creator = root.getattr_creator()
	print "creator:", creator.getValue()
	
	# read previously created group
	myentry = f.readGroup("root:NXroot/entry:NXentry")
	
	mydata = myentry.getGroup("data", "NXdata")
	counts = mydata.getElem("counts")
	print "counts:",counts.getData()

	#create a few new elements
	entry_title 		= NXelem(name="title", nxtype=NX_CHAR, data="entry title")
	entry_run_number 	= NXelem(name="run_number", nxtype=NX_INT32, dims=[1], data=2)
	
	#attach elements to entry group
	myentry.set_title(entry_title)
	myentry.set_run_number(entry_run_number)

	#alternatively we could also write
	myentry.setElem(entry_title)
	myentry.setElem(entry_run_number)

	
	#write file
	f.writeGroup(myentry)
	#close file	
	f.close()

	#clear objects
	myentry = None
	entry_title = None
	
	#reopen nexus file
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	entry_title = f.readElem("root:NXroot/entry:NXentry/title:SDS")
	print "title:", entry_title.getData()

	#close nexus file
	f.close()
	print "\n\n"


	#Example 3: OPEN NEXUS FILE AND SUBSEQUENTLY READ SUBGROUPS
	print "OPEN NEXUS FILE AND SUBSEQUENTLY READ SUBGROUPS"
	print "==================================================="
	#reopen nexus file
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	
	root = f.getRoot()
	entry = f.readSubGroup(root, "entry:NXentry")
	entry.setElem (NXelem(name="command_line", nxtype=NX_CHAR, dims=[20], data="/command") )
	
	sample = f.readSubGroup(entry, "sample:NXsample")
	tvalues = [20.0, 21.0, 20.0, 20.0, 21.0, 20.0, 20.0, 21.0, 20.0, 20.0, 21.0, 20.0, 20.0, 21.0, 20.0, 20.0, 21.0, 20.0, 19.0, 18.0 ]
	sample.setElem( NXelem(name="temperature", nxtype=NX_FLOAT32, dims=[20], data=tvalues) )
	
	entry.setGroup(sample)
	root.setGroup(entry)
	f.save()
	f.close()

	#reopen nexus file
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	sample = f.readGroup("root:NXroot/entry:NXentry/sample:NXsample")
	temperature = sample.get_temperature()
	# equivalent to: temperature = sample.getElem("temperature")
	print "temperature:", temperature.getData()
	
	#close nexus file
	f.close()
	print "\n\n"



	#Example 4: OPEN NEXUS FILE AND PLACE SOME ARBITRARY GROUPS/ELEMS/ATTRS/DATA 
	print "OPEN NEXUS FILE AND PLACE SOME ARBITRARY GROUPS/ELEMS/ATTRS/DATA"
	print "================================================================"
	
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	
	user_name = NXelem(name="name", nxtype=NX_CHAR, dims=[20], data="Hartmut Gilde")
	user_role = NXelem(name="role", nxtype=NX_CHAR, dims=[20], data="observer")
	user_mail = NXelem(name="email", nxtype=NX_CHAR, dims=[30], data="hartmut.gilde@frm2.tum.de")
	
	# write single group to specific location in tree
	usr = NXuser(name="hartmut", path="root:NXroot/entry:NXentry/user:NXuser")
	usr.set_name(user_name)
	usr.set_role(user_role)
	usr.set_email(user_mail)

	#write single element to specified location in tree
	density = NXelem(path="root:NXroot/entry:NXentry/sample:NXsample/density:SDS", name="density", nxtype=NX_FLOAT32, dims=[4], data=[0.5, 0.5, 0.5, 0.5])
	f.writeElem(density)
	
	density = None
	f.close()


	# reopen it again
	f = NXfile("nxtest.nxs", NXACC_RDWR)

	density = f.readElem("root:NXroot/entry:NXentry/sample:NXsample/density:SDS")
	print "directly read density: ", density.getData()
	
	#read data from single elem
	cmd_ln = f.readData("root:NXroot/entry:NXentry/command_line:SDS")
	print "directly read command_line: ", cmd_ln
	
	f.close()
	print "\n\n"




	#Example 6: INTERRUPT SCAN PROCESS(WRITE PROCESS TO NXDATA) AND RECOVER FROM SAVED DATA
	print "INTERRUPT SCAN PROCESS(WRITE PROCESS TO NXDATA) AND RECOVER FROM SAVED DATA"
	print "================================================================"
	
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	
	entry = f.readGroup("root:NXroot/entry:NXentry")
	
	tnxdata = NXdata(name="tdata")
	
	tsdata = NXdataelem( name="ncounts", 
								dims=[2,3,20], 
								axnames="[p]", 
								nxcachefile="./nxcache.nxs") 
	tsaxish = NXaxis(name="h", nxtype=NX_FLOAT32, dims=[20]) 
	tsaxisk = NXaxis(name="k", nxtype=NX_FLOAT32, dims=[20]) 
	tsaxisl = NXaxis(name="l", nxtype=NX_FLOAT32, dims=[20]) 
	tsaxish.attachToNXData(tsdata)
	tsaxisk.attachToNXData(tsdata)
	tsaxisl.attachToNXData(tsdata)

	tnxdata.setData(data=tsdata)
	
	entry.setGroup(tnxdata)
	
	status = f.writeGroup(entry)
	
	# loops simulate interruption on scan line 2 and entry 12 
	for j in range(2):
		for i in range(2):
			for k in range(12):
				print "writing", float((j+1)*(i+1)*(k+1)), " to [", j,"," , i, "]"
				tsdata.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
				time.sleep(1)
	for k in range(12):			
		#time.sleep(0.2)
		tsaxish.appendData(2.6)
		tsaxisk.appendData(3.1)
		tsaxisl.appendData(4.4)
		
	f = None
	
	# now try to recover	
	f = NXfile("nxtest.nxs", NXACC_RDWR)
	entry = f.readGroup("root:NXroot/entry:NXentry")
	
	trdata2 = NXdataelem(name="ncounts", 
								dims=[2,3,20], 
								axnames="[p]",
								nxcachefile="./nxcache.nxs")
	trdata2.restoreData()
	print "restored data: ", trdata2.data
	traxish = NXaxis(name="h", nxtype=NX_FLOAT32, dims=[20]) 
	traxisk = NXaxis(name="k", nxtype=NX_FLOAT32, dims=[20]) 
	traxisl = NXaxis(name="l", nxtype=NX_FLOAT32, dims=[20])
	traxish.attachToNXData(trdata2)
	traxisk.attachToNXData(trdata2)
	traxisl.attachToNXData(trdata2)
	traxish.restoreData()
	traxisk.restoreData()
	traxisl.restoreData()
	
	print "restored axis h:", traxish.data
	print "restored axis k:", traxisk.data
	print "restored axis l:", traxisl.data

	for j in range(1,2):
		for i in range(2,3):
			for k in range(10,20):
				print "writing", float((j+1)*(i+1)*(k+1)), " to [", j,"," , i, "]"
				time.sleep(0.1)
				trdata2.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
	for k in range(0,4):			
		traxish.appendData(2.6)
		traxisk.appendData(3.1)
		traxisl.appendData(4.4)

	f.close()
	print "\n\n"
	


if __name__=="__main__":
	main(sys.argv)
	

