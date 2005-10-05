#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include "string_util_ext.h"
#include "../string_util.h"

using std::cout;
using std::endl;
using std::find;
using std::find_if;
using std::invalid_argument;
using std::runtime_error;
using std::isspace;
using std::string;
using std::vector;

/*
 * split a line of numeric entrys using whitespace, commas, semicolons, ... as delimiter 
 */
extern std::vector<std::string> string_util::split_values(const string &str){
	std::vector<std::string> result;
	typedef std::string::size_type string_size;

	string_size i=0;

	while(i<str.size()){

		// skip seperators at end of last word
		while(i<str.size() && (isspace(str[i]) || (ispunct(str[i]) && (str[i]!='.' && str[i]!='-' && str[i]!='+' && str[i]!='\'' && str[i]!='\"')) )) {
			i++;
		}
		// find end of "word"
		string_size j=i;
		//std::cout << "str["<<i<<"]: " << str[i] << std::endl;  
		if (str[i] == '\'') {
			while(j<str.size()) {
				j++;
				if (str[j]=='\'') {
					j++;
					break;
				}
			}
		}
		else if (str[i] == '\"') {
			while(j<str.size()) {
				j++;
				if (str[j]=='\"') {
					j++;
					break;
				}
			}
		}
		else {
			while(j<str.size() && !isspace(str[j]) && !( ispunct(str[j]) && str[j]!='.' && str[j]!='-' && str[j]!='+') ){
				j++;
			}
		}

		if(i!=j){
			result.push_back(str.substr(i,j-i));
			i=j;
		}
	}
	
	/*std::cout << "STARTTTTTTTTTTTTTTTTTTTTTTT: " <<std::endl;
	for (std::vector<std::string>::iterator itt=result.begin(); itt!=result.end(); itt++) {
		std::cout << "resulting vector: " << *itt << std::endl;
	}*/
	return result;
}

/*
 * split a line of numeric entrys using whitespace, commas, semicolons, ... as delimiter 
 */
extern std::vector<unsigned int> string_util::split_ints(const std::string &str){
  std::vector<unsigned int> result;
  typedef std::string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at end of last word
    while(i<str.size() && (isspace(str[i]) || (ispunct(str[i]) && str[i]!='.') )) {
      i++;
	 }

    // find end of "word"
    string_size j=i;
    while(j<str.size() && !isspace(str[j]) && !(ispunct(str[j]) && str[j]!='.') ){
      j++;
    }

    if(i!=j){
      result.push_back(string_util::str_to_int(str.substr(i,j-i)));
      i=j;
    }
  }

  return result;
}


/*
 * split a string into doubles using whitespace as delimiter 
 */
extern std::vector<double> string_util::split_doubles(const string &str){
  std::vector<double> result;
  typedef std::string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at end of last word
    while(i<str.size() && isspace(str[i]) )
      i++;

    // find end of "word"
    string_size j=i;
    while(j<str.size() && !isspace(str[j])){
      j++;
    }

    if(i!=j){
      result.push_back(string_util::str_to_float(str.substr(i,j-i)));
      i=j;
    }
  }

  return result;
}

/*
 * split a string up using colon as delimiter 
 */
extern std::vector<std::string> string_util::split_colons(const string &str){
  std::vector<std::string> result;
  typedef std::string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at end of last word
    while(i<str.size() && (str[i]==',') )
      i++;

    // find end of "word"
    string_size j=i;
    while(j<str.size() && str[j]!=','){
      j++;
    }

    if(i!=j){
      result.push_back(str.substr(i,j-i));
      i=j;
    }
  }

  return result;
}

/*
 * split a string up using whitespace as delimiter 
 */
extern std::vector<std::string> string_util::split_whitespace(const string &str){
  std::vector<std::string> result;
  typedef std::string::size_type string_size;

  string_size i=0;
  while(i<str.size()){
    // skip seperators at end of last word
    while(i<str.size() && isspace(str[i]) )
      i++;

    // find end of "word"
    string_size j=i;
    while(j<str.size() && !isspace(str[j])){
      j++;
    }

    if(i!=j){
      result.push_back(str.substr(i,j-i));
      i=j;
    }
  }

  return result;
}

/*
 *  strip punctuation characters in front of and at end of strings  
 */
extern std::vector<std::string> string_util::strip_punct(std::vector<std::string> &strvec){
	std::vector<std::string> result;
	typedef std::string::size_type string_size;

	for (std::vector<std::string>::iterator it = strvec.begin(); it!=strvec.end(); it++) {
		string_size i=0;
	   std::string str = *it;
		// skip punctuation characters in front of word
		while (ispunct(str[i])){
			str = str.substr(1);
		}
		// skip punctuation characters at end of word
		while (ispunct(str[str.size()-1])){
			str = str.substr(0, str.size()-1);
		}
		
		if ((str.size()>0) && (isalpha(str[0]) || isdigit(str[0]))) {
			result.push_back(str);
		}
	}
	return result;
}

/*
 *  strip punctuation characters in front of at end of strings  
 */
extern bool string_util::contains (std::string &str, std::string substr) {
   unsigned int loc = str.find(substr, 0 );
   if( loc != string::npos )
     return true;
   else
     return false;	
}

extern std::string string_util::lower_str(std::string str) {
	for(unsigned int i=0; i<str.size(); i++) {
		char c = (char)tolower((char)str[i]);
		str[i] = c;
	}
	return str;
}
