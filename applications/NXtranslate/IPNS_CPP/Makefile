#CFLAGS += -g -Wno-deprecated -DIPNS_LITTLE_ENDIAN -Wno-deprecated
CFLAGS += -c -g -DIPNS_LITTLE_ENDIAN  -Wno-deprecated
DFLAGS += -g -L./
all: $(OBJS) testProg testProg2 libIPNSRunfile.a

OBJS += Header.o RandomAccessRunfile.o LpsdDetIdMap.o DetectorMap.o Segment.o Runfile.o TimeField.o
INCLUDE += Header.h RandomAccessRunfile.h LpsdDetIdMap.h DetectorMap.h Segment.h Runfile.h TimeField.h

clean:
	rm -f $(OBJS) testProg testProg2 libIPNSRunfile.a

libIPNSRunfile.a: $(OBJS) $(INCLUDE)
	ar rcs libIPNSRunfile.a $(OBJS)

RandomAccessRunfile.o: RandomAccessRunfile.cpp RandomAccessRunfile.h
	g++ $(CFLAGS) RandomAccessRunfile.cpp

Header.o: Header.cpp Header.h RandomAccessRunfile.h
	g++ $(CFLAGS) Header.cpp

LpsdDetIdMap.o: LpsdDetIdMap.cpp LpsdDetIdMap.h RandomAccessRunfile.h
	g++ $(CFLAGS) LpsdDetIdMap.cpp

DetectorMap.o: DetectorMap.cpp DetectorMap.h Header.h RandomAccessRunfile.h
	g++ $(CFLAGS) DetectorMap.cpp

TimeField.o: TimeField.cpp TimeField.h Header.h RandomAccessRunfile.h
	g++ $(CFLAGS) TimeField.cpp

Segment.o: Segment.cpp Segment.h
	g++ $(CFLAGS) Segment.cpp

Runfile.o: Runfile.cpp Runfile.h TimeField.h DetectorMap.h Header.h RandomAccessRunfile.h
	g++ $(CFLAGS) Runfile.cpp

# how to build the retriever
ipns_retriever.o: ipns_retriever.cpp ipns_retriever.h libIPNSRunfile.a
	g++ $(CFLAGS) ipns_retriever.cpp

testProg: testProg.cpp Header.o RandomAccessRunfile.o LpsdDetIdMap.o
	g++ $(DFLAGS) -o testProg Header.o RandomAccessRunfile.o LpsdDetIdMap.o testProg.cpp

testProg2: testProg2.cpp libIPNSRunfile.a
	g++ -o testProg2 testProg2.cpp $(DFLAGS) -lIPNSRunfile 
