#include "Header.h"
#include "RandomAccessRunfile.h"
#include "LpsdDetIdMap.h"

Header::Header(){}

// set up constructor
Header::Header(ifstream *inFile, char *iNameIn) {
  RandomAccessRunfile runFile(inFile);
  //  cout << "HEADER: " << iNameIn << "\n";
  (*inFile).seekg(0);
  controlParameter.location = runFile.readRunInt();
  controlParameter.size = runFile.readRunInt();
  detectorMapTable.location = runFile.readRunInt();
  detectorMapTable.size = runFile.readRunInt();
  timeFieldTable.location = runFile.readRunInt();
  timeFieldTable.size = runFile.readRunInt();
  timeScaleTable.location = runFile.readRunInt();
  timeScaleTable.size = runFile.readRunInt();
  timeShiftTable.location = runFile.readRunInt();
  timeShiftTable.size = runFile.readRunInt();
  areaStartTable.location = runFile.readRunInt();
  areaStartTable.size = runFile.readRunInt();
  timeDelayTable.location = runFile.readRunInt();
  timeDelayTable.size = runFile.readRunInt();
  histStartAddress = runFile.readRunInt();
  numOfBlocks = runFile.readRunInt();
  offsetToFree = runFile.readRunInt();
  versionNumber = runFile.readRunInt();
  detectorAngle.location = runFile.readRunInt();
  detectorAngle.size = runFile.readRunInt();
  flightPath.location = runFile.readRunInt();
  flightPath.size = runFile.readRunInt();
  detectorHeight.location = runFile.readRunInt();
  detectorHeight.size = runFile.readRunInt();
  detectorType.location = runFile.readRunInt();
  detectorType.size = runFile.readRunInt();
  controlTable.location = runFile.readRunInt();
  controlTable.size = runFile.readRunInt();
  seqHistWidth.location = runFile.readRunInt();
  seqHistWidth.size = runFile.readRunInt();
  nDet = runFile.readRunShort();
  runFile.readRunString(userName, 20);
  userName[20] = '\0';
  runFile.readRunString(runTitle, 80 );
  runTitle[80] = '\0';
  runNum = runFile.readRunFileNum();
  nextRun = runFile.readRunFileNum();
  runFile.readRunString(startDate, 9);
  startDate[9] = '\0';
  runFile.readRunString(startTime, 8);
  startTime[8] = '\0';
  runFile.readRunString(endDate, 9);
  endDate[9] = '\0';
  runFile.readRunString(endTime, 8);
  endTime[8] = '\0';
  runFile.readRunString(protStatus, 1);
  protStatus[1] = '\0';
  runFile.readRunString(varToMonitor, 1);
  varToMonitor[1] = '\0';
  presetMonitorCounts = runFile.readRunInt();
  elapsedMonitorCounts = runFile.readRunInt();
  numOfCyclesPreset = runFile.readRunShort();
  numOfCyclesCompleted = runFile.readRunShort();
  runAfterFinished = runFile.readRunFileNum();
  totalMonitorCounts = runFile.readRunInt();
  detCalibFile = runFile.readRunFileNum();
  runFile.readRunString(detLocUnit, 1);
  detLocUnit[1] = '\0';
  runFile.readRunString(pseudoTimeUnit, 1);
  pseudoTimeUnit[1] = '\0';
  sourceToSample = runFile.readRunFloat();
  sourceToChopper = runFile.readRunFloat();
  moderatorCalibFile = runFile.readRunFileNum();
  groupToMonitor = runFile.readRunShort();
  channelToMonitor = runFile.readRunShort();
  numOfHistograms = runFile.readRunShort();
  numOfTimeFields = runFile.readRunShort();
  numOfControl = runFile.readRunShort();
  controlFlag = runFile.readRunShort();
  clockShift = runFile.readRunShort();
  totalChannels = runFile.readRunInt();
  numOfPulses = runFile.readRunInt();
  sizeOfDataArea = runFile.readRunInt();
  hardwareTMin = runFile.readRunInt();
  hardwareTMax = runFile.readRunInt();
  hardTimeDelay = runFile.readRunInt();
  numOfX = runFile.readRunShort();
  numOfY = runFile.readRunShort();
  numOfWavelengths = runFile.readRunShort();
  maxWavelength = runFile.readRunInt();
  minWavelength = runFile.readRunInt();
  dta = runFile.readRunFloat();
  dtd = runFile.readRunFloat();
  omega = runFile.readRunFloat();
  chi = runFile.readRunFloat();
  phi = runFile.readRunFloat();
  xLeft = runFile.readRunFloat();
  xRight = runFile.readRunFloat();
  yLower = runFile.readRunFloat();
  yUpper = runFile.readRunFloat();
  xDisplacement = runFile.readRunFloat();
  yDisplacement = runFile.readRunFloat();
  xLength = runFile.readRunFloat();
  areaChannelWidth = runFile.readRunShort();
  areaDoubleInterval = runFile.readRunShort();
  addressOf1DData = runFile.readRunInt();
  addressOf2DData = runFile.readRunInt();
  endOfOverflows = runFile.readRunInt();
  channels1D = runFile.readRunInt();
  numOfOverflows = runFile.readRunShort();
  clockPeriod = runFile.readRunFloat();
  (*inFile).seekg(462);
  energyIn = runFile.readRunFloat();
  energyOut = runFile.readRunFloat();
  numOfSeqHist = runFile.readRunShort();
  protonCurrent = runFile.readRunFloat();
  areaBinning = runFile.readRunShort();
  microproccessor = runFile.readRunShort();
  numOfLockouts = runFile.readRunShort();
  firstOverflow = runFile.readRunInt();
  expNum = runFile.readRunFileNum();
  firstRun = runFile.readRunFileNum();
  lastRun = runFile.readRunFileNum();
  defaultRun = runFile.readRunFileNum();
  samplePos = runFile.readRunShort();
  numOfHeadBlocks = runFile.readRunShort();
  overflowSort = runFile.readRunShort();
  standardClock = runFile.readRunFloat();
  lpsdClock = runFile.readRunFloat();
  (*inFile).seekg(512);
  messageRegion.location = runFile.readRunInt();
  messageRegion.size = runFile.readRunInt();
  discSettings.location = runFile.readRunInt();
  discSettings.size = runFile.readRunInt();
  PSD_IDMap.location = runFile.readRunInt();
  PSD_IDMap.size = runFile.readRunInt();
  if ( versionNumber < 5 ) {
    //    cout << "VersionNumber " << versionNumber << "\n";
    int ii;
    for( ii = 0; ii < 5; ii++ ) {
      iName[ii] = iNameIn[ii];
    }
    //    cout << "iName, iNameIn: " << iName << ", " << iNameIn << "\n";
    detectorLength.location = 0;
    detectorLength.size = 0;
    detectorWidth.location = 0;
    detectorWidth.size = 0;
    detectorDepth.location = 0;
    detectorDepth.size = 0;
    detectorSGMap.location = 0;
    detectorSGMap.size = 0;
    detCoordSys.location = 0;
    detCoordSys.size = 0;
    detectorRot1.location = 0;
    detectorRot1.size = 0;
    detectorRot2.location = 0;
    detectorRot2.size = 0;
    detectorEfficiency.location = 0;
    detectorEfficiency.size = 0;
    psdOrder.location = 0;
    psdOrder.size = 0;
    numSegs1.location = 0;
    numSegs1.size = 0;
    numSegs2.location = 0;
    numSegs2.size = 0;
    crateNum.location = 0;
    crateNum.size = 0;
    slotNum.location = 0;
    slotNum.size = 0;
    inputNum.location = 0;
    inputNum.size = 0;
    dataSource.location = 0;
    dataSource.size = 0;
    minID.location = 0;
    minID.size = 0;
    instrumentType = 0;
    filterType = 0;
    sampleEnv = 0;
    detectorConfig = 0;
    runType = 0;
    if ( strcmp( iName, "glad") == 0|| strcmp( iName, "lpsd")==0){
//      int nLpsdCrates = nDet/8192 + 1;
      numOfElements = nDet;
      LpsdDetIdMap lpsdDetIdMap( inFile, this );
      int numDets = 0;
      int* nDiB = new int[lpsdDetIdMap.NumOfBanks()];
      lpsdDetIdMap.NumDetsInBank(nDiB);
      
      for ( ii =0; ii<= lpsdDetIdMap.NumOfBanks(); ii++) {
	//cout<< nDiB[ii] << "\n";
	numDets += nDiB[ii];
      }
      nDet = numDets;
    }
    else if ( strcmp( iName, "sad0") == 0 ||
 	      strcmp( iName, "sad1") == 0 ||
 	      strcmp( iName, "posy") == 0 ||
 	      strcmp( iName, "pne0") == 0 ||
 	      strcmp( iName, "sand") == 0 ) {
      numOfElements = nDet + numOfX * numOfY;
      nDet += 1;
    }
    else if ( strcmp( iName, "scd0") == 0) {
      numOfElements = nDet + 85 * 85;
      nDet += 1;
      dta = -dta;
    }
    else {
      numOfElements = nDet;
    }
    if ( versionNumber < 4 ) {
      standardClock = 0.125;
      lpsdClock = 0.5;
    }
    else {
      (*inFile).seekg(632);
      standardClock = runFile.readRunFloat();
      lpsdClock = runFile.readRunFloat();
      
    }
  }
  else {
    (*inFile).seekg(632);
    standardClock = runFile.readRunFloat();
    lpsdClock = runFile.readRunFloat();
    numOfElements = runFile.readRunInt(); 
    if (numOfElements == 0 ) {
      numOfElements = nDet;
    }
    (*inFile).seekg(700);
    detectorLength.location = runFile.readRunInt();
    detectorLength.size = runFile.readRunInt();
    detectorWidth.location = runFile.readRunInt();
    detectorWidth.size = runFile.readRunInt();
    detectorDepth.location = runFile.readRunInt();
    detectorDepth.size = runFile.readRunInt();
    
    runFile.readRunString(iName, 4);
    iName[4] = '\0';
    detectorSGMap.location = runFile.readRunInt();
    detectorSGMap.size = runFile.readRunInt();
    detCoordSys.location = runFile.readRunInt();
    detCoordSys.size = runFile.readRunInt();
    detectorRot1.location = runFile.readRunInt();
    detectorRot1.size = runFile.readRunInt();
    detectorRot2.location = runFile.readRunInt();
    detectorRot2.size = runFile.readRunInt();
    detectorEfficiency.location = runFile.readRunInt();
    detectorEfficiency.size = runFile.readRunInt();
    psdOrder.location = runFile.readRunInt();
    psdOrder.size = runFile.readRunInt();
    numSegs1.location = runFile.readRunInt();
    numSegs1.size = runFile.readRunInt();
    numSegs2.location = runFile.readRunInt();
    numSegs2.size = runFile.readRunInt();
    crateNum.location = runFile.readRunInt();
    crateNum.size = runFile.readRunInt();
    slotNum.location = runFile.readRunInt();
    slotNum.size = runFile.readRunInt();
    inputNum.location = runFile.readRunInt();
    inputNum.size = runFile.readRunInt();
    dataSource.location = runFile.readRunInt();
    dataSource.size = runFile.readRunInt();
    minID.location = runFile.readRunInt();
    minID.size = runFile.readRunInt();
    if (versionNumber >= 6) {
      instrumentType = runFile.readRunInt();
      filterType = runFile.readRunInt();
      sampleEnv = runFile.readRunInt();
      detectorConfig = runFile.readRunInt();
      runType = runFile.readRunInt();
    }
    else {
      instrumentType = 0;
      filterType = 0;
      sampleEnv = 0;
      detectorConfig = 0;
      runType = 0;
    }
  }    
  
#ifdef IPNS_HEADER_DEBUG
  cout << "control Parameter (location, size): (" << controlParameter.location 
       << ", " << controlParameter.size << ")\n";
  cout << "detectorMapTable (location, size): (" << detectorMapTable.location 
       << ", " << detectorMapTable.size << ")\n";
  cout << "timeFieldTable(location, size): (" << timeFieldTable.location << 
    ", " << timeFieldTable.size << ")\n";
  cout << "timeScaleTable(location, size): (" << timeScaleTable.location << 
    ", " << timeScaleTable.size << ")\n";
  cout << "timeShiftTable(location, size): (" << timeShiftTable.location << 
    ", " << timeShiftTable.size << ")\n";
  cout << "areaStartTable(location, size): (" << areaStartTable.location << 
    ", " << areaStartTable.size << ")\n";
  cout << "timeDelayTable(location, size): (" << timeDelayTable.location << 
    ", " << timeDelayTable.size << ")\n";
  cout << "histStartAddress: " << histStartAddress << "\n";
  cout << "numOfBlocks: " << numOfBlocks << "\n";
  cout << "offsetToFree: " << offsetToFree << "\n";
  cout << "versionNumber: " << versionNumber << "\n";
  cout << "detectorAngle(location, size): (" << detectorAngle.location << 
    ", " << detectorAngle.size << ")\n";
  cout << "flightPath(location, size): (" << flightPath.location << 
    ", " << flightPath.size << ")\n";
  cout << "detectorHeight(location, size): (" << detectorHeight.location << 
    ", " << detectorHeight.size << ")\n";
  cout << "detectorType(location, size): (" << detectorType.location << 
    ", " << detectorType.size << ")\n";
  cout << "controlTable(location, size): (" << controlTable.location << 
    ", " << controlTable.size << ")\n";
  cout << "seqHistWidth(location, size): (" << seqHistWidth.location << 
    ", " << seqHistWidth.size << ")\n";
  cout << "nDet: " << nDet << "\n";
  cout << "userName: " << userName << "\n";
  cout << "runTitle: " << runTitle << "\n";
  cout << "runNum: " << runNum << "\n";
  cout << "nextRun: " << nextRun << "\n";
  cout << "startDate: " << startDate << "\n";
  cout << "startTime: " << startTime << "\n";
  cout << "endDate: " << endDate << "\n";
  cout << "endTime: " << endTime << "\n";
  cout << "protStatus: " << protStatus << "\n";
  cout << "varToMonitor: " << varToMonitor << "\n";
  cout << "presetMonitorCounts: " << presetMonitorCounts << "\n";
  cout << "elapsedMonitorCounts: " << elapsedMonitorCounts << "\n";
  cout << "numOfCyclesPreset: " << numOfCyclesPreset << "\n";
  cout << "numOfCyclesCompleted: " << numOfCyclesCompleted << "\n";
  cout << "runAfterFinished: " << runAfterFinished << "\n";
  cout << "totalMonitorCounts: " << totalMonitorCounts << "\n";
  cout << "detCalibFile: " << detCalibFile << "\n";
  cout << "detLocUnit: " << detLocUnit << "\n";
  cout << "pseudoTimeUnit: " << pseudoTimeUnit << "\n";
  cout << "sourceToSample: " << sourceToSample << "\n";
  cout << "sourceToChopper: " << sourceToChopper << "\n";
  cout << "moderatorCalibFile: " << moderatorCalibFile << "\n";
  cout << "groupToMonitor: " << groupToMonitor << "\n";
  cout << "channelToMonitor: " << channelToMonitor << "\n";
  cout << "numOfHistograms: " << numOfHistograms << "\n";
  cout << "numOfTimeFields: " << numOfTimeFields << "\n";
  cout << "numOfControl: " << numOfControl << "\n";
  cout << "controlFlag: " << controlFlag << "\n";
  cout << "clockShift: " << clockShift << "\n";
  cout << "totalChannels: " << totalChannels << "\n";
  cout << "numOfPulses: " << numOfPulses << "\n";
  cout << "sizeOfDataArea: " << sizeOfDataArea << "\n";
  cout << "hardwareTMin: " << hardwareTMin << "\n";
  cout << "hardwareTMax: " << hardwareTMax << "\n";
  cout << "hardTimeDelay: " << hardTimeDelay << "\n";
  cout << "numOfX: " << numOfX << "\n";
  cout << "numOfY: " << numOfY << "\n";
  cout << "numOfWavelengths: " << numOfWavelengths << "\n";
  cout << "maxWavelength: " << maxWavelength << "\n";
  cout << "minWavelength: " << minWavelength << "\n";
  cout << "dta: " << dta << "\n";
  cout << "dtd: " << dtd << "\n";
  cout << "omega: " << omega << "\n";
  cout << "chi: " << chi << "\n";
  cout << "phi: " << phi << "\n";
  cout << "xLeft: " << xLeft << "\n";
  cout << "xRight: " << xRight << "\n";
  cout << "yLower: " << yLower << "\n";
  cout << "yUpper: " << yUpper << "\n";
  cout << "xDisplacement: " << xDisplacement << "\n";
  cout << "yDisplacement: " << yDisplacement << "\n";
  cout << "xLength: " << xLength << "\n";
  cout << "areaChannelWidth: " << areaChannelWidth << "\n";
  cout << "areaDoubleInterval: " << areaDoubleInterval << "\n";
  cout << "addressOf1DData: " << addressOf1DData << "\n";
  cout << "addressOf2DData: " << addressOf2DData << "\n";
  cout << "endOfOverflows: " << endOfOverflows << "\n";
  cout << "channels1D: " << channels1D << "\n";
  cout << "numOfOverflows: " << numOfOverflows << "\n";
  cout << "clockPeriod: " << clockPeriod << "\n";
  cout << "energyIn: " << energyIn << "\n";
  cout << "energyOut: " << energyOut << "\n";
  cout << "numOfSeqHist: " << numOfSeqHist << "\n";
  cout << "protonCurrent: " << protonCurrent << "\n";
  cout << "areaBinning: " << areaBinning << "\n";
  cout << "microproccessor: " << microproccessor << "\n";
  cout << "numOfLockouts: " << numOfLockouts << "\n";
  cout << "firstOverflow: " << firstOverflow << "\n";
  cout << "expNum: " << expNum << "\n";
  cout << "firstRun: " << firstRun << "\n";
  cout << "lastRun: " << lastRun << "\n";
  cout << "defaultRun: " << defaultRun << "\n";
  cout << "samplePos: " << samplePos << "\n";
  cout << "numOfHeaderBlocks: " << numOfHeadBlocks << "\n";
  cout << "overflowSort: " << overflowSort << "\n";
  cout << "messageRegion(location, size): (" << messageRegion.location << 
    ", " << messageRegion.size << ")\n";
  cout << "discSettings(location, size): (" << discSettings.location << 
    ", " << discSettings.size << ")\n";
  cout << "PSD_IDMap(location, size): (" << PSD_IDMap.location << 
    ", " << PSD_IDMap.size << ")\n";
  cout << "standardClock: " << standardClock << "\n";
  cout << "lpsdClock: " << lpsdClock << "\n";
  cout << "numOfElements: " << numOfElements << "\n";
  cout << "detectorLength(location, size): (" << detectorLength.location << 
    ", " << detectorLength.size << ")\n";
  cout << "detectorWidth(location, size): (" << detectorWidth.location << 
    ", " << detectorWidth.size << ")\n";
  cout << "detectorDepth(location, size): (" << detectorDepth.location << 
    ", " << detectorDepth.size << ")\n";

  cout << "iName: " << iName << "\n";
  cout << "magicNumber: " << magicNumber << "\n";


  
#endif  
}

