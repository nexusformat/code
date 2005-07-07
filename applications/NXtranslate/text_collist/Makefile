CPP=g++
CFLAGS=-c -g -fPIC

######################################################################
# DO NOT EDIT BELOW THIS POINT
######################################################################
ALL=collist_retriever.o
all:$(ALL) # do not edit

OBJS=collist_retriever.o
CPP_SRC=collist_retriever.cpp
H_SRC=collist_retriever.h
######################################################################
all: collist_retriever.o

collist_retriever.o: collist_retriever.cpp collist_retriever.h ../retriever.h
	$(CPP) $(CFLAGS) -Wall collist_retriever.cpp
clean:
	rm -f $(OBJS)

dist-clean: clean

all-clean: clean

install: 
