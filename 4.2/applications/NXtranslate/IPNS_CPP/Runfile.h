#ifndef IPNS_RUNFILE_H
#define IPNS_RUNFILE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Header.h"
#include "RandomAccessRunfile.h"
#include "LpsdDetIdMap.h"
#include "DetectorMap.h"
#include "TimeField.h"
#include "Segment.h"

using std::ifstream;
//ing namespace std;

struct DiscLevel{
  int lowerLevel;
  int upperLevel;
};

struct DCAL {
  float LENGTH[22];
  float WIDTH[22];
  float DEPTH[22];
  float EFFICIENCY[22];
  int PSD_DIMENSION[22];
  int NUM_OF_SEGS_1[22];
  int NUM_OF_SEGS_2[22];
  char TYPE_DESCRIPTION[22][40];
};

static DCAL DC5 = {
  {0.0, 7.62, 45.72, 22.86, 11.43, 91.44, 38.1, 60.96,
   12.7, 3.81, 12.7, 30.0, 20.0, 40.0, 40.0, 10.0,
   20.0, 20.32, 15.0, 15.0, 50.0, 38.1 },
  {0.0F, 7.62F, 2.377F, 2.377F, 2.377F, 2.377F, 1.074F, 1.074F, 
   0.493F, 3.81F, 3.81F, 30.0F, 20.0F, 40.0F, 40.0F, 1.0F, 1.0F,
   2.377F, 15.0F, 1.0F, 50.0F, 1.074F},
  {0.0F, 3.81F, 2.377F, 2.377F, 2.377F, 2.377F, 1.074F, 1.074F,
   0.493F, 2.54F, 2.54F, 2.54F, 2.54F, 2.54F, 2.54F, 1.0F, 1.0F,
   2.377F, 0.02F, 0.02F, 2.54F, 1.074F},
  {0.0F, 0.001F, 1.00F, 1.00F, 1.00F, 1.00F, 1.00F, 1.00F,
   1.00F, 0.001F, 0.001F, 1.00F, 1.00F, 1.00F, 1.00F, 
   1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F},
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 
    1, 2, 1, 2, 1 },
  { 1, 1, 1, 1, 1, 16, 1, 64, 1, 1, 1, 85, 64, 128, 128,
    256, 256, 8, 150, 74, 256, 64 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 85, 64, 128, 128,
    1, 1, 1, 150, 1, 256, 1},
  {"Not a detector",
   "3\" pancake monitor",
   "1\" x 18\"",
   "1\" x 9\"",
   "1\" x 4.5\"",
   "1\" x 36\" LPSD",
   "0.5\" x 15\"",
   "0.5\" x 24\" LPSD",
   "0.25\" x 5\"",
   "1.5\" pancake monitor",
   "OrdellaBeam Monitor",
   "SCD Anger Camera",
   "Ordella 2210 SAD 20cm",
   "Ordella 2400 SAND 40cm",
   "Ordella 2410 SAND 40cm",
   "Ordella 1204 POSY1 10cm",
   "Ordella 1210 POSY2 20cm",
   "1\" x 8\" LPSD",
   "New SCD Anger Camera",
   "New SCD Anger Diagnostic",
   "SAD Brookhaven Detector",
   "0.5\" x 15\" LPSD"
  }
};
class Runfile {
 public:
  Runfile();
  Runfile( char * );
  Header getHeader();
  vector<float> Get1DSpectrum(Segment seg, int hist);
  vector<float> TimeChannelBoundaries(Segment seg, int hist);
  vector<Segment *> GetSegments();

 private:
  int leaveOpen;
  char *runfileName;
  ifstream *inFile;
  RandomAccessRunfile *runFile;
  Header *header;
  vector<DetectorMap *> detectorMap;
  TimeField **timeField;
  vector<float> detectorAngle;
  vector<float> flightPath;
  vector<float> detectorHeight;
  vector<short> detectorType;
  vector<float> timeScale;
  vector<float> timeShift;
  vector<float> areaStartTime;
  vector<float> timeDelay;
  char *messageRegion;
  LpsdDetIdMap *lpsdIDMap;
  vector<DiscLevel> discriminator;
  vector<int> subgroupIDList;
  vector<int> maxSubgroupID;
  vector<int> minSubgroupID;
  vector<vector<int> > subgroupMap;
  vector<vector<Segment *> > segmentMap;
  int versionNumber;
  vector<Segment *> segments;
  vector<float> detectorLength;
  vector<float> detectorWidth;
  vector<float> detectorDepth;
  vector<short> detCoordSys;
  vector<float> detectorRot1;
  vector<float> detectorRot2;
  vector<float> detectorEfficiency;
  vector<int> psdOrder;
  vector<int> numSegs1;
  vector<int> numSegs2;
  vector<int> crateNum;
  vector<int> slotNum;
  vector<int> inputNum;
  vector<int> dataSource;
  vector<int> minID;
  vector<int> overflows;
  void LoadV4( ifstream *);
  void LoadV5( ifstream *);
  void setMinID();
};
		    
#endif
