#include "RandomAccessRunfile.h"

RandomAccessRunfile::RandomAccessRunfile(){}

//set up constructor
RandomAccessRunfile::RandomAccessRunfile(ifstream *inFile) {
  int curPos = (*inFile).tellg();
  input = inFile;
  (*inFile).seekg(68);
  (*inFile).read((char *)&version, sizeof (int));
  if (version > 16777215 ) {   // Version < 4 was little endian
    int byte1 = version & 0xff;
    int byte2 = (version & 0xff00)>>8;
    int byte3 = (version & 0xff0000)>>16;
    int byte4 = (version & 0xff000000)>>24;
    
    int zero = 0;
    int tfs = 256;
    if (byte1 < zero) byte1 += tfs;
    if (byte2 < zero) byte2 += tfs;
    if (byte3 < zero) byte3 += tfs;
    if (byte4 < zero) byte4 += tfs;
    int newvers = byte4 + (byte3 << 8) + (byte2 <<16) + (byte1 <<24);
    version = newvers;
    (*inFile).seekg(0);
  }
  
  else {
  }
  //  cout << "Version " << version << "\n";
  (*inFile).seekg(curPos);
}



//function to get file version number
int RandomAccessRunfile::getVersion() {
  return version;
}

//readRunInt - read an integer from a file
int RandomAccessRunfile::readRunInt() {
  int ret = 0;
#ifdef IPNS_BIG_ENDIAN
  if (version <= 3){
    int length = 4;
    int zero = 0;
    char b[4] = {0, 0, 0, 0};
    int c[4] = {0, 0, 0, 0};
    (*input).read((char *)b, length);
    int num = 0;
    int tfs = 256;
    c[0] = b[0];
    if (c[0] < zero) c[0] += tfs;
    num += c[0];
    c[1] = b[1];
    if (c[1] < zero) c[1] += tfs;
    num += (c[1] << 8);
    c[2] = b[2];
    if (c[2] < zero) c[2] += tfs;
    num += (c[2] << 16);
    c[3] = b[3];
    if (c[3] < zero) c[3] += tfs;
    num += (c[3] << 24);
    ret = num;
  }
  else {
    int num = 0;
    (*input).read((char *)&num, 4);
    ret = num;
  }
#endif
#ifdef IPNS_LITTLE_ENDIAN
  if (version <= 3){
    int num = 0;
    (*input).read((char *)&num, 4);
    ret = num;
  }
  else { 
    int length = 4;
    uint zero = 0;
    unsigned char b[4] = {0, 0, 0, 0};
    unsigned int c[4] = {0, 0, 0, 0};

    (*input).read((char *)&b[0], length);
    int num = 0;
    unsigned int tfs = 256;
    c[0] = b[0];
    c[1] = b[1];
    if (c[0] < zero) c[0] += tfs;
    if (c[1] < zero) c[1] += tfs;
    num += (c[0] << 24);
    num += (c[1] << 16);
    c[2] = b[2];
    c[3] = b[3];
    if (c[2] < zero) c[2] += tfs;
    if (c[3] < zero) c[3] += tfs;
    num += (c[2] << 8);
    num += (c[3]);
    ret = num;
}
#endif
  return ret;
}

//readRunInt - read an integer from a file
int RandomAccessRunfile::readRunUnsignedInt() {
  unsigned int ret = 0;
#ifdef IPNS_BIG_ENDIAN
  if (version <= 3){
    int length = 4;
    int zero = 0;
    unsigned char b[4] = {0, 0, 0, 0};
    unsigned int c[4] = {0, 0, 0, 0};
    (*input).read((char *)b, length);
    unsigned int num = 0;
    int tfs = 256;
    c[0] = b[0];
    if (c[0] < zero) c[0] += tfs;
    num += c[0];
    c[1] = b[1];
    if (c[1] < zero) c[1] += tfs;
    num += (c[1] << 8);
    c[2] = b[2];
    if (c[2] < zero) c[2] += tfs;
    num += (c[2] << 16);
    c[3] = b[3];
    if (c[3] < zero) c[3] += tfs;
    num += (c[3] << 24);
    ret = num;
  }
  else {
    unsigned int num = 0;
    (*input).read((char *)&num, 4);
    ret = num;
  }
#endif
#ifdef IPNS_LITTLE_ENDIAN
  if (version <= 3){
    unsigned int num = 0;
    (*input).read((char *)&num, 4);
    ret = num;
  }
  else { 
    int length = 4;
    uint zero = 0;
    unsigned char b[4] = {0, 0, 0, 0};
    unsigned int c[4] = {0, 0, 0, 0};

    (*input).read((char *)&b[0], length);
    unsigned int num = 0;
    unsigned int tfs = 256;
    c[0] = b[0];
    c[1] = b[1];
    if (c[0] < zero) c[0] += tfs;
    if (c[1] < zero) c[1] += tfs;
    num += (c[0] << 24);
    num += (c[1] << 16);
    c[2] = b[2];
    c[3] = b[3];
    if (c[2] < zero) c[2] += tfs;
    if (c[3] < zero) c[3] += tfs;
    num += (c[2] << 8);
    num += (c[3]);
    ret = num;
}
#endif
  return ret;
}

//readRunShort - read a short from a file
short RandomAccessRunfile::readRunShort() {
  short ret =0;
#ifdef IPNS_BIG_ENDIAN
    int length = 2;
    int zero = 0;
    char b[4] = {0, 0};
    int c[4] = {0, 0};
    (*input).read((char *)b, length);
    int num = 0;
    int tfs = 256;
    c[0] = b[0];
    if (c[0] < zero) c[0] += tfs;
    num += c[0] << 8;
    c[1] = b[1];
    if (c[1] < zero) c[1] += tfs;
    num += c[1];
    ret = num;
  }
  else {
    int num = 0;
    (*input).read((char *)&num, 2);
    ret = num;
  }
