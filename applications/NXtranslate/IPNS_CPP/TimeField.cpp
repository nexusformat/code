#include "TimeField.h"
#include "RandomAccessRunfile.h"

TimeField::TimeField(){
  tMin= 500.0;
  tMax = 30000.0;
  tStep = 10.0;
  tDoubleLength = 32768;
  emissionDelayBit = 0;
  constantDelayBit = 0;
  used = false;
}

//Constructor
TimeField::TimeField(ifstream *inFile, int iType, Header header) {
  int startPos = (*inFile).tellg();
  unsigned int maskChanWord, minWord, rangeWord, widthWord;//, doubleWord;
  iName = new char[4];
  header.IName( iName );
  versionNumber = header.VersionNumber();
  (*inFile).seekg(header.getTimeFieldTableLoc() +
		  (iType - 1)*16);
  RandomAccessRunfile runFile = RandomAccessRunfile(inFile);
  if ( versionNumber <= 4 ) {
    maskChanWord = runFile.readRunUnsignedInt();
    minWord = runFile.readRunUnsignedInt();
    rangeWord = runFile.readRunUnsignedInt();
    widthWord = runFile.readRunUnsignedInt();
    if ( (strcmp( iName, "glad" ) == 0) ||
	 (strcmp( iName, "glad" ) == 0) ) {
      tMin = (float)minWord*header.LpsdClock();
      tMax = (float)rangeWord*header.LpsdClock();
      tStep = (float)(widthWord & 0xFFFF)*header.LpsdClock();
    }
    else {  
      tMin = (float)minWord*header.StandardClock();
      tMax = (float)rangeWord*header.StandardClock();
      tStep = (float)(widthWord & 0xFFFF)*header.StandardClock();
    }
    tDoubleLength =  (int)(widthWord & 0xffff0000) >> 16;
    numOfChannels = (short)(maskChanWord & 0xffff);
    timeFocusBit = (short)((maskChanWord >> 31 ) &1);
    emissionDelayBit = (short)((maskChanWord >> 30 ) &1);
    constantDelayBit = (short)((maskChanWord >> 29 ) &1);
  }
  else {
    maskChanWord = runFile.readRunInt();
    tMin = (double)runFile.readRunFloat();
    tMax = (double)runFile.readRunFloat();
    tStep = (double)runFile.readRunFloat();
    numOfChannels = (short)(maskChanWord & 0xffff);
    timeFocusBit = (short)((maskChanWord >> 31 ) &1);
    emissionDelayBit = (short)((maskChanWord >> 30 ) &1);
    constantDelayBit = (short)((maskChanWord >> 29 ) &1);
    energyBinBit = (short)((maskChanWord >> 28 ) &1);
    wavelengthBinBit = (short)((maskChanWord >> 27 ) &1);
    pulseHeightBit = (short)((maskChanWord >> 26 ) &1);
    logBinBit = (short)((maskChanWord >> 25 ) &1);
  }
  (*inFile).seekg(startPos);
}

// return tMin for this Time Field
float TimeField::GetTMin(){
  return tMin;
}

// return tMax for this Time Field
float TimeField::GetTMax(){
  return tMax;
}

// return tStep for this Time Field
float TimeField::GetTStep(){
  return tStep;
}

// return number of channels until channel doubling
int TimeField::GetTDoubleLength(){
  return tDoubleLength;
}

// return number of Channels
int TimeField::GetNumOfChannels(){
  if ( logBinBit != 0 ) {
    return (int)(( 1/tStep) * log( (double)(tMax/tMin))+2 );
  }
  else {
    return (int)((tMax-tMin)/tStep);
  }
  return numOfChannels;
}

//return value of the TimeFocusing Bit
short TimeField::GetTimeFocusBit(){
  return timeFocusBit;
}

// return value of Emmision Delay Bit
short TimeField::GetEmissionDelayBit(){
  return emissionDelayBit;
}

//return constant Delay Bit
short TimeField::GetConstantDelayBit(){
  return constantDelayBit;
}

// return energyBinBit
short TimeField::GetEnergyBinBit(){
  return energyBinBit;
}

// return wavelength Bin Bit
short TimeField::GetWavelengthBinBit(){
  return wavelengthBinBit;
}

// return pulse height Bin Bit
short TimeField::GetPulseHeightBinBit(){
  return pulseHeightBit;
}

//return logBinBit
short TimeField::GetLogBinBit(){
  return logBinBit;
}


short TimeField::IsUsed(){
  return used;
}