//Routine to get start address for histogram data
int Header::HistStartAddress() {
  return histStartAddress;
}

//Return number of blocks in the file
int Header::NumOfBlocks() {
  return numOfBlocks;
}

//Return number of bytes to free space (end of file)
int Header::OffsetToFree() {
  return offsetToFree;
}

//Return version number of runfile
int Header::VersionNumber() {
  return versionNumber;
}

//Return number of detectors
short Header::NDet() {
  return nDet;
}

// Return user name
void Header::UserName(char *user) {
  strcpy(user, userName);
}

// Return Run title
void Header::RunTitle(char *title) {
  strcpy(title, runTitle);
}

// Return Run Number stored in the file 
int Header::RunNum() {
  return runNum;
}

// Return the run number of the run to be started after this one
int Header::NextRun() {
  return nextRun;
}

// Return start date for this run
void Header::StartDate(char *date) {
  strcpy(date, startDate);
}

// Return start time for this run
void Header::StartTime(char *time) {
  strcpy(time, startTime);
}

// Return end date for this run
void Header::EndDate(char *date) {
  strcpy(date, endDate);
}

// Return end time for this run
void Header::EndTime(char *time) {
  strcpy(time, endTime);
}

// Return protection status for this file
void Header::ProtStatus(char *stat) {
  strcpy(stat, protStatus);
}

