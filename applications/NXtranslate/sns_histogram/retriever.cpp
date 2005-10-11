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

#define RETRIEVER_TEST      //For debugging only, REMOVE

using std::ifstream;
using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::cout;
using std::endl;
using std::vector;

//Global variable
vector<string> Tag, Def;
vector<string> VecStr;
vector<int> LocalArray, GlobalArray;  //parameters of the declaration part

struct Grp_parameters   //parameters of the different definitions
{
  int init, last, increment;  //with loop(init,end,increment)
  vector<int> value;          //(value[0],value[1],....)
  char c;                     //c=l for loop and c=p for (x,y,z,...)
};

vector<Grp_parameters> GrpPara;

//Declaration of functions
string format_string_location(string& s);    //format the string 
string without_white_spaces(string& s);      //remove spaces from string 
void DeclaDef_separator(string& s,vector<string>& DeclaDef);//separate Declaration from Definition
vector<string> Declaration_separator(string s); //Separate local array from global rarray
string TagDef_separator(string& s, vector<string>& Tag, vector<string>& Def);  //separate Tag from Definition part and return the Grp version of the string_location
string ReplaceTagDef_by_Grp(string& s, int a); //Tag/Def is replaced by Grp to determine the priorities of the associations
vector<string> StoreOperators(string& s, int& HowMany);  //isolate the operators of the definition part of the string location
vector<string::iterator> PositionSeparator(string s, int TagName_Number);  //Find the position of each separator "|"
void GivePriorityToGrp ( string& s, int OperatorNumber, vector<int> GrpPriority, vector<int> InverseDef);  //Give priority to each grp of the definition part
void DefinitionParametersFunction(vector<string> Def,int OperatorNumber);
void InitLastIncre (string& def, int i);   //Isolate loop(init,last,increment)
void ParseGrp_Value (string& def, int i);  //Isolate values of (....)
void ParseDeclarationArray(vector<string>& LocGlobArray);  //Parse Local and Global array from the declaration part

/*********************************
/SnsHistogramRetriever constructor
/*********************************/
SnsHistogramRetriever::SnsHistogramRetriever(const string &str): source(str) 
{
  // open the file
  infile.open(source.c_str());

  // check that open was successful
  if(!infile.is_open())
    throw invalid_argument("Could not open file: "+source);
}

/*********************************
/SnsHistogramRetriever destructor
/*********************************/
SnsHistogramRetriever::~SnsHistogramRetriever()
{
  // cout << "~TextPlainRetriever()" << endl;

  // close the file
  if(infile)
    infile.close();
}

/*********.***********************
/* This is the method for retrieving data from a file. 
/* Interpreting the string is left up to the implementing code.
/********************************/

void SnsHistogramRetriever::getData(const string &location, tree<Node> &tr)
{
  string new_location;
  string DefinitionGrpVersion;    //use to determine priorities
  vector<string> DeclaDef;        //declaration and definition parts
  vector<string> LocGlobArray;    //local and global array (declaration part)
  vector<string> Ope;             //Operators of the defintion part
  int OperatorNumber; 
  string DefinitionPart;          //use to determine the operators
  vector<int> GrpPriority;        //Vector of priority of each group
  vector<int> InverseDef;        //True=Inverse definition, False=keep it like it is


  new_location = location;

  // check that the argument is not an empty string
  if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");
#ifdef RETRIEVER_TEST
  cout << "Initial string location= " << endl << "  " << new_location << endl;
#endif

  //Once upon a time, there was a string location named "location"
  //"location" became new_location after loosing its white spaces
  new_location = format_string_location(new_location);
#ifdef RETRIEVER_TEST
  cout << endl << "String location without white spaces: " << endl << "  " << new_location <<endl;
#endif
  
  //location decided to separate its declaration part (left side of the "D")
  //from its definition part --> DeclaDef vector
  DeclaDef_separator(new_location,DeclaDef);

  //Separate declaration arrays (local and global)
  LocGlobArray = Declaration_separator(DeclaDef[0]);

  //Work on definition part
  DefinitionPart = DeclaDef[1];
  DefinitionGrpVersion = TagDef_separator(DeclaDef[1],Tag, Def);
  
#ifdef RETRIEVER_TEST
  cout << endl << "Version Grp of string location: " << endl << "   DefinitionGrpVersion= " << DefinitionGrpVersion << endl;   //REMOVE
#endif
 
  //Store operators
  OperatorNumber = Tag.size();
  Ope = StoreOperators(DefinitionPart,OperatorNumber);

  //Give to each grp its priority
  GivePriorityToGrp(DefinitionGrpVersion, OperatorNumber, GrpPriority, InverseDef);   
  //Store parameters of the definition part into GrpPara[0], GrpPara[1]...
  DefinitionParametersFunction(Def,OperatorNumber);

  //parse Local and Global Array from Declaration part
  ParseDeclarationArray(LocGlobArray);
  
  //Read the binary file and store the data into an array defined
  //by the GlobalArray part of the declaration

}

