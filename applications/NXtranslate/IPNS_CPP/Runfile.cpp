#include "Runfile.h"
#include <string>
#include <stdexcept>
//#include <algorithm.h>
float PI = 3.14159;
using std::string;

Runfile::Runfile(){}

// setup Constructor
Runfile::Runfile( char *fileName ){

  int nameLen = strlen(fileName);
  
  runfileName =  new char[nameLen + 1];
  int ii;
  strcpy( runfileName, fileName);

  inFile = new ifstream(runfileName, ios::in);

  if (!inFile ) {
    throw std::runtime_error("File could not be opened: "+string(fileName));
  }
  runFile = new RandomAccessRunfile(inFile); 

  char *tokenPtr;
  char *fileName2;
  fileName2 = new char[ nameLen + 1];
  strcpy( fileName2, fileName );
  char *fileNameBeg;
  
  tokenPtr = strtok( fileName2, "/");
  while (tokenPtr != NULL ) {
    fileNameBeg = tokenPtr;
    //  cout << fileName << endl;
    tokenPtr = strtok(NULL, "/");
  }
  
  char iName[5];

  iName[0] = fileNameBeg[0];
  iName[1] = fileNameBeg[1];
  iName[2] = fileNameBeg[2];
  iName[3] = fileNameBeg[3];
  iName[4] = '\0';

  //  cout << "Final iName: "<< fileName << endl;
  

  header = new Header(inFile, iName);
  versionNumber = header->VersionNumber();
  TableType detMapTab = header->GetDetMapTable();
  detectorMap.resize(detMapTab.size/GetDetectorMapTableSize(versionNumber) + 1);
  runFile->seekg(detMapTab.location);
  for ( ii = 1; ii <= detMapTab.size/GetDetectorMapTableSize(versionNumber); 
	ii++ ) {
    detectorMap[ii]=new DetectorMap( inFile, ii, *header );
#ifdef IPNS_RUNFILE_DEBUG
    cout << "DetMap (address, tfType, moreHistBit) [" << ii << "]: (" <<
      (*detectorMap[ii]).GetAddress() << ", " << (*detectorMap[ii]).GetTFType() << ", " << (*detectorMap[ii]).GetMHBit() << ")\n";
#endif
  }
  TableType tfMapTab = header->GetTimeFieldTable();  
  timeField = new TimeField*[tfMapTab.size/16 + 1];
  runFile->seekg(tfMapTab.location);
#ifdef IPNS_RUNFILE_DEBUG
  cout << "TimeField Map\n";
  cout << "tfNum    tMin     tMax     tStep   tDouble  NumChn   tf em del En wv ph log\n";
#endif
  for ( ii = 1; ii <= tfMapTab.size/16; ii++ ) {
    timeField[ii]=new TimeField( inFile, ii, *header );
#ifdef IPNS_RUNFILE_DEBUG
    cout << " " << ii << "   " <<
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      (*timeField[ii]).GetTMin()  << 
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed)<<
      (*timeField[ii]).GetTMax() <<
      "  " << setw(8) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed)<<
      (*timeField[ii]).GetTStep() <<  
      setw(8) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetTDoubleLength() << 
      setw(8) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetNumOfChannels() << 
      setw(6) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetTimeFocusBit() << 
      setw(3) << setiosflags( ios::right | ios::dec) <<
// return wavelength Bin Bit
      (*timeField[ii]).GetEmissionDelayBit() << 
      setw(3) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetConstantDelayBit() << 
      setw(4) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetEnergyBinBit() << 
      setw(3) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetWavelengthBinBit() << 
      setw(3) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetPulseHeightBinBit() << 
      setw(3) << setiosflags( ios::right | ios::dec) <<
      (*timeField[ii]).GetLogBinBit() << 
      "\n";
#endif
  }
  
//  int nel;
  TableType daTable = header->GetDetectorAngleTable();
#ifdef IPNS_RUNFILE_DEBUG
  cout << "reading angle table:       " << daTable.size/4 <<
    " words starting at byte offset " << daTable.location << "\n";
#endif
  detectorAngle.resize(daTable.size/4 +1);
  runFile->seekg(daTable.location);
  runFile->readRunFloatVector( &detectorAngle, daTable.size/4);
  
  TableType fpTable = header->GetFlightPathTable();
#ifdef IPNS_RUNFILE_DEBUG
  cout << "reading flight path table: " << fpTable.size/4 <<
    " words starting at byte offset " << fpTable.location << "\n";
#endif
  flightPath.resize(fpTable.size/4 +1);
  runFile->seekg(fpTable.location);
  runFile->readRunFloatVector( &flightPath, fpTable.size/4);
  
  TableType dhTable = header->GetDetectorHeightTable();
#ifdef IPNS_RUNFILE_DEBUG
  cout << "reading height table:      " << dhTable.size/4 <<
    " words starting at byte offset " << dhTable.location << "\n";
#endif
  detectorHeight.resize(dhTable.size/4 +1);
  runFile->seekg(dhTable.location);
  runFile->readRunFloatVector( &detectorHeight, dhTable.size/4);
  
  TableType dtTable = header->GetDetectorTypeTable();
#ifdef IPNS_RUNFILE_DEBUG
  cout << "reading type table:        " << dtTable.size/2 <<
    " words starting at byte offset " << dtTable.location << "\n";
#endif
  detectorType.resize(dtTable.size/2 +1);
  runFile->seekg(dtTable.location);
  runFile->readRunShortVector( &detectorType, dtTable.size/2);
    
#ifdef IPNS_RUNFILE_DEBUG
  cout << "  ID    Angle  FP Length   Height  Type \n";
  for ( ii = 1; ii <= header->NDet(); ii++ ) {
    cout  <<
      setw(4) << setiosflags( ios::right | ios::dec) <<
      ii  <<
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      detectorAngle[ii] << 
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      flightPath[ii] << 
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      detectorHeight[ii] <<
      setw(7) << setiosflags( ios::right | ios::dec) <<
      detectorType[ii] << "\n";
  }
#endif  
  if ( header->VersionNumber() > 4 ) {
    LoadV5(inFile);
  }
  else {
    LoadV4(inFile);
  }