//Return variable to monitor
void Header::VarToMonitor(char *var) {
  strcpy(var, varToMonitor);
}

// return preset monitor counts
int Header::PresetMonitorCounts() {
  return presetMonitorCounts;
}

// return elapsed monitor counts
int Header::ElapsedMonitorCounts() {
  return elapsedMonitorCounts;
}

// return number of preset cycles
int Header::NumOfCyclesPreset() {
  return numOfCyclesPreset;
}

// return number of completes cycles
int Header::NumOfCyclesCompleted() {
  return numOfCyclesCompleted;
}

//return run number to run after this one completes
int Header::RunAfterFinished() {
  return runAfterFinished;
}

// return total monitor counts
int Header::TotalMonitorCounts() {
  return totalMonitorCounts;
}

// return detector calibration file number
int Header::DetCalibFile() {
  return detCalibFile;
}

// return detector location unit 
void Header::DetLocUnit(char *unit) {
  strcpy(unit, detLocUnit);
}

//return pseudo time unit
void Header::PseudoTimeUnit(char *unit) {
  strcpy(unit, pseudoTimeUnit);
}

//return source to sample distance
float Header::SourceToSample() {
  return sourceToSample;
}

//return source to chopper
float Header::SourceToChopper() {
  return sourceToChopper;
}