/*********************************
/SnsHistogramRetriever::MIME_TYPE 
/*********************************/
const string SnsHistogramRetriever::MIME_TYPE("application/x-SNS-histogram");

string SnsHistogramRetriever::toString() const
{
  return "["+MIME_TYPE+"] "+source;
}

/*********************************
/Format the string location
/*********************************/
std::string format_string_location(string& location)
{
    return without_white_spaces(location);
}

//Remove white spaces
std::string without_white_spaces(string& location)
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
  return ret;
}

/*********************************
/Separate Decla from Definition
/*********************************/
void DeclaDef_separator(string& new_location,vector<string>& VecStr)
{
  typedef std::string::size_type string_size;
  int Dposition = new_location.find("D");

  string_size taille = new_location.size();
  
  VecStr.push_back(new_location.substr(0,Dposition));
  VecStr.push_back(new_location.substr(Dposition+1, taille-VecStr[0].size()-1));

  return;
}

/*********************************
/Separate local from global array
/*********************************/
vector<string> Declaration_separator(string DeclarationStr)
{
  std::vector<string> LocalArray_GlobalArray;
  std::string str;
  typedef string::size_type string_size;
  string_size DeclarationStrSize = DeclarationStr.size();
  int SeparatorPosition = DeclarationStr.find("][");

  LocalArray_GlobalArray.push_back(DeclarationStr.substr(0,SeparatorPosition+1));
  LocalArray_GlobalArray.push_back(DeclarationStr.substr(SeparatorPosition+1,DeclarationStrSize-LocalArray_GlobalArray[0].size()));
  
  return LocalArray_GlobalArray;
}

/*********************************
/Separate Tag from Definition
/*********************************/
string TagDef_separator(string& DefinitionPart, vector<string>& Tag, vector<string>& Def)
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

      //StringLocationGroup.erase(OpenBraPosition, CloseBraPosition);
      //remove TagName|Definition by groupnumber
      
      Tag.push_back( DefinitionPart.substr(OpenBraPosition+1,CPosition-OpenBraPosition-1));
      Def.push_back( DefinitionPart.substr(CPosition+1,CloseBraPosition-CPosition-1));

       DefinitionPart= DefinitionPart.substr(CloseBraPosition+1, length-CloseBraPosition-2);
      
      ++i;
    }

   return ReplaceTagDef_by_Grp(StringLocationGroup,HowManyTimes);

}

