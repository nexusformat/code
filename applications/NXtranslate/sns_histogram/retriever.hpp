#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include "retriever.h"
#include "../node.h"
#include "../node_util.h"
#include "../string_util.h"
#include "../tree.hh"
#include "string_location_format.h"

//Type of binary file
typedef int binary_type;


struct Grp_parameters   //parameters of the different definitions
{
  int init, last, increment;  //with loop(init,end,increment)
  vector<int> value;          //(value[0],value[1],....)
  char c;                     //c=l for loop and c=p for (x,y,z,...)
};

//Declaration of functions
void DefinitionParametersFunction(vector<string> Def,int OperatorNumber, 
                                  vector<Grp_parameters> & GrpPara, Grp_parameters & record);
//Isolate loop(init,last,increment)
void InitLastIncre (string & def, int i, 
                    vector<Grp_parameters> & GrpPara);   
//Isolate values of (....)
void ParseGrp_Value (string & def, int i, 
                     vector<Grp_parameters> & GrpPara);  
//Parse Local and Global array from the declaration part
void ParseDeclarationArray(vector<string> & LocGlobArray, 
                           vector<int> & LocalArray, vector<int> & GlobalArray);  
//find highest priority of Grp
int FindMaxPriority (vector<int>& GrpPriority);  
//calculate the final array according to definiton
void CalculateArray (vector<int> & GrpPriority, 
                     vector<int> & InverseDef, 
                     binary_type * BinaryArray, 
                     vector<string> Ope, 
                     vector<int> & OpePriority,
                     tree<Node> & tr, 
                     vector<string> & Tag, 
                     vector<string> & Def, 
                     vector<int> & LocalArray, 
                     vector<int> & GlobalArray, 
                     vector<Grp_parameters> & GrpPara);  
//make pixelID array
void MakeArray_pixelID (binary_type* MyGrpArray, 
                        binary_type* BinaryArray,
                        int grp_number,
                        int InverseDef, 
                        vector<string> & Def, 
                        vector<int> & LocalArray, 
                        vector<int> & GlobalArray, 
                        vector<Grp_parameters> & GrpPara);  
//make pixelX array
void MakeArray_pixelX (binary_type* MyGrpArray, 
                       binary_type* BinaryArray,
                       int grp_number,
                       int InverseDef, 
                       vector<string> & Def, 
                       vector<int> & LocalArray, 
                       vector<int> & GlobalArray, 
                       vector<Grp_parameters> & GrpPara);   
//make pixelY array
void MakeArray_pixelY (binary_type* MyGrpArray, 
                       binary_type* BinaryArray,
                       int grp_number,
                       int InverseDef, 
                       vector<string> & Def, 
                       vector<int> & LocalArray, 
                       vector<int> & GlobalArray, 
                       vector<Grp_parameters> & GrpPara);   
//make Tbin array
void MakeArray_Tbin (binary_type* MyGrpArray, 
                     binary_type* BinaryArray,
                     int grp_number,
                     int InverseDef, 
                     vector<string> & Def, 
                     vector<int> & LocalArray, 
                     vector<int> & GlobalArray, 
                     vector<Grp_parameters> & GrpPara);     
//make a copy of the binary array
void MakeArray_Everything (binary_type* MyGrpArray, 
                           binary_type* BinaryArray, 
                           vector<int> & LocalArray, 
                           vector<int> & GlobalArray); 
//Do the calculation between the two Arrays
void DoCalculation (binary_type* GrpArray1, 
                    binary_type* GrpArray2,
                    string Operator, 
                    vector<int> & LocalArray, 
                    vector<int> & GlobalArray, 
                    vector<Grp_parameters> & GrpPara);  
//to swap from little to big endian
inline void endian_swap(binary_type & x);  
//Initialize all the array created
void InitializeArray(binary_type * MyGrpArray, 
                     vector<int> & GlobalArray);    
//Make MyGrpArray when tag is !pixelID
void InversePixelIDLoop (binary_type* MyGrpArray, 
                         binary_type * BinaryArray, 
                         vector<int> & GlobalArray, 
                         vector<Grp_parameters> & GrpPara, 
                         int grp_number);  
void PixelIDLoop (binary_type * MyGrpArray, 
                  binary_type * BinaryArray, 
                  vector<int> & GlobalArray, 
                  vector<Grp_parameters> & GrpPara, 
                  int grp_number);
