#include "string_location_format.h"
#include <stdexcept>
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::runtime_error;
using std::invalid_argument;

/*********************************
/Format the string location
/*********************************/
void format_string_location(const string& location, string& new_location)
{
  without_white_spaces(location, new_location);
  return;
}

/*********************************
/Remove white spaces
/*********************************/
void without_white_spaces(const string& location, string& new_location)
{
  typedef std::string::size_type string_size;
  string_size i=0;
  std::string ret="";
 
   while (i != location.size())
    {
      while (i != location.size() && isspace(location[i]))
	{
	  ++i;	
        }
     //find end of next word
     string_size j=i;
     while (j != location.size() && !isspace(location[j]))
       ++j;

     if (i != j)
       {
	 ret+=location.substr(i,j-i);
         i=j;
       }
    } 
   new_location = ret;
   return;
}

/*************************************
/Separate Declaration from definition
/*************************************/
void DeclaDef_separator(string& new_location,vector<string>& VecStr)
{
  typedef std::string::size_type string_size;
  int Dposition = new_location.find("#");

  if (Dposition == -1)
    throw runtime_error("Declaration/Definition spacer invalid or not present");

  string_size taille = new_location.size();
  
  VecStr.push_back(new_location.substr(0,Dposition));
  
  if (taille == Dposition+1)
      //if (taille - VecStr[0].size()-1 <= Dposition+1)
    {
        VecStr.push_back("");
    }
  else
    {
        VecStr.push_back(new_location.substr(Dposition+1, taille - VecStr[0].size()-1));
    }
  return;
}

/*********************************
/Separate local from global array
/*********************************/
void Declaration_separator(string DeclarationStr, vector<string>& LocGlobArray)
{
  // std::vector<string> LocalArray_GlobalArray;
  std::string str;
  typedef string::size_type string_size;
  string_size DeclarationStrSize = DeclarationStr.size();
  int SeparatorPosition = DeclarationStr.find("][");
  
  if (SeparatorPosition == -1)
    throw runtime_error("Format of declaration not valid");  

  LocGlobArray.push_back(DeclarationStr.substr(0,SeparatorPosition+1));
  LocGlobArray.push_back(DeclarationStr.substr(SeparatorPosition+1,DeclarationStrSize-LocGlobArray[0].size()));
  
  return;
}

/*********************************
/Separate Tag from Definition
/*********************************/
string TagDef_separator(string& DefinitionPart, vector<string>& Tag, vector<string>& Def, string& DefinitionGrpVersion)
{
  std::vector<string> ret;
  typedef string::iterator iter;
  iter b = DefinitionPart.begin(), e =  DefinitionPart.end(), a;
  int CPosition, OpenBraPosition, CloseBraPosition = 1;
  string  separator = "|";
  string StringLocationGroup = DefinitionPart;
  static const string OpenBracket = "{";
  static const string CloseBracket = "}";
  int HowManyTimes = 0, i = 0, length =  DefinitionPart.size();

  while (b!=e)
    { 
       if (find(b,  DefinitionPart.end(), separator[0]) !=  DefinitionPart.end())
      	{
	   ++HowManyTimes;
	   b = find(b, DefinitionPart.end(),separator[0]);
	   b=b+1;
	}
        b=b+1;
    }

  while (i < HowManyTimes)
    {
      CPosition =  DefinitionPart.find(separator[0]);
      OpenBraPosition =  DefinitionPart.find(OpenBracket);
      CloseBraPosition =  DefinitionPart.find(CloseBracket);

      CheckSpacerValidity(OpenBraPosition,CPosition,CloseBraPosition);

      Tag.push_back( DefinitionPart.substr(OpenBraPosition+1,CPosition-OpenBraPosition-1));
   
      CheckTagValidity(Tag[i]);
      Def.push_back( DefinitionPart.substr(CPosition+1,CloseBraPosition-CPosition-1));

       DefinitionPart = DefinitionPart.substr(CloseBraPosition+1, length-CloseBraPosition-1);
      
      ++i;
    };

   ReplaceTagDef_by_Grp(StringLocationGroup,HowManyTimes,DefinitionGrpVersion );
   return DefinitionGrpVersion;
}

/*********************************
/Replace Tag/Def part by Grp#
/*********************************/
void ReplaceTagDef_by_Grp(string& StringLocationGroup, int HowManyTimes, string & DefinitionGrpVersion)
{
  static const string  separator = "|";
  static const string OpenBracket = "{";
  static const string CloseBracket = "}";
  string part1, part2;
  int OpenBraPosition, CloseBraPosition;
 
  for (int j=0 ; j<HowManyTimes ; j++)
  
    {
      std::ostringstream Grp;
      OpenBraPosition =  StringLocationGroup.find(OpenBracket);
      CloseBraPosition =  StringLocationGroup.find(CloseBracket);
      
       StringLocationGroup.erase(OpenBraPosition, CloseBraPosition+1-OpenBraPosition);
     
      part1 = StringLocationGroup.substr(0,OpenBraPosition);
      part2 = StringLocationGroup.substr(OpenBraPosition, StringLocationGroup.size());
     
      Grp << "grp" << j ;  
      DefinitionGrpVersion = part1 + Grp.str() + part2;
      }
  
  return;
}