//return moderator calibration file number
int Header::ModeratorCalibFile() {
  return moderatorCalibFile;
}

// return group to be monitored
short Header::GroupToMonitor(){
  return groupToMonitor;
}

//return Channel to monitor
short Header::ChannelToMonitor() {
  return channelToMonitor;
}

// return number of histograms
short Header::NumOfHistograms() {
  return numOfHistograms;
}

// return number of time fields
short Header::NumOfTimeFields() {
  return numOfTimeFields;
}

// return number of control parameters
short Header::NumOfControl() {
  return numOfControl;
}

// return control flag
short Header::ControlFlag(){
  return controlFlag;
}

// return clock shift
short Header::ClockShift() {
  return clockShift;
}

// return total channels in file
int Header::TotalChannels() {
  return totalChannels;
}

//return number of pulses
int Header::NumOfPulses() {
  return numOfPulses;
}

//return size of data area
int Header::SizeOfDataArea() {
  return sizeOfDataArea;
}

// return hardware t min
int Header::HardwareTMin() {
  return hardwareTMin;
}

// return hardware t max
int Header::HardwareTMax() {
  return hardwareTMax;
}

// return hardware time delay
int Header::HardTimeDelay() {
  return hardTimeDelay;
}

// return number of x channels for an area detector
short Header::NumOfX() {
  return numOfX;
}

