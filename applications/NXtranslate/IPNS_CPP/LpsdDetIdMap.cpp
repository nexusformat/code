#include "LpsdDetIdMap.h"
#include "RandomAccessRunfile.h"

// set up constructor
LpsdDetIdMap::LpsdDetIdMap(ifstream *inFile, Header *header){
  int mapIndex = 0;
  int ii;
  int curFilePos = (*inFile).tellg();
  numBanks = -1;
  RandomAccessRunfile runFile(inFile);
  (*inFile).seekg((*header).getLpsdDetIDMapLoc());
  while (mapIndex < (*header).getLpsdDetIDMapSize()/2){
    int bankNo = runFile.readRunShort();
    if (bankNo > numBanks) {
      numBanks = bankNo;
    }
    int numDets = runFile.readRunShort();
#ifdef IPNS_LPSD_ID_MAP_DEBUG
    cout << "Bank: "  << bankNo << ", numDets: " <<
      numDets << "\n";
    cout << "detID    crate   slot   input   minID\n";
#endif  
    
    /*for ( ii = 0; ii < 5; ii++ ) {
      int temp = runFile.readRunShort();
    }*/
    mapIndex += 7;
    map[bankNo] = new MapElement[numDets];
    numDetsInBank[bankNo] = numDets;
    for (ii = 0; ii< numDets; ii++ ) {
      map[bankNo][ii].detID = runFile.readRunShort();
      map[bankNo][ii].crate = runFile.readRunShort();
      map[bankNo][ii].slot = runFile.readRunShort();
      map[bankNo][ii].input = runFile.readRunShort();
      map[bankNo][ii].minID = runFile.readRunShort();
      int temp = runFile.readRunShort();
      temp = runFile.readRunShort();
      mapIndex += 7;
    }
#ifdef IPNS_LPSD_ID_MAP_DEBUG
    for ( ii = 0; ii < numDets; ii++) {
      cout << map[bankNo][ii].detID << "   " << map[bankNo][ii].crate <<
	"   " << map[bankNo][ii].slot << "    " << map[bankNo][ii].input <<
	"   " << map[bankNo][ii].minID << "\n";
    }
#endif  
    }   
  (*inFile).seekg(curFilePos);
}

int LpsdDetIdMap::NumOfBanks(){
  return numBanks;
}

void LpsdDetIdMap::NumDetsInBank(int *nDetsInBank) {

  int ii;
  for( ii =0; ii < 12; ii++) {
    nDetsInBank[ii] = numDetsInBank[ii];
  }
}
void LpsdDetIdMap::DetsInBank(int bank, int *dets){
  int ii;
  for (ii=0; ii<numDetsInBank[bank]; ii++) {
    dets[ii] = (map[bank][ii]).detID;
  }
}

int LpsdDetIdMap::CrateForDet(int bank, int det) {
  return map[bank][det - 1].crate;
}

int LpsdDetIdMap::SlotForDet(int bank, int det) {
  return map[bank][det - 1].slot;
}

int LpsdDetIdMap::InputForDet(int bank, int det) {
  return map[bank][det - 1].input;
}

int LpsdDetIdMap::MinIdForDet(int bank, int det) {
  return map[bank][det - 1].minID;
}

