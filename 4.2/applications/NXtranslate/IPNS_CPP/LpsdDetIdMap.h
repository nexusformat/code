#ifndef IPNS_RUNFILE_LPSDDETIDMAP_H
#define IPNS_RUNFILE_LPSDDETIDMAP_H
#include <iostream>
#include <fstream>
#include "Header.h"
using namespace std;

struct MapElement {
  int detID;
  int crate;
  int slot;
  int input;
  int minID;
};

class LpsdDetIdMap {

 public:
  LpsdDetIdMap();
  LpsdDetIdMap(ifstream *, Header *);
  int NumOfBanks();
  void NumDetsInBank(int *);
  void DetsInBank(int, int []);
  int CrateForDet(int, int);
  int SlotForDet(int, int);;
  int InputForDet(int, int);
  int MinIdForDet(int, int);
 
  

 private:
  MapElement *map[12];
  int numDetsInBank[12];
  int numBanks;

  
};

#endif