/*********************************
/Replace Tag/Def part by Grp#
/*********************************/
string ReplaceTagDef_by_Grp(string& StringLocationGroup, int HowManyTimes)
{
  static const string  separator = "|";
  static const string OpenBracket = "{";
  static const string CloseBracket = "}";
  string part1, part2;
  int OpenBraPosition, CloseBraPosition;
 
  for (int j=0 ; j<HowManyTimes ; j++)
  
    {
      //cout << std::endl << std::endl << "j= " << j << std::endl << std::endl;//REMOVE
      std::ostringstream Grp;
      OpenBraPosition =  StringLocationGroup.find(OpenBracket);
      CloseBraPosition =  StringLocationGroup.find(CloseBracket);
      
       StringLocationGroup.erase(OpenBraPosition, CloseBraPosition+1-OpenBraPosition);
     
      part1 = StringLocationGroup.substr(0,OpenBraPosition);
      part2 = StringLocationGroup.substr(OpenBraPosition, StringLocationGroup.size());
     
      Grp << "grp" << j ;  
      StringLocationGroup = part1 + Grp.str() + part2;
      //cout << "(after adding grp) s= " << StringLocationGroup << std::endl;   //REMOVE
      }

  return StringLocationGroup;
}


/*********************************
/Store operators
/*********************************/
vector<string> StoreOperators(string& StrS, int& HowMany)
{
 typedef string::iterator iter;
  std::vector<iter> VecIter;             //vector of iterators
  std::string::iterator a;
  string operatorAND = "AND";
  string operatorOR = "OR";
  vector<string> Ope;

 //Store each position of "|" into VecIter
 VecIter = PositionSeparator(StrS,HowMany);
  
#ifdef RETRIEVER_TEST
 cout << endl << "List of operators:" << endl;
#endif

  for (int i=0; i<HowMany-1; i++)
    {
      if (find(VecIter[i],VecIter[i+1],operatorAND[0])!=VecIter[i+1])
      { 
	Ope.push_back("AND");
      }
      else
	{
	  Ope.push_back("OR");
	}
#ifdef RETRIEVER_TEST
      cout << "   Ope[" << i << "]= " << Ope[i]<< std::endl;   //REMOVE
#endif

    }	  
  
  return Ope;

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
void GivePriorityToGrp ( string& s, int OperatorNumber, vector<int> GrpPriority, vector<int> InverseDef)
{
  int DefinitionString_size = s.size();
  int GrpNumberLive = 0;
  int Priority = 0;
    
  //Initialization of vector<int> and vector<bool>
  for (int i=0; i<OperatorNumber; i++)
    {
      GrpPriority.push_back(0);
      InverseDef.push_back(0);
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
	default:
       	  //nothing
	  break;
	}
    }

#ifdef RETRIEVER_TEST
  cout << endl << "List of Priority and Inverse functions" << endl;
   //for debugging only     //REMOVE
  for (int j=0; j<OperatorNumber; j++)
    {
      cout<<"   GrpPriority[" << j << "]= " << GrpPriority[j];
      cout<<"          ";
      cout<<"InverseDef["<<j<<"]= "<<InverseDef[j]<<endl;
      }
#endif

  return;
}

/*********************************
/Store values of the definition
/*********************************/
void DefinitionParametersFunction(vector<string> Def,int HowManyDef)
{
  Grp_parameters record;
  string NewString;

#ifdef RETRIEVER_TEST
  cout << endl << "List of Definition: " << endl; 
 //for debugging only     //REMOVE
  for (int i =0; i<HowManyDef;i++)
    {
      cout << "   Def["<<i<<"]= " << Def[i]<<endl;
    }
#endif  

  //find out first if it's a loop or a (....)
  for (int i =0; i<HowManyDef;i++)
    {
      if (Def[i].find("loop") < Def[i].size()) 
	{
        record.c = 'l';
        }
      else
	{
        record.c = 'p';
        }
      GrpPara.push_back(record);
    }
 
  //isolate the variable
  for (int i=0; i<HowManyDef;i++)
    {
      if (GrpPara[i].c == 'l')      //loop
	{
	  InitLastIncre(Def[i],i);
	}
      else                          //(....)
	{
	  ParseGrp_Value(Def[i],i);
	}
    }
 return;
}

