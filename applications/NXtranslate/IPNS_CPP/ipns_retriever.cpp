#include <stdexcept>
#include <string>
#include "ipns_retriever.h"
#include "../node.h"
#include "../string_util.h"
#include "../tree.hh"

using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::vector;
using string_util::starts_with;

const static int GROUP_STRING_LEN=128;

/**
 * The factory will call the constructor with a string. The string
 * specifies where to locate the data (e.g. a filename), but
 * interpreting the string is left up to the implementing code.
 */
IpnsRetriever::IpnsRetriever(const string &str): source(str){
  if(str.size()<=0)
    throw invalid_argument("Cannot initialize from an empty string");
  
  char filename[50];
  strcpy(filename,str.c_str());
  char *f_ptr;
  f_ptr = filename;
  runFile =  new Runfile(f_ptr);
  if(runFile==NULL){
    //handle=NULL;
    throw runtime_error("Opening Runfile failed");
  }
  
}

IpnsRetriever::~IpnsRetriever(){
  /*  if(handle!=NULL){
    if(NXclose(handle)!=NX_OK)
      throw runtime_error("NXclose failed");
    handle=NULL;
    }*/
}

/**
 * This is the method for retrieving data from a file. The whole
 * tree will be written to the new file immediately after being
 * called. Interpreting the string is left up to the implementing
 * code.
 */
void IpnsRetriever::getData(Node &node, const std::string &location){
    if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");
    
    string tLocation(location);
    if(starts_with(tLocation, "header")) {
      Header head = runFile->getHeader();
      int dims[1];
      const int *dim_ptr;
      dim_ptr = dims;
      void *value;
      int data_type;
      int data_rank;
      if( location == "header.runTitle" ) {
	value = new char[81];
	head.RunTitle((char *)value);
	dims[0] = (string((char *)value)).size();
	data_type = NX_CHAR;
	data_rank = 1;
      }
      if( location == "header.userName" ) {
	value = new char[21];
	head.UserName((char *)value);
	dims[0] = (string((char *)value)).size();
	data_type = NX_CHAR;
	data_rank = 1;
      }
      if( location == "header.startDateTime" ) {
	char date[10];
	head.StartDate((char *)date);
	char time[9];
	head.StartTime((char *)time);
	//value = new char[29];
	int year = (date[7]-48)*10 + (date[8]-48);
	cout << date[7]<< "  " << date[8] << "    " <<year << "\n";
	string sVal;
	if ( year > 80) {
	  sVal.append (1, '1');
	  sVal.append( 1, '9');
	}
	else {
	  sVal.append( 1, '2');
	  sVal.append( 1, '0');
	}
	sVal.append( 1, date[7]);
	sVal.append( 1, date[8]);
	sVal.append(1, '-');
	if ( date[3] == 'J' ) {
	  if ( date[4] == 'A' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '1');
	  }
	  else if (date[5] == 'N') {
	    sVal.append( 1, '0');
	    sVal.append( 1, '6');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '7');
	  }
	}
	else if ( date[3] == 'F' ) {
	  sVal.append( 1, '0');
	  sVal.append( 1, '2');
	}
	else if ( date[3] == 'M' ) {
	  if ( date[5] == 'R' ) {
	    sVal.append(1, '0');
	    sVal.append(1, '3');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '5');
	  }
	}
	else if ( date[3] == 'A' ) {
	  if ( date[5] == 'R' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '4');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '8');
	  }
	}
	else if ( date[3] == 'S' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '9');
	}
	else if ( date[3] == 'O' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '0');
	}
	else if ( date[3] == 'N' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '1');
	}
	else if ( date[3] == 'D' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '2');
	}
	sVal.append(1, '-');
	sVal.append(1, date[0]);
	sVal.append(1, date[1]);
	sVal.append(1,  'T');
	int ii;
	for (ii = 0; ii < 8; ii++ ) {
	  sVal.append(1, time[ii]);
	}
	sVal.append(1, '.');
	sVal.append(1, '0');
	sVal.append(1, '0');
	sVal.append(1, '-');
	sVal.append(1, '0');
	sVal.append(1, '6');
	sVal.append(1, ':');
	sVal.append(1, '0');
	sVal.append(1, '0');
	sVal.append(1, '\0');
	value = (void *)sVal.c_str();
	cout << (char *)value << '\n';
	dims[0] = (string((char *)value)).size();
	data_type = NX_CHAR;
	data_rank = 1;
      }
      if( location == "header.endDateTime" ) {
	char date[10];
	head.EndDate((char *)date);
	char time[9];
	head.EndTime((char *)time);
	//value = new char[29];
	int year = (date[7]-48)*10 + (date[8]-48);
	string sVal;
	if ( year > 80) {
	  sVal.append (1, '1');
	  sVal.append( 1, '9');
	}
	else {
	  sVal.append( 1, '2');
	  sVal.append( 1, '0');
	}
	sVal.append( 1, date[7]);
	sVal.append( 1, date[8]);
	sVal.append(1, '-');
	if ( date[3] == 'J' ) {
	  if ( date[4] == 'A' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '1');
	  }
	  else if (date[5] == 'N') {
	    sVal.append( 1, '0');
	    sVal.append( 1, '6');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '7');
	  }
	}
	else if ( date[3] == 'F' ) {
	  sVal.append( 1, '0');
	  sVal.append( 1, '2');
	}
	else if ( date[3] == 'M' ) {
	  if ( date[5] == 'R' ) {
	    sVal.append(1, '0');
	    sVal.append(1, '3');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '5');
	  }
	}
	else if ( date[3] == 'A' ) {
	  if ( date[5] == 'R' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '4');
	  }
	  else {
	    sVal.append( 1, '0');
	    sVal.append( 1, '8');
	  }
	}
	else if ( date[3] == 'S' ) {
	    sVal.append( 1, '0');
	    sVal.append( 1, '9');
	}
	else if ( date[3] == 'O' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '0');
	}
	else if ( date[3] == 'N' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '1');
	}
	else if ( date[3] == 'D' ) {
	    sVal.append( 1, '1');
	    sVal.append( 1, '2');
	}
	sVal.append(1, '-');
	sVal.append(1, date[0]);
	sVal.append(1, date[1]);
	sVal.append(1,  'T');
	int ii;
	for (ii = 0; ii < 8; ii++ ) {
	  sVal.append(1, time[ii]);
	}
	sVal.append(1, '.');
	sVal.append(1, '0');
	sVal.append(1, '0');
	sVal.append(1, '-');
	sVal.append(1, '0');
	sVal.append(1, '6');
	sVal.append(1, ':');
	sVal.append(1, '0');
	sVal.append(1, '0');
	sVal.append(1, '\0');
	value = (void *)sVal.c_str();
	cout << (char *)value << '\n';
	dims[0] = (string((char *)value)).size();
	data_type = NX_CHAR;
	data_rank = 1;
      }
      node = Node(tLocation, value, 1, &(dims[0]), data_type);
      free(value);
    }
}

void IpnsRetriever::getData(const string &location, tree<Node> &tr){
  if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");
  Node node("empty","empty");
  getData(node,location);
  tr.insert(tr.begin(),node);
}

const string IpnsRetriever::MIME_TYPE("application/x-IPNS");

string IpnsRetriever::toString() const{
  return "["+MIME_TYPE+"] "+source;
}
