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

//Declaration of functions
string format_string_location(string& s);    //format the string 
string without_white_spaces(string& s);      //remove spaces from string 
vector<string> DeclaDef_separator(string& s);//separate Declaration from Definition

/*********************************
/SnsHistogramRetriever constructor
/*********************************/
SnsHistogramRetriever::SnsHistogramRetriever(const string &str): source(str) 
{
  cout << "TextPlainRetriever(" << source << ")" << endl; // REMOVE

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
  vector<string> DeclaDef;
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
  cout << "Declaration part= " << DeclaDef[0]<<endl;
  cout << "Definition part= " << DeclaDef[1]<<endl;


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