// return number of y channels for an area detector
short Header::NumOfY() {
  return numOfY;
}

// return number of wavelength channels for an area detector
short Header::NumOfWavelengths() {
  return numOfWavelengths;
}

// return area detector angle
float Header::Dta() {
  return dta;
}

// return area detector distance
float Header::Dtd() {
  return dtd;
}

// return SCD Chi
float Header::Chi() {
  return chi;
}

// return SCD Phi
float Header::Phi() {
  return phi;
}

// return SCD Omega
float Header::Omega() {
  return omega;
}

// return X displacement of Left side of area detector
float Header::XLeft() {
  return xLeft;
}

// return X displacement of right side of area detector
float Header::XRight() {
  return xRight;
}

// return Y displacement of Lower edge of area detectors
float Header::YLower() {
  return yLower;
}

// return Y displacement of Upper edge of area detectors
float Header::YUpper() {
  return yUpper;
}

// return X displacement of detector center
float Header::XDisplacement() {
  return xDisplacement;
}

// return Y displacement of detector center
float Header::YDisplacement() {
  return yDisplacement;
}

// return X length for area detector
float Header::XLength() {
  return xLength;
}

//return channel width for area detectors
short Header::AreaChannelWidth() {
  return areaChannelWidth;
}

//return number of channels before doubling width for area detectors.
short Header::AreaDoubleInterval() {
  return areaDoubleInterval;
}

