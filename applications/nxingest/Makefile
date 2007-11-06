SHEEL = /bin/sh 
#########################################################
#                                                       #
#      Makefile for the following executable:           #
#                                                       #
EXECUTABLE = nxingest
VERSION = 1.9
#########################################################
SOURCES = nxingest_main.cpp nxingest_utils.cpp nxingest_debug.cpp nxingest_nexus.cpp nxingest_parse.cpp nxingest_time.cpp
OBJECTS = nxingest_main.o   nxingest_utils.o   nxingest_debug.o   nxingest_nexus.o   nxingest_parse.o   nxingest_time.o
TEST_DATA_DIR = test_data

# #####  Flags ##### #
CPP = g++ 
CFLAGS = -g 
INCFLAG = -I. -I/usr/include -I/usr/local/include
LIBFLAG = -L. -L/usr/lib -L/usr/local/lib -lNeXus -lmxml -lxml2 -lhdf5 -lmfhdf -ldf -lsz -ljpeg -lz -lpthread -liconv -lm 
ALL_CFLAGS = $(CFLAGS) $(INCFLAG)
.SUFFIXES:
.SUFFIXES: .cpp .o

#  #####  Compilation of the executable ##### #
all : $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(CFLAGS) $(LIBFLAG) $(OBJECTS) -o $(EXECUTABLE)
	
.cpp.o:
	$(CPP) -c $(ALL_CFLAGS) $<
	
# #####  Unit Test ##### # 
test:	$(EXECUTABLE)
	 $(TEST_DATA_DIR)/test-script
	
# ##### Cleaning ##### #
clean : 
	rm -f *.o nxingest  $(TEST_DATA_DIR)/*.h5 *.gz $(TEST_DATA_DIR)/results/*

# ##### Installation  ##### #
install: all
	@echo  No installation script. Free standing executable.
	
# ##### Tar and Compress ##### #
tar : clean
	tar cvfz $(EXECUTABLE)-$(VERSION).tar.gz Makefile $(SOURCES) *.h *.txt $(TEST_DATA_DIR)/test-script $(TEST_DATA_DIR)/mapping/*.xml $(TEST_DATA_DIR)/Muon/*.nxs $(TEST_DATA_DIR)/Gemini/*GD.nxs $(TEST_DATA_DIR)/results
	# need the gnu tar to compress.at the same time than making the tar file
