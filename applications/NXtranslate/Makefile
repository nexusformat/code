CPP=g++
ALL=nxtranslate NXtranslate.pdf # do not edit
all:$(ALL) # do not edit

# search for include
INC_FLAGS=-IIPNS_CPP
#search for libraries
LIB_FLAGS=-LIPNS_CPP

# NeXus specific stuff
NEXUS_LIBS=-lNeXus -lmfhdf -ldf -ljpeg -lhdf5
NEXUS_CFLAGS=

# XMLLIB2 specific stuff
XML_CFLAGS=`xml2-config --cflags`
XML_LIBS=`xml2-config --libs`

# select output types options are "-DHDF4" and "-DHDF5"
OUTPUT_CFLAGS=-DHDF4 -DHDF5

# select input types
include IPNS_CPP/Makefile.nxtranslate # IPNS runfile support
include text_plain/Makefile.nxtranslate # text/plain support

# installation directory
INSTALL_DIR=/usr/local/bin
######################################################################
# DO NOT EDIT BELOW THIS POINT
######################################################################
INPUT_CFLAGS=$(IPNS_CPP_CFLAGS) $(TEXT_PLAIN_CFLAGS)
INPUT_OBJS=$(IPNS_CPP_OBJS) $(TEXT_PLAIN_OBJS)
INPUT_LIBS=$(IPNS_CPP_LIBS) $(TEXT_PLAIN_LIBS)
CFLAGS=-c -g -fPIC
LIBS=$(LIB_FLAGS) $(XML_LIBS) $(NEXUS_LIBS) $(INPUT_LIBS)

include docs/Makefile.nxtranslate # add the documentation

#ALL=nxtranslate NXtranslate.pdf
OBJS=attr.o main.o nexus_retriever.o nexus_util.o node.o node_util.o \
    retriever.o string_util.o xml_parser.o $(INPUT_OBJS)
CPP_SRC=attr.cpp main.cpp nexus_retriever.cpp  nexus_util.cpp node.cpp \
    node_util.cpp retriever.cpp string_util.cpp xml_parser.cpp
H_SRC=attr.h nexus_retriever.h nexus_util.h node.h node_util.h Ptr.h \
    retriever.h string_util.h xml_parser.h tree.hh nxtranslate_debug.h
TEST_SRC=test_simple.xml test_nexus.xml test_nexus_macro.xml test_ipns.xml \
    hrcs1797.run test_text_plain.xml
TEST_RESULT=test_simple.nxs test_nexus.nxs test_nexus_macro.nxs \
    $(IPNS_CPP_TEST) $(TEXT_PLAIN_TEST)
SRC=$(CPP_SRC) $(H_SRC) $(TEST_SRC) $(DOC_SRC) $(IPNS_CPP_SRC) Makefile \
    NXtranslate.pdf README LICENSE BUGS CHANGES $(TEXT_PLAIN_SRC)
VERS=0.2.0-rc4
######################################################################
nxtranslate: $(OBJS) $(INPUT_LIBS)
	cd IPNS_CPP && $(MAKE)
	$(CPP) $(OBJS) $(LIBS) -o nxtranslate

attr.o: attr.cpp attr.h
	$(CPP) $(NEXUS_CFLAGS) $(CFLAGS) attr.cpp

main.o: main.cpp xml_parser.h string_util.h
	$(CPP) $(NEXUS_CFLAGS) $(CFLAGS) $(OUTPUT_CFLAGS) main.cpp

nexus_retriever.o: nexus_retriever.cpp nexus_retriever.h retriever.h node.h tree.hh nexus_util.h string_util.h
	$(CPP) $(NEXUS_CFLAGS) $(CFLAGS) nexus_retriever.cpp

nexus_util.o: nexus_util.cpp nexus_util.h node.h string_util.h
	$(CPP) $(NEXUS_CFLAGS) $(CFLAGS) nexus_util.cpp

node.o: node.cpp attr.h nexus_util.h node.h string_util.h
	$(CPP) $(NEXUS_CFLAGS) $(CFLAGS) node.cpp

node_util.o: node_util.cpp node_util.h
	$(CPP) $(CFLAGS) node_util.cpp

retriever.o: retriever.cpp nexus_retriever.h retriever.h Ptr.h
	$(CPP) $(INC_FLAGS) $(NEXUS_CFLAGS) $(CFLAGS) $(INPUT_CFLAGS) retriever.cpp

string_util.o: string_util.cpp string_util.h
	$(CPP) $(CFLAGS) string_util.cpp

xml_parser.o: xml_parser.cpp node.h xml_parser.h nexus_util.h node_util.cpp node_util.h string_util.h retriever.h Ptr.h
	$(CPP) $(XML_CFLAGS) $(NEXUS_CFLAGS) $(CFLAGS) xml_parser.cpp

install: nxtranslate
	cp nxtranslate $(INSTALL_DIR)

######################################################################
# Testing stuff

test_simple.nxs: nxtranslate test_simple.xml
	./nxtranslate test_simple.xml

test_nexus.nxs: nxtranslate test_nexus.xml
	./nxtranslate test_nexus.xml

test_nexus_macro.nxs: nxtranslate test_nexus_macro.xml
	./nxtranslate test_nexus_macro.xml -DFILE=test_simple.nxs

test_ipns.nxs: nxtranslate test_ipns.xml hrcs1797.run
	./nxtranslate test_ipns.xml -DfileName=hrcs1797.run

test_text_plain.nxs: nxtranslate test_text_plain.xml test_simple.xml
	./nxtranslate test_text_plain.xml

test-clean:
	rm -f $(TEST_RESULT)

######################################################################
# maintenience targets
# target taken from "Art of Unix Programming chp19sec02" by ESR
NXtranslate-$(VERS).tar.gz: $(SRC)
	@ls $(SRC) | sed s:^:NXtranslate-$(VERS)/: >MANIFEST
	@(cd ..; ln -s NXtranslate NXtranslate-$(VERS))
	(cd ..;tar -czvf NXtranslate/NXtranslate-$(VERS).tar.gz `cat NXtranslate/MANIFEST`)
	@(cd ..; rm NXtranslate-$(VERS))

dist: NXtranslate-$(VERS).tar.gz

test: nxtranslate $(TEST_RESULT)

dist-test: test
	./nxtranslate test_path_error.xml

clean: test-clean
	rm -f $(OBJS)
	cd IPNS_CPP && make clean

dist-clean: clean

all-clean: clean
	rm $(ALL)