#endif
#ifdef IPNS_LITTLE_ENDIAN
  if (version <= 3){
    short num = 0;
    (*input).read((char *)&num, 2);
    ret = num;
  }
  else { 
    int length = 2;
    int zero = 0;
    char b[2] = {0, 0};
    int c[2] = {0, 0};
    (*input).read((char *)b, length);
    int num = 0;
    int tfs = 256;
    c[0] = b[0] << 8;
    if (c[0] < zero) c[0] += tfs;
    num += c[0];
    c[1] = b[1];
    if (c[1] < zero) c[1] += tfs;
    num += c[1];
    ret = num;
  }
#endif
  return ret;
}

//readRunFloat - read a float from a file
float RandomAccessRunfile::readRunFloat() {
  float ret = 0.0;
//  int length = 4;
  int hi_mant = 0, low_mant = 0, exp = 0, sign = 0;
  float f_val = 0.0;
  if (version <= 3) {   // Convert VAX Float
    unsigned long val = (long)readRunUnsignedInt();
    /*    if (val < 0) {
      val = val + 4294967296;
      }*/
    /* add 128 to put in the implied 1 */
    hi_mant = (val & 127) + 128;
    val = val >> 7;
    /* exponent is "excess 128" */
    exp = ((int)(val & 255)) - 128;
    val = val >> 8;
    
    sign = val & 1;
    low_mant = val >> 1;
    /* This could also be a "reserved" operand of sort */
    if ( exp == -128 ) 
      f_val = 0;
    else
      f_val = ((hi_mant/256.0) + (low_mant/16777216.0)) *
	pow(2.0, (double)exp);

    if ( sign == 1 ) f_val = -f_val;
    ret = (float)f_val;
  }
  else {
#ifdef IPNS_BIG_ENDIAN
    (*input).read((char *)&ret, 4);
#endif
#ifdef IPNS_LITTLE_ENDIAN
    float tempIn = 0;
    (*input).read((char *)&tempIn, 4);
    cout << tempIn;
    unsigned char *tempInChar;
    tempInChar = (unsigned char *)&tempIn;
    unsigned char temp0, temp1, temp2, temp3;
    temp0 = tempInChar[0];
    temp1 = tempInChar[1];
    temp2 = tempInChar[2];
    temp3 = tempInChar[3];
    tempInChar[0] = temp3;
    tempInChar[1] = temp2;
    tempInChar[2] = temp1;
    tempInChar[3] = temp0;
    ret = tempIn;
#endif
  }
  return ret;
}

//readRunString - read a string from a file
void RandomAccessRunfile::readRunString(char charArray[], int len) {
  (*input).read((char *)charArray, len);
}

//readRunFileNum - read a FileNumber from a file.  This is a special version
//                 of readRunString
int RandomAccessRunfile::readRunFileNum() {
  int ret = 0;
  if (version > 3) {
    ret = readRunInt();
  }
#ifdef IPNS_BIG_ENDIAN
#endif
#ifdef IPNS_LITTLE_ENDIAN
  else if (version <=3) {
    char inp[4] = {0, 0, 0, 0};
    readRunString(&(inp[0]), 4);
    ret = (inp[0] - 48) * 1000;
    ret += (inp[1] - 48) * 100;
    ret += (inp[2] - 48) * 10;
    ret += (inp[3] - 48);
    //    cout << "readRunFileNum: inp" << inp << endl;
  }
#endif
  return ret;
}

//readRunIntArray - read an array of integers from a file. The argument
//intArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1
void RandomAccessRunfile::readRunIntArray(int *intArray, int nel) {
  int ii;
  for ( ii = 1; ii <= nel; ii++ ) {
    intArray[ii] = readRunInt();

  }
}

//readRunShortArray - read an array of shorts from a file. The argument
//shortArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1 
void RandomAccessRunfile::readRunShortArray(short *shortArray, int nel) {
  int ii;
  for ( ii = 1; ii <= nel; ii++ ) {
    shortArray[ii] = readRunShort();
  }
}

//readRunFloatArray - read an array of floats from a file. The argument
//shortArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1 
void RandomAccessRunfile::readRunFloatArray(float *floatArray, int nel) {
  int ii;
  
  for ( ii = 1; ii <= nel; ii++ ) {
    floatArray[ii] = readRunFloat();
  }
}

//readRunIntVector - read an array of integers from a file. The argument
//intArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1
void RandomAccessRunfile::readRunIntVector(vector<int> *intArray, int nel) {
  int ii;
  for ( ii = 1; ii <= nel; ii++ ) {
    (*intArray)[ii] = readRunInt();

  }
}

//readRunShortVector - read an array of shorts from a file. The argument
//shortArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1 
void RandomAccessRunfile::readRunShortVector(vector<short> *shortArray, int nel) {
  int ii;
  for ( ii = 1; ii <= nel; ii++ ) {
    (*shortArray)[ii] = readRunShort();
  }
}

//readRunFloatVector  - read an array of floats from a file. The argument
//shortArray needs to have memory allocated for nel + 1 elements since the 
//array is assumed to start indexing at 1 
void RandomAccessRunfile::readRunFloatVector(vector<float> *floatArray, int nel) {
  int ii;
  
  for ( ii = 1; ii <= nel; ii++ ) {
    (*floatArray)[ii] = readRunFloat();
  }
}

/*
main(int argc, char *argv[]) {
  cout << "Input file is " << argv[1] << "\n";
  ifstream inFile(argv[1], ios::in);
  if ( !inFile ) {
    cerr << "File could not be opened: " << argv[1] << "\n";
  }
  RandomAccessRunfile rarf(&inFile);
  return 0;

}
*/