/*********************************
/Store Initial, last and increment
/*********************************/
void InitLastIncre (string& def, int i)
{
  static const string sep=",";
  int pos1, pos2;
  string new_def;

  //Remove "loop(" and ")"
  def=def.substr(5,def.size()-6);

  //store the info into GrpPara[i].init, end and increment
  pos1 = def.find(sep);
  new_def = def.substr(pos1+1,def.size()-pos1);
  pos2 = new_def.find(sep);
 
  GrpPara[i].init =atoi((def.substr(0,pos1)).c_str());
  GrpPara[i].last =atoi((def.substr(pos1+1,pos2).c_str()));
  GrpPara[i].increment = atoi((new_def.substr(pos2+1, new_def.size()-1).c_str()));

  return;
}

/*********************************
/Store values of (......) 
/*********************************/
void ParseGrp_Value(string& def, int i)
{
  int b=0, a=0;

  while (b <= def.size())
    {
      if (def[b]==',')
	{
	  GrpPara[i].value.push_back(atoi((def.substr(a,b-a)).c_str()));
	  a=b+1;
	}
      if (b==def.size())
	{
	  GrpPara[i].value.push_back(atoi((def.substr(a,b-a)).c_str()));
	}
      ++b;
    }

#ifdef RETRIEVER_TEST
  cout << endl << "List of values from list of identifiers: " << endl;
  //for debugging     //REMOVE
   for (int j=0; j<GrpPara[i].value.size(); j++)
    {
      cout << "   GrpPara.value["<<i<<"]= " << GrpPara[i].value[j]<<endl;
    }
#endif

  return;
}

/*********************************
/Parse Local and Globa array
/*********************************/


void ParseDeclarationArray(vector<string>& LocGlobArray)
{
  int a=0, b=0;
  

  //Parse Local array
  int i=0;

      //remove square braces
  LocGlobArray[i]=LocGlobArray[i].substr(1,LocGlobArray[i].size()-2);

#ifdef RETRIEVER_TEST
  cout << endl << "Local and Global Array: " << endl;
    cout << "   LocGlobArray[" << i << "]= " << LocGlobArray[i]<<endl;   //REMOVE
#endif

  while (b <= LocGlobArray[i].size())
    {
      if (LocGlobArray[i][b]==',')
	{
	  LocalArray.push_back(atoi((LocGlobArray[i].substr(a,b-a)).c_str()));
          a=b+1;}
	
      if (b==LocGlobArray[i].size())
	{
	  LocalArray.push_back(atoi((LocGlobArray[i].substr(a,b-a)).c_str()));
        }
	
      ++b;
    }

  i=1,a=0,b=0;
  
//Parse Global Array

 //remove square braces
  LocGlobArray[i]=LocGlobArray[i].substr(1,LocGlobArray[i].size()-2);
  
#ifdef RETRIEVER_TEST
     cout << "   LocGlobArray[" << i << "]= " << LocGlobArray[i]<<endl;   //REMOVE
#endif

  while (b <= LocGlobArray[i].size())
    {
      if (LocGlobArray[i][b]==',')
	{
	  GlobalArray.push_back(atoi((LocGlobArray[i].substr(a,b-a)).c_str()));
          a=b+1;}
	
      if (b==LocGlobArray[i].size())
	{
	  GlobalArray.push_back(atoi((LocGlobArray[i].substr(a,b-a)).c_str()));
        }
	
      ++b;
    }
#ifdef RETRIEVER_TEST
  cout << endl << "Parsing of values: " << endl;
  cout << "   Local array" << endl;

  //for debugging only      //REMOVE
  for (int j=0; j<LocalArray.size();j++)
    {
      cout << "      LocalArray["<<j<<"]= "<<LocalArray[j]<<endl;
    }
  //for debugging only      //REMOVE
  cout << "   Global array" << endl;
  for (int j=0; j<GlobalArray.size();j++)
    {
      cout << "      GlobalArray["<<j<<"]= "<<GlobalArray[j]<<endl;
    }
#endif


  return;
}



