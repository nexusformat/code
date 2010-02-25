#ifndef IPNS_RUNFILE_DETECTORMAP_H
#define IPNS_RUNFILE_DETECTORMAP_H
#include <iostream>
#include <fstream>
#include "Header.h"
using namespace std;

class DetectorMap {
 public:
  DetectorMap();
  DetectorMap(ifstream *, int, Header);
  int GetAddress();
  int GetTFType();
  int GetMHBit();

 private:
  int address;
  int tfType;
  int moreHistBit;
  char *iName;
  int versionNumber;
};

int GetDetectorMapTableSize(int);

#endif
