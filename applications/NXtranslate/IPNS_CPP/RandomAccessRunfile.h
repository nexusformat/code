#ifndef RAND_ACC_RUNFILE_H
#define RAND_ACC_RUNFILE_H
#include <iostream>
#include <fstream>
#include <vector>

using std::vector;
using namespace std;

class RandomAccessRunfile {
 public:
  RandomAccessRunfile();
  RandomAccessRunfile(ifstream *inFile);
  int getVersion();
  int readRunInt();
  int readRunUnsignedInt();
  short readRunShort();
  float readRunFloat();
  void readRunString(char [], int);
  int readRunFileNum();
  void readRunIntArray(int *, int);
  void readRunShortArray(short *, int);
  void readRunFloatArray(float *, int);
  void readRunIntVector(vector<int> * , int);
  void readRunFloatVector(vector<float> *, int);
  void readRunShortVector(vector<short> *, int);
  void readRunFloatVectorFromShort(vector<float> *, int);
  void readRunFloatVectorFromInt(vector<float> *, int);
  istream& seekg( streampos );
 private:
  int version;
  ifstream *input;
};
#endif
