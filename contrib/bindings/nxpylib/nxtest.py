#!/usr/bin/python

import sys
import types
import string
import os
from time import *

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
	#nxcachefile is the name of the cache file to be created, if omitted, no data will be cached
	dataElem = NXdataelem(name="counts", dims=[3,5,10], nxtype=NX_INT32, nxcachefile="nxcache1.nxs")
	
	#add some data
	for i in range(dataElem.dims[0]):
		for j in range(dataElem.dims[1]):
			for k in range(dataElem.dims[2]):
				print "writing ",i+j, " to position: ",i,j,k
				dataElem.appendData(i+j+k, [i,j])
	
	#append data entry to group
	mydata.setData(dataElem)
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
	print "EXAMPLE 2: OPEN EXISTING FILE AND PUT SOME ELEMENTS"
	print "==================================================="
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
	print counts.getData()

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
	entry = root.readSubGroup("entry:NXentry", f)
	entry.setElem (NXelem(name="command_line", nxtype=NX_CHAR, dims=[20], data="/command") )
	
	sample = entry.readSubGroup("sample:NXsample", f)
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
	
	# to be done ...	
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
								axcachefile="axcache.nxt", 
								cachefile="./cache.nxt", 
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
				#sleep(0.0)
				tsdata.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
	for k in range(12):			
		#sleep(0.2)
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
								axcachefile="axcache.nxt", 
								cachefile="./cache.nxt", 
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
				sleep(0.1)
				trdata2.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
	for k in range(0,4):			
		traxish.appendData(2.6)
		traxisk.appendData(3.1)
		traxisl.appendData(4.4)

	f.close()
	print "\n\n"
	
	return 0






































	#samplepath = file.pathStringToList("root:NXroot/centry:NXentry/mysample:NXsample")
	#f.writeGroup(NXsample(name="mysample", path=samplepath), NXfile.FLAT)

	file = NXfile("NXtest3.nx5", NXACC_RDWR)
	new_entry = file.readGroup("root:NXroot/MyEntry:NXentry", NXfile.DEEP)
	print "groups:-----------------------",new_entry.group.groups
	smp = NXsample(name="nsample")
	new_entry.setGroup(smp)
	status = file.writeGroup(new_entry, NXfile.DEEP)	
	print "groups:-----------------------",new_entry.group.groups

	
	mysample = file.readGroup("root:NXroot/MyEntry:NXentry/MySample:NXsample", NXfile.FLAT)
	#print dir(mysample)	
	#temp = mysample.get_temperature().getData()
	#print "temp: ", temp
	uc = mysample.get_unit_cell()	
	uc2 = mysample.getElem("unit_cell")
	print "unit cell: ", uc, uc2
	
	s_path_l = NXelem(name="path_length", nxtype=NX_FLOAT32, dims=[1], data=2.3)
	newelem = NXelem(path=file.pathStringToList("root:NXroot/MyEntry:NXentry"), name="program_name", nxtype=NX_CHAR, dims=[10], data="nexus_main")
		
	status = file.writeElem(newelem)
	print "writeElem with unknown elem returned: ", status

	newelem.setData("klara schuhmann")
	status = file.writeData(newelem)
	
	#newelem.setAttrValue("version", "2.3.4.5.5")
	#status = file.writeElem(newelem)
	#print "writeElem with unknown elem returned: ", status
	
	#mysample.setElem(s_path_l)
	#status = file.writeGroup(mysample, NXfile.FLAT)
	
	myentry = file.readGroup("root:NXroot/MyEntry:NXentry", NXfile.FLAT)
	print "myentry: class: %s, name: %s" % (myentry.group.nxclass, myentry.group.name)
	status = file.writeGroup(myentry, NXfile.FLAT)
	
	#appsample = myentry.readSubGroup("MySample:NXsample", file)

	#s_path_l = NXelem(name="path_length", nxtype=NX_FLOAT32, dims=[1], data=2.3)
	#appsample.setElem(s_path_l)
	#status = file.writeGroup(appsample, NXfile.FLAT)

	#mytemp = file.readElem("root:NXroot/MyEntry:NXentry/MySample:NXsample/temperature:SDS")
	#print "mytemp: ", mytemp.getData()
	
	#mydata = file.readData("root:NXroot/MyEntry:NXentry/MySample:NXsample/temperature:SDS")
	#print "data: ", mydata

	#myatt = file.readAttr("root:NXroot/MyEntry:NXentry/definition:SDS/version")
	#print "att: ", myatt.getValue()

	#runc = myentry.get_run_cycle()
	#runc.setData("was willst du denn hier?")

	
	#dur = NXelem(name="duration", nxtype=NX_INT32, dims=[1], data=66)
	#myentry.setElem(dur)
	
	#cmln = NXelem(name="cromma", nxtype=NX_CHAR, dims=[255], data="no special command")
	#myentry.setElem(cmln)
	
	#status = file.writeGroup(myentry, NXfile.FLAT)

	user_name = NXelem(name="name", nxtype=NX_CHAR, dims=[20], data="Hartmut Gilde")
	user_role = NXelem(name="role", nxtype=NX_CHAR, dims=[20], data="observer")
	mail_path = file.pathStringToList("root:NXroot/MyEntry:NXentry/hartmut:NXuser")
	user_mail = NXelem(path=mail_path, name="email", nxtype=NX_CHAR, dims=[20], data="hgilde@web.de")
	
	usr = NXuser(name="hartmut")
	usr.set_name(user_name)
	usr.set_role(user_role)
	myentry.setGroup(usr)
	status = file.writeGroup(myentry, NXfile.DEEP)	
	print "writeGroup myentry  returned: ", status
	
	print "construct data"
	mydata = NXdataelem(name="ncounts", 
								dims=[2,3,20], 
								axnames="[p]", 
								cachefile="./cache1.bkp", 
								nxcachefile="./nxcache1.nxs") 
	myaxish = NXaxis(name="h", nxtype=NX_FLOAT32, dims=[20]) 
	myaxisk = NXaxis(name="k", nxtype=NX_FLOAT32, dims=[20]) 
	myaxisl = NXaxis(name="l", nxtype=NX_FLOAT32, dims=[20]) 
	print "begin writing"
	
	print mydata.data
	#mydata.restoreData("NXdatamydata_12345678.bck")
	#print mydata.data
	for j in range(2):
		for i in range(3):
			for k in range(20):
				mydata.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
		myaxish.appendData(2.6)
		myaxisk.appendData(3.1)
		myaxisl.appendData(4.4)
		
	print mydata.data

	nxdata = NXdata(name="ntest_data2")
	nxdata.setData(data=mydata)
	nxdata.setAxes(axlist=[myaxish, myaxisk, myaxisl])
	myentry.setGroup(nxdata)

	write = "ye"
	read = "yes"	

	if write =="yes":
		print "new try .........."
		tsdata = NXdataelem( name="ncounts", 
									dims=[2,3,20], 
									axnames="[p]", 
									axcachefile="axcache.nxt", 
									cachefile="./cache.nxt", 
									nxcachefile="./nxcache.nxs") 
		tsaxish = NXaxis(name="h", nxtype=NX_FLOAT32, dims=[20]) 
		tsaxisk = NXaxis(name="k", nxtype=NX_FLOAT32, dims=[20]) 
		tsaxisl = NXaxis(name="l", nxtype=NX_FLOAT32, dims=[20]) 
		tsaxish.attachToNXData(tsdata)
		tsaxisk.attachToNXData(tsdata)
		tsaxisl.attachToNXData(tsdata)

		for j in range(2):
			for i in range(3):
				for k in range(20):
					print "writing", float((j+1)*(i+1)*(k+1)), " to [", j,"," , i, "]"
					sleep(0.0)
					tsdata.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
		print "ready 2" 			
		for k in range(20):			
			sleep(0.2)
			tsaxish.appendData(2.6)
			tsaxisk.appendData(3.1)
			tsaxisl.appendData(4.4)
			
		print "ready 3" 			

		tnxdata = NXdata(name="tdata")
		tnxdata.setData(data=tsdata)
		myentry.setGroup(tnxdata)
		status = file.writeGroup(myentry, NXfile.DEEP)
	
	if read == "yes":
		trdata2 = NXdataelem(name="ncounts", 
									dims=[2,3,20], 
									axnames="[p]",
									axcachefile="axcache.nxt", 
									cachefile="./cache.nxt", 
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
		
		print "axis:", traxish.data, traxish.gridinfo
		print "axis:", traxisk.data, traxisk.gridinfo
		print "axis:", traxisl.data, traxisl.gridinfo
	
		for j in range(1,2):
			for i in range(2,3):
				for k in range(10,20):
					print "writing", float((j+1)*(i+1)*(k+1)), " to [", j,"," , i, "]"
					sleep(0.1)
					trdata2.appendData(float((j+1)*(i+1)*(k+1)), [j,i])
		for k in range(0,4):			
			traxish.appendData(2.6)
			traxisk.appendData(3.1)
			traxisl.appendData(4.4)
			
		tnxdata2 = NXdata(name="counts")
		tnxdata2.setData(data=trdata2)
		myentry.setGroup(tnxdata2)
		status = file.writeGroup(myentry, NXfile.DEEP)


	#file.writeElem(user_mail)
	#myrc = file.readData("root:NXroot/MyEntry:NXentry/run_cycle:SDS")
	#print "directly read run_cycle: ", myrc
	
	#myst = file.readData("root:NXroot/MyEntry:NXentry/start_time:SDS")
	#print "directly read start_time: ", myst
	
	#myst = file.readElem("root:NXroot/MyEntry:NXentry/start_time:SDS")
	#print "directly read start_time: ", myst.getData()
	#print "dims: ", myst.getDims()
	
	#myd = file.readElem("root:NXroot/MyEntry:NXentry/duration:SDS")
	#print "directly read duration: ", myd.getData()
	#print "dims: ", myd.getDims()
	#appsample = myentry.getSample("MySample")
	#print "appsample: ", appsample
	#temp = appsample.get_temperature().getData()
	#print "apptemp: ", temp
	
	#runc = myentry.get_run_cycle().getData()
	#print "run_cycle: ", runc

	

if __name__=="__main__":
	main(sys.argv)
	

