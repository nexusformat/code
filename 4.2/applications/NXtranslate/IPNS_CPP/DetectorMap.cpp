#include <cstring>
#include "DetectorMap.h"
#include "RandomAccessRunfile.h"

DetectorMap::DetectorMap(){}
// Constructor
DetectorMap::DetectorMap(ifstream *inFile, int id, Header header) {
  int temp;
  iName = new char[4];
  header.IName( iName );
  versionNumber = header.VersionNumber();
  RandomAccessRunfile runFile = RandomAccessRunfile(inFile);
  if ( versionNumber < 6 ) {
    temp = runFile.readRunInt();
  }
  else {
    address = runFile.readRunInt();
    tfType = runFile.readRunShort();
    moreHistBit = runFile.readRunShort();
  }
  if (versionNumber) {
    if ((!(strcmp( iName, "glad" ) == 0) &&
	 !(strcmp( iName, "lpsd" ) == 0)) || 
	versionNumber >= 5 ) {
      address = temp & 0x7FFFFF;
      tfType = (temp >> 24) & 0xFF;
      moreHistBit = (temp >> 23) & 0x1;
    }
    else {
      address = temp & 0xFFFF;
      address = address << 8;
      tfType = (temp >> 24) & 0xFF;
      moreHistBit = (temp >> 23) & 0x1;
    }
  }
}

// return the value of the address for this item
int DetectorMap::GetAddress(){
  return address;
}

// return the value of the time field Type for this item
int DetectorMap::GetTFType(){
  return tfType;
}

// return the value of the more hist bit for this item
int DetectorMap::GetMHBit(){
  return moreHistBit;
}

int GetDetectorMapTableSize( int versionNumber ) {
  if (versionNumber <= 5 ) {
    return (4);
  }
  else {
    return (8);
  }
}
