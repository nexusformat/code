#ifndef IPNS_RUNFILE_TIMEFIELD_H
#define IPNS_RUNFILE_TIMEFIELD_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Header.h"
using namespace std;

class TimeField {
 public:
  TimeField(ifstream *, int, Header);
  TimeField();
  float GetTMin();
  float GetTMax();
  float GetTStep();
  int GetTDoubleLength();
  int GetNumOfChannels();
  short GetTimeFocusBit();
  short GetEmissionDelayBit();
  short GetConstantDelayBit();
  short GetEnergyBinBit();
  short GetWavelengthBinBit();
  short GetPulseHeightBinBit();
  short GetLogBinBit();
  short IsUsed();

 private:
  float tMin;
  float tMax;
  float tStep;
  int tDoubleLength;
  short numOfChannels;
  short timeFocusBit;
  short emissionDelayBit;
  short constantDelayBit;
  short energyBinBit;
  short wavelengthBinBit;
  short pulseHeightBit;
  short logBinBit;
  short hBit;
  short used;
  char *iName;
  int versionNumber;
};


#endif
