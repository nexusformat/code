#include <string>
#include <stdlib.h>
#include <vector>

using std::string;
using std::vector;

void format_string_location(const string & s, string& s_output);    //format the string 
void without_white_spaces(const string & s, string& s_output);      //remove spaces from string 
void DeclaDef_separator(string & s,vector<string>& DeclaDef);//separate Declaration from Definition
void Declaration_separator(string s, vector<string>& LocGlobArray); //Separate local array from global rarray
string TagDef_separator(string& DefinitionPart, vector<string>& Tag, vector<string>& Def, string& DefinitionGrpVersion); //separate Tag from Definition part and return the Grp version of the string_location
void ReplaceTagDef_by_Grp(string& s, int a, string& GrpVersion); //Tag/Def is replaced by Grp to determine the priorities of the associations
void CheckSpacerValidity(int i, int j, int k);  //check if a "|" is missing
void CheckTagValidity (string& Tag);  //Check if the Tags are valid
void StoreOperators(string& s, int& HowMany, vector<string>& Ope);  //isolate the operators of the definition part of the string location
vector<string::iterator> PositionSeparator(string s, int TagName_Number);  //Find the position of each separator "|"
void GivePriorityToGrp ( string& s, int OperatorNumber, vector<int>& GrpPriority, vector<int>& InverseDef, vector<int>& OpePriority);  //Give priority to each grp of the definition part