#ifdef IPNS_RUNFILE_DEBUG
  cout<< detectorMap.size() << "  " << sizeof (DetectorMap)<<"\n";
  for (ii =1; ii < detectorMap.size(); ii++) {
    cout << "DetMap (address, tfType, moreHistBit) [" << ii << "]: (" <<
      (*detectorMap[ii]).GetAddress() << ", " << (*detectorMap[ii]).GetTFType() <<
      ", " << (*detectorMap[ii]).GetMHBit() << ")\n";
  }
#endif
#ifdef IPNS_RUNFILE_DEBUG
  cout << "  ID    Angle  FP Length   Height  Type \n";
  for ( ii = 1; ii <= header->NDet(); ii++ ) {
    cout  <<
      setw(4) << setiosflags( ios::right | ios::dec) <<
      ii  <<
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      detectorAngle[ii] << 
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      flightPath[ii] << 
      setw(9) << setprecision(2)<<
      setiosflags(ios::showpoint | ios::right| ios::fixed) << 
      detectorHeight[ii] <<
      setw(7) << setiosflags( ios::right | ios::dec) <<
      detectorType[ii] << "\n";
  }
  cout << "****************************************\n";
  cout << "**** subgroupMap ***********************\n";
  cout << "****************************************\n";
  for ( ii = 1; ii < subgroupMap.size(); ii++ ) {
    cout << "subgroup " << ii << ":";
    int jj;
    for ( jj = 1; jj < subgroupMap[ii].size(); jj++ ) {
      cout << subgroupMap[ii][jj] << ", ";
    }
    cout << "\n";
  }
  cout << "****************************************\n";
  cout << "****************************************\n";
  cout << "******overflows*************************\n";
  cout << "****************************************\n";
  if (header->NumOfOverflows() > 0) {
    for (ii = 1 ; ii <= header->NumOfOverflows(); ii++ ) {
      cout << overflows[ii]<< ", ";
    }
    cout << "\n";
  }
  else {
    cout << "No Overflows\n";
  }
  cout << "****************************************\n";
  cout << "****************************************\n";
  
#endif  
  
}


Header  Runfile::getHeader() {
  return *header;
}


