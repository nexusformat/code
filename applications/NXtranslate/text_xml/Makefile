CPP=g++
CFLAGS=`xml2-config --cflags` -c -g -fPIC

######################################################################
# DO NOT EDIT BELOW THIS POINT
######################################################################
ALL=retriever.o
all:$(ALL) # do not edit

OBJS=retriever.o
CPP_SRC=retriever.cpp
H_SRC=retriever.h
######################################################################
all: retriever.o

retriever.o: retriever.cpp retriever.h
	$(CPP) $(CFLAGS) -Wall retriever.cpp
clean:
	rm -f $(OBJS)

dist-clean: clean

all-clean: clean