/*********************************
/Check if a "|" spacer is missing
/*********************************/
void CheckSpacerValidity(int openBra, int spacerPosition, int closeBra)
{
  if (spacerPosition < openBra || spacerPosition > closeBra)
     
    throw runtime_error("Missing \"|\" spacer or wrong definition declaration");
  return;
}

/*********************************
/Check validity of Tags
/*********************************/
void CheckTagValidity (string & Tag)
{
 if (Tag == "pixelID" || Tag == "pixelX" || Tag == "pixelY" || Tag == "Tbin")
    return;
  else
    throw runtime_error("One of the Tag is not a valid Tag");
return;
}

/*********************************
/Store operators
/*********************************/
void StoreOperators(string& StrS, int& HowMany, vector<string>& Ope)
{
  typedef string::iterator iter;
  std::vector<iter> VecIter;             //vector of iterators
  std::string::iterator a;
  string operatorOR = "OR";
  string operatorAND = "AND";

 //Store each position of "|" into VecIter
 VecIter = PositionSeparator(StrS,HowMany);
  
#ifdef RETRIEVER_DEFINITION_TEST
 cout << endl <<endl << "List of operators:" << endl;
#endif

  for (int i=0; i<HowMany-1; i++)
    {
      if (find(VecIter[i],VecIter[i+1],operatorOR[0])!=VecIter[i+1])
      { 
	Ope.push_back("OR");
      }
      else if (find(VecIter[i],VecIter[i+1],operatorAND[0])!=VecIter[i+1])
	{
	  Ope.push_back("AND");
	}
      else
	throw runtime_error("Not a valid operator");

#ifdef RETRIEVER_DEFINITION_TEST
      cout << "   Ope[" << i << "]= " << Ope[i];  
#endif

    }	  
  return;
}

/*********************************
/Find iterator for each separator
/*********************************/
vector<string::iterator> PositionSeparator(string s, int TagName_Number)
{ std::vector<string::iterator> VecIter;
  int i = 0;
  typedef string::iterator iter;
  iter b = s.begin();  
  string separator = "|";

  while (i < TagName_Number)
    {
      VecIter.push_back(find(b, s.end(), separator[0]));
      b = find(b,s.end(),separator[0])+1;
      ++i;
    }
  return VecIter;
}

/*********************************
/Give priority for each group
/*********************************/
void GivePriorityToGrp ( string& s, int OperatorNumber, vector<int>& GrpPriority, vector<int>& InverseDef, vector<int>& OpePriority)
{
  int DefinitionString_size = s.size();
  int GrpNumberLive = 0;
  int Priority = 0;
    
  //Initialization of vector<int> and vector<bool>
  for (int i=0; i<OperatorNumber; i++)
    {
      GrpPriority.push_back(0);
      InverseDef.push_back(0);
      OpePriority.push_back(0);   //NEW
    }
  
  //move along the definition part and look for g,A,O,!,(,and ).
  for (int j=0; j<DefinitionString_size; j++)
    {
      switch (s[j])
	{
	case 'g':
          j=j+3;   //move to end of grp#
	  GrpPriority[GrpNumberLive]=Priority;
   	  ++GrpNumberLive;
     	  break;
	case '!':
	  InverseDef[GrpNumberLive]=1;
          break;
	case '(':
	  ++Priority;
          break;
	case ')':
	  --Priority;
          break;
	case 'A':
	  OpePriority[GrpNumberLive-1]=Priority;
	  break;
	case 'O':
	  OpePriority[GrpNumberLive-1]=Priority;
	  break;
	default:
       	  //nothing
	  break;
	}
    }
 
  if (Priority != 0)
    throw runtime_error("Format of parentheses not valid");

#ifdef RETRIEVER_DEFINITION_TEST
  cout << endl << endl << "List of Priority and Inverse functions" << endl;
  for (int j=0; j<OperatorNumber; j++)
    {
      cout<<"   GrpPriority[" << j << "]= " << GrpPriority[j];
      cout<<"          ";
      cout<<"InverseDef["<<j<<"]= "<<InverseDef[j]<<endl;
    }
    cout << endl << "List of Priority of operators" << endl;
  for (int j=0; j<OperatorNumber-1;j++)
    {
      cout<<"   OpePriority[" <<j<<"]= "<< OpePriority[j]<<endl;
    }
 
#endif

  return;
}