//load V4 specific information
void Runfile::LoadV4(ifstream *inFile) {
  int *gladbank;
  int *gladdetinbank;

  detectorLength.resize(header->NDet() + 1);
  detectorWidth.resize(header->NDet() + 1);
  detectorDepth.resize(header->NDet() + 1);
  detectorEfficiency.resize(header->NDet() + 1);
  psdOrder.resize(header->NDet() + 1);
  numSegs1.resize(header->NDet() + 1);
  numSegs2.resize(header->NDet() + 1);
  crateNum.resize(header->NDet() + 1);
  slotNum.resize(header->NDet() + 1);
  inputNum.resize(header->NDet() + 1);
  dataSource.resize(header->NDet() + 1);
  minID.resize(header->NDet() + 1);
  detectorRot1.resize(header->NDet() + 1);
  detectorRot2.resize(header->NDet() + 1);
  char iName[4];
  header->IName(iName);
  if ( (strcmp(iName, "scd0") == 0) ||
       (strcmp(iName, "sad0") == 0) ||
       (strcmp(iName, "sad1") == 0) ||
       (strcmp(iName, "sand") == 0) || 
       (strcmp(iName, "pne0") == 0) || 
       (strcmp(iName, "posy") == 0) ) {
    
    detectorAngle.resize(detectorAngle.size() + 1);
    flightPath.resize(flightPath.size() + 1);
    detectorHeight.resize(detectorHeight.size() + 1);
    detectorType.resize(detectorType.size() + 1);
    detectorMap.resize(detectorMap.size() + 1);

    detectorMap[detectorMap.size() - 1] = new DetectorMap();
    detectorAngle[detectorAngle.size() - 1] = (float)header->Dta() -
      (float)(atan(header->XDisplacement()/header->Dtd()) 
	      * 180/PI);
    flightPath[flightPath.size() - 1] = (float)header->Dtd()/100.0f;
    detectorHeight[detectorHeight.size() - 1] = 
      (float)header->YDisplacement()/100.0f;
    detectorMap[sizeof(detectorMap)/sizeof(DetectorMap) - 1] = 
      new DetectorMap( );
    if ( (strcmp(iName, "scd0") == 0) ){
      detectorType[ detectorType.size() - 1 ] = 11;
    }
    else if ( (strcmp(iName, "sad0") == 0) ||
	      (strcmp(iName, "sad1") == 0) ){
      if ((header->NumOfX() == 64) && (header->NumOfY() == 64)) {
	detectorType[ detectorType.size() - 1 ] = 12;
      }
      else if ((header->NumOfX() == 128) && (header->NumOfY() == 128)) {
	detectorType[ detectorType.size() - 1 ] = 13;
      }
      else if ((header->NumOfX() == 1) && (header->NumOfY() == 1)) {
	detectorType[ detectorType.size() - 1 ] = 13;
      }
      
    }
    else if ( (strcmp(iName, "sand") == 0) ){
      detectorType[ detectorType.size() - 1 ] = 13;
      
    }
    else if ( (strcmp(iName, "pne0") == 0) ){
      detectorType[ detectorType.size() - 1 ] = 15;
      
    }
    else if ( (strcmp(iName, "posy") == 0) ){
      detectorType[ detectorType.size() - 1 ] = 16;
      
    }
    
  }
  
  lpsdIDMap = new LpsdDetIdMap( inFile, header );
  segments.resize(header->NumOfElements() + 1);
  for ( int ii = 1; ii <= header->NDet(); ii++ ) {
    if ( detectorAngle[ii] == 0.0F &&
	 flightPath[ii] == 0.0F &&
	 detectorType[ii] == 0 ) {
    }
    else if ( (strcmp(iName, "hrcs") == 0 ) || 
	      (strcmp(iName, "lrcs") == 0 ) ) {
      if ( header->VersionNumber() < 4) {
	switch (detectorType[ii]){
	case 0: {
	  if ( ii < 3 )
	    detectorType[ii] = 1;
	  break;
	}
	case 1: {
	  detectorType[ii] = 2;
	  break;
	}
	case 2: {
	  detectorType[ii] = 3;
	  break;
	}
	case 5: {
	  detectorType[ii] = 4;
	  break;
	}
	}
      }
    }
    else if ( (strcmp(iName, "gppd") == 0 ) ) {
      switch (detectorType[ii]){
      case 1: {
	detectorType[ii] = 9;
	break;
      }
      case 2: {
	detectorType[ii] = 6;
	break;
      }
      }
    }
    else if ( (strcmp(iName, "sepd") == 0 ) ) {
      switch (detectorType[ii]){
      case 1: {
	detectorType[ii] = 6;
	break;
      }
      case 2: {
	detectorType[ii] = 9;
	break;
      }
      }
    }
    else if ( (strcmp(iName, "qens") == 0 ) ) {
      switch (detectorType[ii]){
      case 0: {
	detectorType[ii] = 8;
	break;
      }
      case 1: {
	detectorType[ii] = 10;
	break;
      }
      }
    }
    else if ( (strcmp(iName, "hipd") == 0 ) ) {
      switch (detectorType[ii]){
      case 0: {
	detectorType[ii] = 9;
	break;
      }
      case 1: {
	detectorType[ii] = 6;
	break;
      }
      }
    }
    else if ( (strcmp(iName, "chex" ) == 0) ) {
      switch (detectorType[ii]){
      case 0: {
	detectorType[ii] = 6;
	break;
      }
      case 1: {
	detectorType[ii] = 10;
	break;
      }
      }
    }
    if ((strcmp(iName, "hrcs") == 0 )||
 	(strcmp(iName, "lrcs") == 0 )||
 	(strcmp(iName, "gppd") == 0 )||
 	(strcmp(iName, "sepd") == 0 )||
 	(strcmp(iName, "qens") == 0 )||
 	(strcmp(iName, "hipd") == 0 )||
        (strcmp(iName, "chex") == 0 ) ){
      psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
      numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
      numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
      detectorLength[ii] = DC5.LENGTH[detectorType[ii]];
      detectorWidth[ii] = DC5.WIDTH[detectorType[ii]];
      detectorDepth[ii] = DC5.DEPTH[detectorType[ii]];
      detectorRot1[ii] = 0.0f;
      detectorRot2[ii] = 0.0f;
      minID[ii] = ii;
      segments[ii] = new Segment();
      segments[ii]->SetDetID(ii); 
      segments[ii]->SetRow(1); 
      segments[ii]->SetColumn(1); 
      segments[ii]->SetLength(DC5.LENGTH[detectorType[ii]]); 
      segments[ii]->SetWidth(DC5.WIDTH[detectorType[ii]]); 
      segments[ii]->SetDepth(DC5.DEPTH[detectorType[ii]]); 
      segments[ii]->SetEfficiency(DC5.EFFICIENCY[detectorType[ii]]); 
      segments[ii]->SetSegID(ii);
      if ( header->VersionNumber() < 4 ) {
	crateNum[ii] = (ii-1)/160 + 1;
	slotNum[ii] = (ii- (crateNum[ii] - 1) * 160 -1) / 8   
	  + 1;
	inputNum[ii] = ii - (crateNum[ii] -1) * 160 - 
	  (slotNum[ii] -1) * 8;
      }
      else {
	crateNum[ii] = (ii-1)/176 + 1;
	slotNum[ii] = (ii- (crateNum[ii] - 1) * 176 -1) / 16   
	  + 1;
	inputNum[ii] = ii - (crateNum[ii] -1) * 176 - 
	  (slotNum[ii] -1) * 16;
      }
    }
    if ( (strcmp(iName, "scd0") == 0) ) {
      switch (detectorType[ii]){
      case 0:
      case 1:{
	detectorType[ii] = 1;
	crateNum[ii] = 1;
	slotNum[ii] = 1;
	inputNum[ii] = ii;
	break;
      }
      case 11: {
	detectorType[ii] = 11;
	crateNum[ii] = 1;
	slotNum[ii] = 19;
	inputNum[ii] = 1;
	psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	break;
      }
      }
    }
    
    
    if ( (strcmp(iName, "sad0") == 0) ||
	 (strcmp(iName, "sad1") == 0) ) {
      switch (detectorType[ii]){
      case 1: {
	detectorType[ii] = 9;
	crateNum[ii] = 1;
	slotNum[ii] = 1;
	inputNum[ii] = ii;
	if ((ii == 1) && (detectorAngle[ii] == 0.000f)) {
	  detectorAngle[ii] = 180.0f;
	}
	break;
      }
      case 2: {
	detectorType[ii] = 9;
	crateNum[ii] = 1;
	slotNum[ii] = 1;
	inputNum[ii] = ii;
	break;
      }
      case 12: 
      case 13: {
	crateNum[ii] = 1;
	slotNum[ii] = 19;
	inputNum[ii] = 1;
	if ((header->NumOfX() == 64) && (header->NumOfY() == 64)) {
	  detectorType[ii] = 12;
	  psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	  numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	  numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	}
	else if ((header->NumOfX() == 128) && (header->NumOfY() == 128)) {
	  detectorType[ii] = 13;
	  psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	  numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	  numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	}
	else if ((header->NumOfX() == 1) && (header->NumOfY() == 1)) {
	  detectorType[ii] = 12;
	  psdOrder[ii] = 1;
	  numSegs2[ii] = 1;
	}
	break;
      }
      }
    }
    if ( (strcmp(iName, "sand") == 0) ){
      switch (detectorType[ii]){
      case 0: 
      case 1: {
	detectorType[ii] = 9;
	crateNum[ii] = 1;
	slotNum[ii] = 1;
	inputNum[ii] = ii;
	if ((ii == 1) && (detectorAngle[ii] == 0.000f)) {
	  detectorAngle[ii] = 180.0f;
	}
	break;
      }
      case 13: {
	detectorType[ii] = 13;
	crateNum[ii] = 1;
	slotNum[ii] = 19;
	inputNum[ii] = 1;
	if ((header->NumOfX() == 64) && (header->NumOfY() == 64)) {
	  detectorType[ii] = 12;
	  psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	  numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	  numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	}
	else if ((header->NumOfX() == 128) && (header->NumOfY() == 128)) {
	  detectorType[ii] = 13;
	  psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	  numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	  numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	}
	else if ((header->NumOfX() == 1) && (header->NumOfY() == 1)) {
	  detectorType[ii] = 13;
	  psdOrder[ii] = 2;
	  numSegs1[ii] = 1;
	  numSegs2[ii] = 1;
	}
	break;
      }
      }
    }
    if ( (strcmp(iName, "pne0") == 0) ){
      switch (detectorType[ii]){
      case 0: 
      case 1: {
	detectorType[ii] = 1;
	break;
      }
      case 15: {
	detectorType[ii] = 15;
	crateNum[ii] = 1;
	slotNum[ii] = 19;
	inputNum[ii] = 1;
	psdOrder[ii] = 2;
	numSegs2[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	numSegs1[ii] = 2;
	break;
      }
      }
    }
    if ( (strcmp(iName, "posy") == 0) ){
      switch (detectorType[ii]){
      case 0: 
      case 1: {
	detectorType[ii] = 1;
	break;
      }
      case 16: {
	detectorType[ii] = 16;
	crateNum[ii] = 1;
	slotNum[ii] = 19;
	inputNum[ii] = 1;
	psdOrder[ii] = 2;
	numSegs2[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	numSegs1[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	break;
      }
      }
    }
    if ( (strcmp(iName, "scd0") == 0 ) ||
	 (strcmp(iName, "sad0") == 0 ) ||
	 (strcmp(iName, "sad1") == 0 ) ||
	 (strcmp(iName, "posy") == 0 ) ||
	 (strcmp(iName, "pne0") == 0 ) ||
	 (strcmp(iName, "sand") == 0 ) ){
      switch (detectorType[ii]) {
      case 0:
      case 1:
      case 9: {
	psdOrder[ii] = DC5.PSD_DIMENSION[detectorType[ii]];
	numSegs1[ii] = DC5.NUM_OF_SEGS_1[detectorType[ii]];
	numSegs2[ii] = DC5.NUM_OF_SEGS_2[detectorType[ii]];
	detectorLength[ii] = DC5.LENGTH[detectorType[ii]];
	detectorWidth[ii] = DC5.WIDTH[detectorType[ii]];
	detectorDepth[ii] = DC5.DEPTH[detectorType[ii]];
	minID[ii] = ii;
	segments[ii] = new Segment();
	segments[ii]->SetDetID(ii); 
	segments[ii]->SetRow(1); 
	segments[ii]->SetColumn(1); 
	segments[ii]->SetLength(DC5.LENGTH[detectorType[ii]]); 
	segments[ii]->SetWidth(DC5.WIDTH[detectorType[ii]]); 
	segments[ii]->SetDepth(DC5.DEPTH[detectorType[ii]]); 
	segments[ii]->SetEfficiency(DC5.EFFICIENCY[detectorType[ii]]); 
	segments[ii]->SetSegID(ii);
	break;
      }
      case 11:
      case 12:
      case 13:
      case 15:
      case 16: {
	detectorLength[ii] = (float)(header->YUpper() - header->YLower());
	detectorWidth[ii] = (float)(header->XRight() - header->XLeft() );
	detectorDepth[ii] = DC5.DEPTH[detectorType[ii]];
	detectorHeight[ii] = (float)header->YDisplacement()/100.0f;
	minID[ii] = ii;
	int index;
	for ( int segY = 0; segY < numSegs1[ii]; segY++) {
	  for ( int segX = 0; segX < numSegs2[ii]; segX++) {
	    index = ii + segX + segY *( numSegs2[ii]);
	    segments[index] = new Segment();
	    segments[index]->SetDetID(ii); 
	    segments[index]->SetRow(segY + 1); 
	    segments[index]->SetColumn(segX + 1); 
	    segments[index]->SetLength(detectorLength[ii]/numSegs1[ii]);
	    segments[index]->SetWidth(detectorWidth[ii]/numSegs2[ii]); 
	    segments[index]->SetDepth( DC5.DEPTH[detectorType[ii]]); 
	    segments[index]->SetEfficiency(DC5.EFFICIENCY[detectorType[ii]]); 
	    segments[index]->SetSegID(index);
	  }
	}
	break;
      }
      }		
      
    }
  }
  if ( (strcmp(iName, "glad") == 0 ) || 
       (strcmp(iName, "lpsd") == 0 ) ) {
    psdOrder.resize(header->NumOfElements() + 1);
    numSegs1.resize(header->NumOfElements() + 1);
    numSegs2.resize(header->NumOfElements() + 1);
    crateNum.resize(header->NumOfElements() + 1);
    slotNum.resize(header->NumOfElements() + 1);
    inputNum.resize(header->NumOfElements() + 1);
    dataSource.resize(header->NumOfElements() + 1);
    minID.resize(header->NumOfElements() + 1);
    detectorRot1.resize(header->NumOfElements() +1);
    detectorRot2.resize(header->NumOfElements() +1);
    gladbank = new int[header->NumOfElements() + 1];
    gladdetinbank = new int[header->NumOfElements() + 1];
    detectorLength.resize(header->NumOfElements() + 1);
    detectorWidth.resize(header->NumOfElements() + 1);
    detectorDepth.resize(header->NumOfElements() + 1);
    int detNum = 0;
    int *dets =new int[0];
    for ( int jj = 0; jj < lpsdIDMap->NumOfBanks(); jj++ ) {
      lpsdIDMap->DetsInBank(jj, dets);
      int tminID, tcrate, tslot, tinput;
      for ( uint kk = 0; kk < sizeof(dets)/4;
	    kk++ ) {
	detNum++;
	tminID = lpsdIDMap->MinIdForDet(jj,dets[kk]);
	tcrate = lpsdIDMap->CrateForDet(jj,dets[kk]);
	tslot = lpsdIDMap->SlotForDet(jj,dets[kk]);	
	tinput = lpsdIDMap->InputForDet(jj,dets[kk]);
	if ( tminID != 0 && tcrate != 0 && tslot != 0 &&
	     tinput != 0 ) {
	  if ( jj == 0 ) {
	    psdOrder[tminID] = 1;
	    numSegs1[tminID] = 1;
	    numSegs2[tminID] = 1;
	    crateNum[tminID] = tcrate;
	    slotNum[tminID] = tslot;
	    inputNum[tminID] = tinput;
	    detectorRot1[tminID] = 0.0f;
	    detectorRot2[tminID] = 0.0f;
	    segments[tminID] = new Segment();
	    segments[tminID]->SetDetID(tminID); 
	    segments[tminID]->SetRow(1);
	    segments[tminID]->SetColumn(1); 
	    segments[tminID]->SetLength(DC5.LENGTH[1]); 
	    segments[tminID]->SetWidth(DC5.WIDTH[1]); 
	    segments[tminID]->SetDepth(DC5.DEPTH[1]); 
	    segments[tminID]->SetEfficiency(DC5.EFFICIENCY[1]); 
	    segments[tminID]->SetSegID(tminID);
	    gladbank[tminID] = 0;
	    gladdetinbank[tminID] = kk;
	  }
	  else {
	    for ( int ll = 0; ll < 64; ll++ ) {
	      psdOrder[tminID + ll] = 1;
	      numSegs1[tminID + ll] = 1;
	      numSegs2[tminID + ll] = 1;
	      crateNum[tminID + ll] = tcrate;
	      slotNum[tminID + ll] = tslot;
	      inputNum[tminID + ll] = tinput;
	      psdOrder[tminID + ll] = 1;
	      numSegs1[tminID + ll] = 1;
	      numSegs2[tminID + ll] = 1;
	      detectorRot1[tminID + ll] = 0.0f;
	      detectorRot2[tminID + ll] = 0.0f;
	      detectorLength[tminID+ll] = DC5.LENGTH[7]/64; 
	      detectorWidth[tminID+ll] = DC5.WIDTH[7]; 
	      detectorDepth[tminID] = DC5.DEPTH[7]; 
	      segments[tminID + ll] = new Segment();
	      segments[tminID + ll]->SetDetID(tminID + ll); 
	      segments[tminID + ll]->SetRow(1); 
	      segments[tminID + ll]->SetColumn(1); 
	      segments[tminID + ll]->SetLength(DC5.LENGTH[7]/ 64); 
	      segments[tminID + ll]->SetWidth(DC5.WIDTH[7]); 
	      segments[tminID + ll]->SetDepth( DC5.DEPTH[7]); 
	      segments[tminID + ll]->SetEfficiency(DC5.EFFICIENCY[7]); 
	      segments[tminID + ll]->SetSegID(tminID + ll);
	      gladbank[tminID + ll] = jj;
	      gladdetinbank[tminID + ll] = kk;
	    }
	  }
	}
      }
    }
  }
  int i;
  if (header->VersionNumber() > 3) {
    TableType discTable = header->GetDiscSetTable();
    inFile->seekg(discTable.location);
    int size = header->NDet()*2;
    vector<short> tdisc(size + 1, 0);
    runFile->readRunShortVector(&tdisc, size );
    discriminator.resize(header->NDet() + 1);
    for (i = 1; i <= header->NDet(); i++) {
      //      discriminator[i] = new DiscLevel();
      discriminator[i].lowerLevel = tdisc[(i-1)*2 + 1];
      discriminator[i].upperLevel = tdisc[(i-1)*2 + 2];
    }
  }
  leaveOpen = false;
  
  subgroupIDList.resize(header->NumOfElements() * header->NumOfHistograms() + 1 );
  subgroupMap.resize(1);
  subgroupMap[0].resize(1);
  segmentMap.resize(1);
  segmentMap[0].resize(1);
  for (i=0; (static_cast<uint>(i)) < subgroupIDList.size(); i++ )
    subgroupIDList[i] = -1;
  int group = 0;
  maxSubgroupID.resize( header->NumOfHistograms() + 1 );
  minSubgroupID.resize( header->NumOfHistograms() + 1 );
  for (int nHist = 1; nHist <= header->NumOfHistograms(); nHist++) {
    minSubgroupID[nHist] = group + 1;
    if((!(strcmp(iName, "scd0") == 0)&&
	!(strcmp(iName, "sad0") == 0)&&
	!(strcmp(iName, "sad1") == 0)&&
	!(strcmp(iName, "posy") == 0)&&
	!(strcmp(iName, "pne0") == 0)&&
	!(strcmp(iName, "sand") == 0)) ){
      int index, tfType;
      int index2, tfType2;
      for (int nDet = 1; nDet <= header->NumOfElements(); nDet++ ) {
	index = ( (nHist -1) * header->NumOfElements() ) + nDet;
	tfType = detectorMap[index]->GetTFType();
	if ((subgroupIDList[index] == -1) && (tfType != 0)) {
	  group++;
	  subgroupIDList[index] = group;
	  subgroupMap.resize(group + 1);
	  subgroupMap[group].resize(2);
	  subgroupMap[group][1] = nDet;
	  segmentMap.resize(group + 1);
	  segmentMap[group].resize(1);
	  segmentMap[group][0] = segments[nDet];
	  for (int k = nDet+1; k <= header->NumOfElements(); k++) {
	    
	    index2 = 
	      ( (nHist -1) * header->NumOfElements() ) + k;
	    
	    tfType2 = detectorMap[index2]->GetTFType();
	    if ( ( detectorMap[index2]->GetAddress() == 
		   detectorMap[index]->GetAddress()) &&
		 (tfType2 != 0) ){
	      subgroupIDList[index2] = group;
	      subgroupMap[group].resize(subgroupMap[group].size() +1);
	      subgroupMap[group][subgroupMap[group].size() - 1] = k;
	      segmentMap[group].resize(segmentMap[group].size() + 1);
	      segmentMap[group][segmentMap[group].size() - 1] = segments[k];
	    }
	  }
	}
      }
    }
    else {
      int index, tfType,aindex;
      int index2, tfType2;
      for (int nDet = 1; nDet <= header->NDet(); nDet++ ) {
	if (nDet < header->NDet() ) {
	  index = ( (nHist -1) * header->NumOfElements() ) + nDet;
	  tfType = detectorMap[index]->GetTFType();
	  if ((subgroupIDList[index] == -1) && (tfType != 0)) {
	    group++;
	    subgroupIDList[index] = group;
	    subgroupMap.resize(group + 1);
	    subgroupMap[group].resize(2);
	    subgroupMap[group][1] = nDet;
	    segmentMap.resize(group + 1);
	    segmentMap[group].resize(1);
	    segmentMap[group][0] = segments[nDet];
	    for (int k = nDet+1; k <= header->NDet(); k++) {
	      if ( k < header->NDet() ) {
		index2 = 
		  ((nHist -1)*header->NumOfElements() ) + k;
		tfType2 = detectorMap[index2]->GetTFType();
		if ( ( detectorMap[index2]->GetAddress() == 
		       detectorMap[index]->GetAddress()) &&
		     (tfType2 != 0) ){
		  subgroupIDList[index2] = group;
		  subgroupMap[group].resize(subgroupMap[group].size() + 1);
		  subgroupMap[group][subgroupMap[group].size() - 1] = k;
		  segmentMap[group].resize(segmentMap[group].size() + 1);
		  segmentMap[group][segmentMap[group].size() - 1] =segments[k];
		  
		}
	      }
	    }
	  }
	}
	else { 
	  int segX = numSegs1[nDet];
	  int segY = numSegs2[nDet];
	  subgroupMap.resize(group + (segX*segY) + 1);
	  segmentMap.resize(group + (segX*segY) + 1);
	  for ( int ny = 0; ny < segY; ny++ ) {
	    for ( int nx=0; nx < segX; nx++ ) {
	      aindex = nx + ny*(segX);
	      subgroupMap[group + aindex + 1].resize(2);
	      subgroupMap[group + aindex + 1][1] = nDet;
	      segmentMap[group + aindex + 1].resize(1);
	      segmentMap[group + aindex + 1][1] = 
		segments[header->NDet() + aindex];
      
	    }
	  }
	  group = group + segX * segY;
	}
      }
    }
    maxSubgroupID[nHist] = group;
  }

  TableType tsTable = header->GetTimeScaleTable();
  inFile->seekg(tsTable.location);
  short *timeScaleTemp = new short[tsTable.size/2 + 1];
  timeScale.resize(tsTable.size/2 + 1);
  runFile->readRunShortArray(timeScaleTemp,
				 tsTable.size/2);
  for (i = 1; i <= tsTable.size/2; i++) {
    timeScale[i] = (float)(1 + timeScaleTemp[i]/ pow( (float)2, (float)15));
  }
  delete(timeScaleTemp);

  // Correct GLAD detector angles to be in plane angles
  if ( (strcmp(iName, "glad") == 0)) {
    double cvdr = PI/ 180.0;
    double cvrd = 180.0/PI;
    float fp, zdet,ang1;
    double xdet, ydetz, ydet, dist;
    for ( int ii = 0; ii <= header->NumOfElements(); ii++ ){
      if ( flightPath[ii] !=0.00f &&
	   detectorAngle[ii] != 0.00f && 
	   detectorHeight[ii] !=0.00f ) {
	fp = flightPath[ii];
	zdet = detectorHeight[ii];
	ang1 = detectorAngle[ii];
	xdet= 0.0;
	ydetz= 0.0;
	ydet = 0.0;
	dist = 0.0;
	xdet = cos( ang1 * cvdr) * fp;
	ydetz = sin(ang1 * cvdr) * fp;
	if ( fabs(ydetz * ydetz - zdet * zdet) < 0.001 && 
	     ydetz * ydetz< zdet*zdet){
	  ydet = 0.0;
	}
	else {
	  ydet = 
	    sqrt( ydetz * ydetz - zdet * zdet );
	}
	dist = 
	  sqrt( fp * fp - zdet * zdet );
	double ang2;
	if ( xdet < ydet ) {
	  ang2 =
	    (acos(xdet/dist) * cvrd);
	}
	else {
	  ang2 = 
	    (asin(ydet/dist) * cvrd );
	}
	/*	if ( ang2 == Double.NaN ) {
	  cout<<  ii < "detAngle set to 0 ";
	  detectorAngle[ii] = 0.0f;
	}
	else {
	*
	detectorAngle[ii] = (float)ang2;
	}*/
	if (gladbank[ii] > 6 ) {
	  detectorAngle[ii] = -1.0f * detectorAngle[ii];
	}
	else if (gladbank[ii] == 6 && gladdetinbank[ii] > 14 ) {
	  detectorAngle[ii] = -1.0f * detectorAngle[ii];
	}
      }
    }
    
  }

  //End fixing Glad detector angles
  // Read area detector start times
  TableType arTable = header->GetAreaStartTable();
  if ( arTable.size > 0 ) {
    inFile->seekg( arTable.location );
    int numStart = arTable.size/2;
    vector<int> iareaStartTime(numStart + 1); 
    runFile->readRunIntVector( &iareaStartTime, numStart );
    vector<float> areaStartTime(numStart);
    for (int ii = 0; ii < numStart; ii++) {
      areaStartTime[ii] = 
	iareaStartTime[ii+1]*(float)header->ClockPeriod();
    }
  }

  // Read Overflow Table
  if ( header->NumOfOverflows() > 0 ) {
    int nchall =  header->Channels1D();
    if ( (header->NumOfWavelengths() > 0) &&
	 (header->TotalChannels() > 0 )) {
      nchall = header->Channels1D() + header->TotalChannels();
    }
//    int nover = (header->EndOfOverflow() - nchall)/2;
    
    int offset = nchall*2 + header->HistStartAddress();
    inFile->seekg( offset );
    overflows.resize(header->NumOfOverflows() + 1);
    runFile->readRunIntVector(&overflows, header->NumOfOverflows());
    for (int ii = 1; ii <= header->NumOfOverflows(); ii++ ) {
      if (overflows[ii] > header->AddressOf2DData() && 
	  header->NumOfWavelengths()>0) {
	overflows[ii] = (overflows[ii]-header->AddressOf2DData());
      }
      else if (overflows[ii] > header->AddressOf1DData()){
	overflows[ii] = (overflows[ii]-header->AddressOf1DData());
	if ( header->NumOfWavelengths() > 0 ) {
	  overflows[ii]= overflows[ii]+header->TotalChannels() * 2;
	}
      }
      
    }
  }
  sort(overflows.begin(), overflows.end());

  
}

//load V5 specific information
void Runfile::LoadV5(ifstream *inFile) {
  TableType tab;
  
  tab = header->GetDetLengthTable();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector(&detectorLength, tab.size/4);
  
  tab = header->GetDetWidthTable();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector(&detectorWidth, tab.size/4);
  
  tab = header->GetDetDepthTable();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector(&detectorDepth, tab.size/4);

  tab = header->GetDetCoordSysTable();
  inFile->seekg(tab.location);
  runFile->readRunShortVector(&detCoordSys, tab.size/2);
  
  tab = header->GetDetRot1Table();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector(&detectorRot1, tab.size/4);
  
  tab = header->GetDetRot2Table();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector( &detectorRot2, tab.size/4);
  
  tab = header->GetDetEffTable();
  inFile->seekg(tab.location);
  runFile->readRunFloatVector( &detectorEfficiency, tab.size/4);
  
  tab = header->GetPsdOrderTable();
  if (tab.size != 0 ) {
    inFile->seekg(tab.location);
    runFile->readRunIntVector( &psdOrder, tab.size/4);
  }
  else {
    psdOrder.resize(header->NDet() + 1);
    for ( int ii = 1; ii <= header->NDet(); ii++ ) {
      if ( detectorType[ii] != 0 ) psdOrder[ii] = 1;
    }
  }
  
  tab = header->GetNumSegs1Table();
  if (tab.size != 0 ) {
    inFile->seekg(tab.location);
    runFile->readRunIntVector(&numSegs1, tab.size/4);
    for ( int i = 1; i <= tab.size / 4; i++ ) {
      //correction for missing data in Early HRMECS files
      if ((numSegs1[i] == 0 ) && (detectorType[i] != 0)) { 
	numSegs1[i] = 1;
      }
    }
  }
  else {
    numSegs1.resize(header->NDet() + 1);
    for ( int ii = 1; ii <= header->NDet(); ii++ ) {
      if ( detectorType[ii] != 0 ) numSegs1[ii] = 1;
    }
  }
  
  tab = header->GetNumSegs2Table();
  if (tab.size != 0 ) {
    inFile->seekg(tab.location);
 
    runFile->readRunIntVector( &numSegs2, tab.size/4);
	}
  else {
    numSegs2.resize(header->NDet() + 1);
    for ( int ii = 1; ii <= header->NDet(); ii++ ) {
      if ( detectorType[ii] != 0 ) numSegs2[ii] = 1;
    } 
  }
  
  tab = header->GetCrateNumTable();
  inFile->seekg(tab.location);
  if (tab.size > 0 ) {
    
    runFile->readRunIntVector( &crateNum, tab.size/4);
  }
  else {
    crateNum.resize(header->NDet() + 1);
  }
  
  tab = header->GetSlotNumTable();
  inFile->seekg(tab.location);
  if ( tab.size > 0 ) { 
    
    runFile->readRunIntVector( &slotNum, tab.size/4);
  }
  else {
    slotNum.resize(header->NDet() + 1);
  }
  
  tab = header->GetInputNumTable();
  inFile->seekg(tab.location);
  if ( tab.size > 0 ) {
    
    runFile->readRunIntVector( &inputNum, tab.size/4);
  }
  else {
    inputNum.resize(header->NDet() + 1);
  }
  
  tab = header->GetDataSourceTable();
  inFile->seekg(tab.location);
  if ( tab.size > 0 ) {
    
    runFile->readRunIntVector( &dataSource, tab.size/4 );
  }
  else {
    dataSource.resize(header->NDet() + 1);
    minID.resize(header->NDet() + 1);
    for (int i = 1; i <= header->NDet(); i++ ) {
      minID[i] = i;
    }
  }
  
  tab = header->GetMinIDTable();
  inFile->seekg(tab.location);
  if ( tab.size > 0 ) {
    runFile->readRunIntVector( &minID, tab.size/4 );
    // make corrections to older minIDs
    int ifLess = false;
    
    for ( int i = 1; i <= tab.size / 4; i++ ) {
      if (minID[i] < i ) ifLess = true;
    }
    if ( ifLess ) setMinID();  // Fix for early, unused entries
  }
  else {
    minID.resize(header->NumOfElements() + 1);
    for ( int i = 1; i <= header->NumOfElements(); i++ ) {
      minID[i] = i;
    }
  }
  

  //***** Sort out subgroup stuff
      segments.resize(header->NumOfElements() + 1);
  segments[0] = new Segment();
  for (int ii = 1; ii <= header->NDet(); ii++ ) {
    for ( int segY = 0; segY < numSegs1[ii]; segY ++ ) {
      for ( int segX = 0; segX < numSegs2[ii]; segX ++ ) {
	int segID = minID[ii] + segX + segY * numSegs2[ii];
	segments[segID] = 
	  new Segment( ii, segY +1 , segX + 1, 
		       detectorLength[ii]/numSegs1[ii],
		       detectorWidth[ii]/numSegs2[ii],
		       detectorDepth[ii],
		       detectorEfficiency[ii],
		       segID
		       );
	
      }
    }
  }
  tab = header->GetDetSGMapTable();
  inFile->seekg(tab.location );
  vector<vector<int> >  IDMap; 
  IDMap.resize(header->NumOfHistograms() );
  for (int ii = 0; ii < header->NumOfHistograms(); ii++) {
    IDMap[ii].resize(header->NumOfElements());
  }
  subgroupIDList.resize(header->NumOfElements() * header->NumOfHistograms() +1);
  
  minSubgroupID.resize(header->NumOfHistograms() + 1);
  maxSubgroupID.resize(header->NumOfHistograms() + 1);
  int i;
  for ( i = 0; i < header->NumOfHistograms(); i++ ) {
    for (int jj = 0; jj < header->NumOfElements(); jj++ ) {
      IDMap[i][jj] = runFile->readRunInt();
      subgroupIDList[(i)* header->NumOfElements() + jj]  = IDMap[i][jj];
      if ( IDMap[i][jj] > maxSubgroupID[i + 1]){
	maxSubgroupID[i + 1] = IDMap[i][jj];
      }
      if ( (IDMap[i][jj] < minSubgroupID[i + 1] 
	    || minSubgroupID[i + 1] == 0)
	   && (IDMap[i][jj] > 0)  ) {
	minSubgroupID[i+1] = IDMap[i][jj];
      }
    }
  }
  subgroupMap.resize(maxSubgroupID[header->NumOfHistograms()]+ 1);
  segmentMap.resize(maxSubgroupID[header->NumOfHistograms()]+ 1);
  vector<int> id_count(maxSubgroupID[header->NumOfHistograms()]+1);
  
  for ( uint id = 0; id < id_count.size(); id++ )
    id_count[id] = 0;                   // zero out all of the counters
  
  // step across the IDMap once
  // incrementing the counters
  for ( int jj = 0; jj < header->NumOfHistograms(); jj++ )
    for ( int kk = 0; kk < header->NumOfElements(); kk++ )
      {
	i = IDMap[jj][kk];
	if ( i > 0 )
	  id_count[i]++;
      }
  // now allocate proper size arrays
  for ( i = 1 ; i <= maxSubgroupID[header->NumOfHistograms()]; i++ )
    {
      subgroupMap[i].resize( id_count[i] );
      segmentMap[i].resize( id_count[i] );
    }
  // set up a list of next free index
  // for each id and start index at 0
  vector<int> next_index( id_count.size() );
  for ( uint id = 0; id < next_index.size(); id++ )
    next_index[id] = 0;
  // step across the array once more
  // adding the group_id and segment
  // to the right list
  for ( int jj = 0; jj < header->NumOfHistograms(); jj++ )
    for ( int kk = 0; kk < header->NumOfElements(); kk++ )
      {
	i = IDMap[jj][kk];
	if ( i > 0 )                     // save in next free spot in list
	  {
	    subgroupMap[i][ next_index[i] ] = kk + 1;
	    segmentMap [i][ next_index[i] ] = segments[ kk + 1 ];
	    next_index[i]++;
	  }
      }
  
  tab = header->GetTimeScaleTable();
  inFile->seekg(tab.location);
  timeScale.resize(tab.size/4 + 1);
  runFile->readRunFloatVector(&timeScale, tab.size/4);

  
}


void Runfile::setMinID() {
  int lastRealID = 1;
  minID[1] = 1;
  if ( header->NDet() > 1 ) {
    for (int id = 2; id <= header->NDet(); id++ ) {
      if ( detectorType[id] != 0 ) {
	if ( detectorType[lastRealID] !=0 ) {
	  minID[id] = minID[lastRealID] + 
	    numSegs1[lastRealID] * numSegs2[lastRealID];
	}
	else {
	  minID[id] = minID[lastRealID] + 1;
	}
		    lastRealID = id;
      }
      else { 
	minID[id] = minID[lastRealID] + 1;
	lastRealID = id;
      }
    }
  }
}
    
/**
   Retrieves the spectrum of a detector segment.  This method opens and 
   closes the file on each call.
   @param seg Segment to be retrieved.
   @param hist Histogram number for data to retrieved.
   @return The retrieved spectrum.
*/
vector<float> Runfile::Get1DSpectrum(Segment seg, int hist) {
  int numOfTimeChannels;
  int i;
  vector<float> data;
  char *bdata;
  int tfType;
  int index, offset;
  int wordLength;
  
  /*  if ( leaveOpen == false){
    runfile = new RandomAccessRunfile(runfileName, "r");
    }*/
  int detID = seg.DetID();
  if ( !((psdOrder[detID] == 2) && (header->VersionNumber() < 5 )) ) {
    index = seg.SegID() + (hist-1) * header->NDet();
    if (detectorMap[index]->GetTFType() == 0 ) {
      cout << "invalid id in Get1DSpectrum(id,hist), " <<
			  "returning null\n";
      //      return NULL;
      throw  invalid_argument("invalid id in get1DSpectrum(id, hist)");

    }
    tfType = detectorMap[index]->GetTFType();
    numOfTimeChannels = timeField[tfType]->GetNumOfChannels();
    //add one element since start readRunFloatVectorFromXXXX has empty first
    //element
    data.resize(numOfTimeChannels + 1);
    // calculate start of data.
    if (header->NumOfWavelengths() > 0) {
      offset = detectorMap[index]->GetAddress() 
	+ header->TotalChannels() * 2 
	+ header->HistStartAddress() + 4;
    }
    else {
      offset = detectorMap[index]->GetAddress() + 
	header->HistStartAddress() + 4;
    }
    
    runFile->seekg(offset);
    if (numOfTimeChannels !=0){
      
      if (header->VersionNumber() < 4){
	runFile->readRunFloatVectorFromShort( &data, numOfTimeChannels);
      }
      else{
	runFile->readRunFloatVectorFromInt( &data, numOfTimeChannels);
      }
      // clear out empty first element
      vector<float>::iterator dataStart = data.begin();
      data.erase(dataStart);
    }
    /*    if (!leaveOpen ){
      runfile.close();
      }*/
    return data; 
  }
  else {                        // Area detector data
    float minWave = header->MinWavelength();
    float maxWave = header->MaxWavelength();
    int numWaves = header->NumOfWavelengths();
    float stepWave = (maxWave - minWave)/numWaves;
    int areaStartAddress;
    int sliceInterval;
    int aindex;
    
    areaStartAddress = header->HistStartAddress();
    sliceInterval = (header->TotalChannels()*2)/(numWaves);
    aindex = seg.Column()  + (seg.Row() -1) * header->NumOfX();
    
    int ioffset = areaStartAddress  + 2 + aindex*2;
    data.resize(numWaves);
    bdata = new char[2];
    int nbytes;
    int xx;
    int minSearchChan=1;
    int maxSearchChan=header->NumOfOverflows();
    for ( int ii = 0; ii < numWaves; ii++ ) {
      runFile->seekg( ioffset + ii * sliceInterval );
      int ovOffset = ioffset + ii * sliceInterval - header->HistStartAddress();
      data[ii] = (float)runFile->readRunShort();
      
      if (header->NumOfOverflows() > 0) {
	char sdone = false;
	for(int jj = minSearchChan; jj<=maxSearchChan&&!sdone; jj++) {
	  if ( ovOffset == overflows[jj] ) {
	    
	    data[ii] = data[ii] + 65536;
	  }
	  else if( ovOffset < overflows[jj] ) {
	    sdone = true;
	    minSearchChan = jj;
	  }
	}
      }
    }
    /*    if (!leaveOpen ){
      runfile.close();
      }*/
    return data;
  }
}

//
//
vector<Segment *>  Runfile::GetSegments() {
  return segments;
}
