#====================================================================
# Copyright (c) 2004, P.F.Peterson <petersonpf@ornl.gov>
#               Spallation Neutron Source at Oak Ridge National Laboratory
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#====================================================================
CPP=g++
C_FLAGS=-c -g -fPIC
INC_FLAGS=-I/usr/local/include
LD_FLAGS=-lNeXus -lmfhdf -ldf -ljpeg -lhdf5
######################################################################
# do not edit below this point
ALL=nxdir
OBJ=main.o string_util.o tree.o data.o data_writer.o
SRC=main.cpp string_util.cpp tree.cpp data.cpp nxdir.h nxdir_help.h \
    data_writer.cpp Makefile CHANGES LICENSE README TODO
VERS=0.2.5
######################################################################

all: $(ALL)

main.o: main.cpp nxdir.h nxdir_help.h
	$(CPP) $(C_FLAGS) $(INC_FLAGS) main.cpp

data.o: data.cpp nxdir.h
	$(CPP) $(C_FLAGS) $(INC_FLAGS) data.cpp

string_util.o: string_util.cpp nxdir.h
	$(CPP) $(C_FLAGS) $(INC_FLAGS) string_util.cpp

tree.o: tree.cpp nxdir.h
	$(CPP) $(C_FLAGS) $(INC_FLAGS) tree.cpp

nxdir: $(OBJ)
	$(CPP) $(OBJ) $(LD_FLAGS) -o nxdir

# target taken from "Art of Unix Programming chp19sec02" by ESR
NXdir-$(VERS).tar.gz: $(SRC)
	@ls $(SRC) | sed s:^:NXdir-$(VERS)/: >MANIFEST
	@(cd ..; ln -s NXdir NXdir-$(VERS))
	(cd ..;tar -czvf NXdir/NXdir-$(VERS).tar.gz `cat NXdir/MANIFEST`)
	@(cd ..; rm NXdir-$(VERS))

######################################################################
test:

clean:
	rm *.o

clean_out: clean
	rm test*.nxs

distclean: clean_out
	rm $(ALL)
