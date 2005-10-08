#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iterator>
#include "retriever.h"
#include "../node.h"
#include "../node_util.h"
#include "../string_util.h"
#include "../tree.hh"

using std::ifstream;
using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::cout;
using std::endl;
using std::vector;

//Global variable
vector<string> Tag, Def;

//Declaration of functions
string format_string_location(string& s);    //format the string 
string without_white_spaces(string& s);      //remove spaces from string 
vector<string> DeclaDef_separator(string& s);//separate Declaration from Definition
vector<string> Declaration_separator(string& s); //Separate local array from global rarray
string TagDef_separator(string& s, vector<string>& Tag, vector<string>& Def);  //separate Tag from Definition part and return the Grp version of the string_location
string ReplaceTagDef_by_Grp(string& s, int a); //Tag/Def is replaced by Grp to determine the priorities of the associations

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
  cout << "~TextPlainRetriever()" << endl;

  // close the file
  if(infile)
    infile.close();
}

/********************************
/* This is the method for retrieving data from a file. 
/* Interpreting the string is left up to the implementing code.
/********************************/

void SnsHistogramRetriever::getData(const string &location, tree<Node> &tr)
{
  string new_location;
  string DefinitionGrpVersion;    //use to determine priorities
  vector<string> DeclaDef;        //declaration and definition parts
  vector<string> LocGlobArray;    //local and global array (declaration part)
  
  new_location = location;

  // check that the argument is not an empty string
  if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");

  //Once upon a time, there was a string location named "location"
  //"location" became new_location after loosing its white spaces
  new_location = format_string_location(new_location);
  
  //location decided to separate its declaration part (left side of the "D")
  //from its definition part --> DeclaDef vector
  DeclaDef = DeclaDef_separator(new_location);
  //Separate declaration arrays (local and global)
  LocGlobArray = Declaration_separator(DeclaDef[0]);

  //Work on definition part
  DefinitionGrpVersion = TagDef_separator(DeclaDef[1],Tag, Def);
  cout << "DefinitionGrpVersion= " << DefinitionGrpVersion << endl;

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
vector<string> DeclaDef_separator(string& new_location)
{
  vector<string> VecStr;
  typedef std::string::size_type string_size;
  int Dposition = new_location.find("D");

  string_size taille = new_location.size();
  
  VecStr.push_back(new_location.substr(0,Dposition));
  VecStr.push_back(new_location.substr(Dposition+1, taille-VecStr[0].size()-1));

  return VecStr;
}

/*********************************
/Separate local from global array
/*********************************/
vector<string> Declaration_separator(string& DeclarationStr)
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

  //cout << "StringLocationGroup (before adding group)= " << StringLocationGroup << std::endl;

 //find how many time we have "|" in the string
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

  //std::cout << "StringLocationGroup (after adding group)= " << StringLocationGroup << std::endl;
     
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
      //cout << std::endl << std::endl << "j= " << j << std::endl << std::endl;
      std::ostringstream Grp;
      OpenBraPosition =  StringLocationGroup.find(OpenBracket);
      CloseBraPosition =  StringLocationGroup.find(CloseBracket);
      
       StringLocationGroup.erase(OpenBraPosition, CloseBraPosition+1-OpenBraPosition);
     
      part1 = StringLocationGroup.substr(0,OpenBraPosition);
      part2 = StringLocationGroup.substr(OpenBraPosition, StringLocationGroup.size());
     
      Grp << "grp" << j ;  
      //cout << "grp = " << Grp.str() << std::endl;
       StringLocationGroup = part1 + Grp.str() + part2;
      //cout << "(after adding grp) s= " << s << std::endl;
     
          }

  return StringLocationGroup;


}