void InversePixelIDList (binary_type * MyGrpArray, 
                         binary_type * BinaryArray, 
                         vector<int> & GlobalArray, 
                         vector<Grp_parameters> & GrpPara, 
                         int grp_number);
void PixelIDList (binary_type * MyGrpArray, 
                  binary_type * BinaryArray, 
                  vector<int> & GlobalArray, 
                  vector<Grp_parameters> & GrpPara, 
                  int grp_number);
void InversePixelXLoop (binary_type * MyGrpArray, 
                        binary_type * BinaryArray, 
                        vector<int> & GlobalArray, 
                        vector<Grp_parameters> & GrpPara, 
                        int grp_number);
void PixelXLoop (binary_type * MyGrpArray, 
                 binary_type * BinaryArray, 
                 vector<int> & GlobalArray, 
                 vector<Grp_parameters> & GrpPara, 
                 int grp_number);
void InversePixelXList (binary_type * MyGrpArray, 
                        binary_type * BinaryArray, 
                        vector<int> & GlobalArray, 
                        vector<Grp_parameters> & GrpPara, 
                        int grp_number);
void PixelXList (binary_type * MyGrpArray, 
                 binary_type * BinaryArray, 
                 vector<int> & GlobalArray, 
                 vector<Grp_parameters> & GrpPara, 
                 int grp_number);
void InversePixelYLoop (binary_type * MyGrpArray, 
                        binary_type * BinaryArray, 
                        vector<int> & GlobalArray, 
                        vector<Grp_parameters> & GrpPara, 
                        int grp_number); 
void PixelYLoop  (binary_type * MyGrpArray, 
                  binary_type * BinaryArray, 
                  vector<int> & GlobalArray, 
                  vector<Grp_parameters> & GrpPara, 
                  int grp_number); 
void InversePixelYList (binary_type * MyGrpArray, 
                        binary_type * BinaryArray, 
                        vector<int> & GlobalArray, 
                        vector<Grp_parameters> & GrpPara, 
                        int grp_number); 
void PixelYList (binary_type * MyGrpArray, 
                 binary_type * BinaryArray, 
                 vector<int> & GlobalArray, 
                 vector<Grp_parameters> & GrpPara, 
                 int grp_number); 
void InverseTbinLoop (binary_type * MyGrpArray, 
                      binary_type * BinaryArray, 
                      vector<int> & GlobalArray, 
                      vector<Grp_parameters> & GrpPara, 
                      int grp_number); 
void TbinLoop (binary_type * MyGrpArray, 
               binary_type * BinaryArray, 
               vector<int> & GlobalArray, 
               vector<Grp_parameters> & GrpPara, 
               int grp_number);
void InverseTbinList  (binary_type * MyGrpArray, 
                       binary_type * BinaryArray, 
                       vector<int> & GlobalArray, 
                       vector<Grp_parameters> & GrpPara, 
                       int grp_number); 
void TbinList (binary_type * MyGrpArray, 
               binary_type * BinaryArray, 
               vector<int> & GlobalArray, 
               vector<Grp_parameters> & GrpPara, 
               int grp_number); 
void CheckHighestPriority (int & GrpPriority_size, 
                           int & CurrentPriority, 
                           vector<int> & GrpPriority);
void Shift_GrpPriorities_Left (vector<int> & GrpPriority, 
                               int & GrpPriority_size, 
                               int & i);
void Shift_Ope_OpePriority_Left (vector<int> & OpePriority, 
                                 vector<string> & Ope, 
                                 int & GrpPriority_size, 
                                 int & i);
void Shift_MyGrpArray_Left (binary_type ** MyGrpArray, 
                            int & GrpPriority_size, 
                            int & i);
void MakePriorities (vector<int> & GrpPriority, 
                     int & HighestPriority, 
                     vector<int> & OpePriority, 
                     binary_type ** MyGrpArray, 
                     vector<int> & GlobalArray, 
                     vector<int> & LocalArray, 
                     vector<string> & Ope, 
                     vector<Grp_parameters> & GrpPara);
void Check_Want_Everything (vector<string> & DeclaDef, 
                            int & Everything, 
                            vector<string> & Tag, 
                            vector<string> & Ope); 
void SwapEndian (vector<int> & GlobalArray, 
                 binary_type * BinaryArray);