//return address of 1D data
int Header::AddressOf1DData() {
  return addressOf1DData;
}

//return address of 2D data
int Header::AddressOf2DData() {
  return addressOf2DData;
}

// return Addres for end of overflows
int Header::EndOfOverflow() {
  return endOfOverflows;
}

//return number of channels of 1D data
int Header::Channels1D() {
  return channels1D;
}

//return the number of overflows
short Header::NumOfOverflows() {
  return numOfOverflows;
}

//return clock period for area detectors
double Header::ClockPeriod() {
  return clockPeriod;
}

//return fixed energy in
double Header::EnergyIn() {
  return energyIn;
}

//return fixed energy out
double Header::EnergyOut() {
  return energyOut;
}

//return number of sequential histograms
short Header::NumOfSeqHist() {
  return numOfSeqHist;
}

//return Proton Current
double Header::ProtonCurrent() {
  return protonCurrent;
}

//return type of time data for area detectors
short Header::AreaBinning() {
  return areaBinning;
}

//return microprocessor type
short Header::Microprocessor() {
  return microproccessor;
}

//return number of lockouts
short Header::NumOfLockouts() {
  return numOfLockouts;
}

//return location of first overflow
int Header::FirstOverflow() {
  return firstOverflow;
}

//return experiment number
int Header::ExpNum() {
  return expNum;
}

