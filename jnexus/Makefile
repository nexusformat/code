#--------------------------------------------------------------------------
# Makefile for the NeXus Java API. Builds both the required Java class
# files and the shared library with the native methods.
# 
# Mark Koennecke, October 2000
#--------------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .java .class .c .o 

###########################################################################
#         C O N F I G U R A T I O N       S E C T I O N                   #
###########################################################################

#--------------------------------------------------------------------------
#           DigitalUnix 4.0D
# Usage: make du40
#--------------------------------------------------------------------------
# Where to find the HDF libraries: HDF4.1r3 is REQUIRED
du40:= HDFROOT=/data/koenneck

#Where to find the napi C stuff
du40:= NAPIROOT=../

# Where the Java binaries live
du40:= JAVABIN=/usr/bin
header:= JAVABIN=/usr/bin

# The classpath for compiling java
du40:= JCP=/usr/lib/classes.zip:.:

# The path to the include files for the jni-headers
du40:= JINCLUDE= -I/usr/include/java -I/usr/include/java/alpha

# The C-compiler to use
du40:= CC=cc

# Compiler flags to use for compiling
du40:= CFLAGS= -g -c -pthread $(JINCLUDE) -I$(HDFROOT)/include -I$(NAPIROOT) \
       -Inative

#Compiler flags used for linking
du40:= LFLAGS= -g -shared -L$(HDFROOT)/lib -L$(NAPIROOT)

#Target location for the shared library
du40:= SHLIB=bin/du40/libjnexus.so

###########################################################################
#     END OF C O N F I G U R A T I O N       S E C T I O N                #
#                  DO NOT TOUCH ANYTHING BELOW!                           #
###########################################################################

.java.class:
	$(JAVABIN)/javac -g -classpath $(JCP) $*.java

.c.o:
	$(CC) $(CFLAGS) -o $*.o $*.c

JOBJ=ncsa/hdf/hdflib/HDFException.class \
     ncsa/hdf/hdflib/HDFJavaException.class \
     ncsa/hdf/hdflib/HDFNotImplementedException.class \
     ncsa/hdf/hdflib/HDFConstants.class \
     ncsa/hdf/hdflib/HDFArray.class \
     ncsa/hdf/hdflib/HDFNativeData.class \
     neutron/nexus/NexusException.class \
     neutron/nexus/NXlink.class \
     neutron/nexus/NeXusFileInterface.class \
     neutron/nexus/AttributeEntry.class \
     neutron/nexus/NexusFile.class 
     

COBJ=native/hdfnativeImp.o \
     native/hdfexceptionImp.o \
     native/handle.o \
     native/NexusFile.o

du40: all
rh62: all


all: jnexus.jar libjnexus.so test

jnexus.jar: $(JOBJ)
	- rm jnexus.jar
	$(JAVABIN)/jar cvf jnexus.jar $(JOBJ) \
	  ncsa/hdf/hdflib/ArrayDescriptor.class

libjnexus.so: $(COBJ)
	$(CC) $(LFLAGS) -o $(SHLIB) $(COBJ) -lNeXus \
	  -lmfhdf -ldf \
	  $(HDFROOT)/lib/libjpeg.a -lz -lm 


header: neutron/nexus/NexusFile.class
	$(JAVABIN)/javah -jni -d native neutron.nexus.NexusFile

test: test/TestJapi.class

clean:
	- rm neutron/nexus/*.class
	- rm ncsa/hdf/hdflib/*.class
	- rm native/*.o


# Make distributions
ALLDIS=apidoc test jnexus.jar *.html neutron/nexus/*.java \
	ncsa/hdf/hdflib/*.java README.* COPY*.* NeXus.gif 

disdu40: $(ALLDIS)
	- rm jnexusdu40.tar.gz
	tar cvf jnexusdu40.tar $(ALLDIS)
	cp bin/du40/libjnexus.so .
	tar uvf jnexusdu40.tar libjnexus.so
	gzip jnexusdu40.tar
	- rm *.so
 
distux: $(ALLDIS)
	- rm jnexustux.tar.gz
	tar cvf jnexustux.tar $(ALLDIS)
	cp bin/rh62/libjnexus.so .
	tar uvf jnexustux.tar libjnexus.so
	gzip jnexustux.tar
	- rm *.so

ZIP=/data/koenneck/bin/zip
disw32: $(ALLDIS)
	- rm jnexusw32.zip
	$(ZIP) -r jnexusw32.zip $(ALLDIS)
	cp jnexus/bin/win32/*.dll .
	cp bin/win32/*.dll .
	$(ZIP) -g jnexusw32.zip *.dll
	- rm *.dll

WWW:
	- rm www.tar
	cp index.html japinotes.html
	tar cvf www.tar apidoc test/TestJapi.java \
	japinotes.html jnexustut.html *.zip *.tar.gz COPYING.NCSA

