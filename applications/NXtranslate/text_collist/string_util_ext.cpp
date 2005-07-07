#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "string_util_ext.h"

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
    while(i<str.size() && (isspace(str[i]) || (ispunct(str[i]) && str[i]!='.') ))
      i++;

    // find end of "word"
    string_size j=i;
    while(j<str.size() && !isspace(str[j]) && !(ispunct(str[i]) && str[i]!='.') ){
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
 *  strip punctuation characters in front of at end of strings  
 */
extern std::vector<std::string> string_util::strip_punct(std::vector<std::string> &strvec){
	std::vector<std::string> result;
	typedef std::string::size_type string_size;

	for (std::vector<std::string>::iterator it = strvec.begin(); it!=strvec.end(); it++) {
		string_size i=0;
	   std::string str = *it;
		// skip characters in front of word
		while(i<str.size() && ispunct(str[i])) {
			i++;
		}

		// skip characters at end of word
		string_size j=str.size();
		while(j>=i && ispunct(str[j-1])){
			j--;
		}

		if(i!=j){
			std::string word = str.substr(i,j-i);
			if (word.size()>=2 && isalpha(word[0])) {
				result.push_back(word);
			}
			
			i=j;
		}
	}
	return result;
}