//return first run in a sequence
int Header::FirstRun() {
  return firstRun;
}

//return last run in a sequence
int Header::LastRun() {
  return lastRun;
}

//return default run
int Header::DefaultRun() {
  return defaultRun;
}

//return sample position
short Header::SamplePos() {
  return samplePos;
}

//return number of blocks in the header
short Header::NumOfHeadBlocks() {
  return numOfHeadBlocks;
}

//return overflow sort
short Header::OverflowSort() {
  return overflowSort;
}

//return clock period for standard detectors
double Header::StandardClock() {
  return standardClock;
}

//return clock period for lpsd's 
double Header::LpsdClock() {
  return lpsdClock;
}

//return number of elements for all detectors
int Header::NumOfElements() {
  return numOfElements;
}

//return instrument name
void Header::IName(char *name) {
  strcpy(name, iName);
}

//return magic number
 int Header::MagicNumber() {
   return magicNumber;
 }

//return location/size of the LPSD ID Map
TableType Header::GetLpsdDetIDMap() {
  return PSD_IDMap;
}

// return location/size of the Time Field Table
TableType Header::GetTimeFieldTable() {
  return timeFieldTable;
}

// return location/size of the Detector Map Table
TableType Header::GetDetMapTable(){
  return detectorMapTable;
}

// return location/size of the Time Scale Table
TableType Header::GetTimeScaleTable() {
  return timeScaleTable;
}

// return location/size of the time Shift Table
TableType Header::GetTimeShiftTable(){
  return timeShiftTable;
}

// return location/size of the Time Delay Table
TableType Header::GetTimeDelayTable(){
  return timeDelayTable;
}

// return location/size of the Area Start Table
TableType Header::GetAreaStartTable(){
  return areaStartTable;
}


// return location/size of the Detector Angle Table
TableType Header::GetDetectorAngleTable(){
  return detectorAngle;
}

// return location/size of the Flight Path Length Table
TableType Header::GetFlightPathTable(){
  return flightPath;
}

// return location/size of the Detector Height Table
TableType Header::GetDetectorHeightTable(){
  return detectorHeight;
}

// return location/size of the Detector Type Table
TableType Header::GetDetectorTypeTable(){
  return detectorType;
}

// return location/size of the Control Parameter Table
TableType Header::GetControlTable(){
  return controlTable;
}

// return location/size of the Sequential Histogram Width Table
TableType Header::GetSeqHistWidthTable(){
  return seqHistWidth;
}

// return location/size of the Message region Table
TableType Header::GetMessageRegTable() {
  return messageRegion;
}

// return location/size of the Discriminator Region Table
TableType Header::GetDiscSetTable(){
  return discSettings;
}

// return location/size of the Detector Length Table
TableType Header::GetDetLengthTable(){
  return detectorLength;
}

// return location/size of the Detector Width Table
TableType Header::GetDetWidthTable(){
  return detectorWidth;
}

// return location/size of the Detector Depth Table
TableType Header::GetDetDepthTable(){
  return detectorDepth;
}

// return location/size of the Detector subgroup map Table
TableType Header::GetDetSGMapTable(){
  return detectorSGMap;
}

TableType Header::GetDetCoordSysTable(){
  return detCoordSys;
}
TableType Header::GetDetRot1Table(){
  return detectorRot1;
}

TableType Header::GetDetRot2Table(){
  return detectorRot2;
}

TableType Header::GetDetEffTable(){
  return detectorEfficiency;
}

TableType Header::GetPsdOrderTable(){
  return psdOrder;
}

TableType Header::GetNumSegs1Table(){
  return numSegs1;
}

TableType Header::GetNumSegs2Table(){
  return numSegs2;
}

TableType Header::GetCrateNumTable(){
  return crateNum;
}

TableType Header::GetSlotNumTable(){
  return slotNum;
}

TableType Header::GetInputNumTable(){
  return inputNum;
}

TableType Header::GetDataSourceTable(){
  return dataSource;
}

TableType Header::GetMinIDTable(){
  return minID;
}






//return location of the LPSD ID Map for GLAD
int Header::getLpsdDetIDMapLoc(){
  return PSD_IDMap.location;
}

//return size of LPSD ID Map for GLAD
int Header::getLpsdDetIDMapSize() {
  return PSD_IDMap.size;
}

//return location of TimeFieldTable
int Header::getTimeFieldTableLoc() {
  return timeFieldTable.location;
}

//return size of TimeFieldTable
int Header::getTimeFieldTableSize() {
  return timeFieldTable.size;
}

