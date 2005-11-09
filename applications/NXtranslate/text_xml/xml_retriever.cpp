#include <algorithm>
#include <iostream>
#include <fstream>
#include <libxml/xmlmemory.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "retriever.h"
#include "xml_retriever_dom.h"
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
using string_util::starts_with;

typedef vector<string> StringVec;

static string get_type(const string &location){
  static const string CHAR("CHAR");
  if(starts_with(location,CHAR))
    return "NX_CHAR";

  static const string INT8("INT8");
  if(starts_with(location,INT8))
    return "NX_INT8";

  static const string INT16("INT16");
  if(starts_with(location,INT16))
    return "NX_INT16";

  static const string INT32("INT32");
  if(starts_with(location,INT32))
    return "NX_INT32";

  static const string UINT8("UINT8");
  if(starts_with(location,UINT8))
    return "NX_UINT8";

  static const string UINT16("UINT16");
  if(starts_with(location,UINT16))
    return "NX_UINT16";

  static const string UINT32("UINT32");
  if(starts_with(location,UINT32))
    return "NX_UINT32";

  static const string FLOAT32("FLOAT32");
  if(starts_with(location,FLOAT32))
    return "NX_FLOAT32";

  static const string FLOAT64("FLOAT64");
  if(starts_with(location,FLOAT64))
    return "NX_FLOAT64";

  throw invalid_argument("Cannot determine type in location: "+location);
}

static string xmlChar_to_str(const xmlChar *ch, int len){
  string result((char *)ch);
  if( (len>0) && ((unsigned int)len<result.size()) )
    result.erase(result.begin()+len,result.end());

  return string_util::trim(result);
}

static bool is_right_square_bracket(const char c){
  static const string RIGHT="]";
  return find(RIGHT.begin(),RIGHT.end(),c)!=RIGHT.end();
}

static string get_dims(const string &location){
  using std::find;
  static const string LEFT("[");

  if(!starts_with(location,LEFT))
    return "";

  string result="";
  for(string::const_iterator it=location.begin() ; it!=location.end() ; it++ ){
    result+=(*it);
    if(is_right_square_bracket(*it))
      break;
  }

  if(result.size()==location.size())
    return "";
  else
    return result;
}

static xmlNode* find_element(xmlNode *a_node, const string &name){
  xmlNode *cur_node=NULL;

  string nodeName;
  for( cur_node=a_node ; cur_node!=NULL ; cur_node=cur_node->next ){
    nodeName=xmlChar_to_str(cur_node->name,-1);
    if(nodeName==name)
      return cur_node->xmlChildrenNode;
  }
  return NULL;
}

static void print_element_names(xmlNode * a_node){
    xmlNode *cur_node = NULL;

    for( cur_node=a_node ; cur_node!=NULL ; cur_node=cur_node->next ){
        if(cur_node->type == XML_ELEMENT_NODE){
          cout << "node type: Element, name: " << cur_node->name << endl;
        }

        //print_element_names(cur_node->children);
    }
}

static string getStrAttr(const xmlNodePtr &node,string&name); // REMOVE

static xmlNode* open_path(xmlNode *node,StringVec::iterator begin, StringVec::iterator end){
  // error check input
  if(begin==end)
    return NULL;

  string name("type"); // REMOVE
  //cout << "[" << node->name << ":" << *begin  << ":" << getStrAttr(node,name) << "]->" ; // REMOVE

  // locate the next part of the path
  node=find_element(node,*begin);

  // if it returned poorly then get out now
  if(node==NULL)
    return NULL;

  /*
  string name("type"); // REMOVE
  cout << "(" << getStrAttr(node,name) << ")->"; // REMOVE
  */

  // go to next step
  begin++;
  if(begin==end)
    return node;

  // recursively call self
  return open_path(node,begin,end);
}

static string getStrValue(const xmlDocPtr &doc, const xmlNodePtr &node){
  return xmlChar_to_str(node->content,-1);
}

static string getStrAttr(const xmlNodePtr &node,string&name){
  //cout << "getStrAttr(" << node->name << "," << name << ")" << endl; // REMOVE

  // error check the input
  if(node==NULL)
    throw runtime_error("Encountered NULL Node in getStrAttr(node,"+name+")");
  if(name.size()<=0)
    return string("");

  // get the value
  xmlChar* c_name=xmlCharStrdup(name.c_str());
  //  xmlAttrPtr attr=xmlHasProp(node,(xmlChar*)c_name);
  xmlChar *char_value=xmlGetProp(node,(xmlChar*)c_name);

  //  cout << "*****NODE:" << node->name << endl; // REMOVE
  xmlAttrPtr attr=(xmlAttrPtr)(node->properties);
  while(attr!=0){
    cout << "     " << attr->name << ":" << attr->children->content << endl;
    attr=attr->next;
  }

  //  cout << "*****SIBL:" << node->next->name << endl;
  //  cout << "*****PARE:" << node->parent->name << endl;
  /*
  if(node->type==XML_TEXT_NODE) // REMOVE
    cout << "     is text node" << endl; // REMOVE
  else // REMOVE
    cout << "     CHIL:" << node->children->name << endl; // REMOVE
  */
  // free the name of the attribute
  xmlFree(c_name);

  // convert the value to a string
  if(char_value==NULL)
    return string("");
  string value=xmlChar_to_str(char_value,-1);

  // free the value
  xmlFree(char_value);

  // return the result
  return string_util::trim(value);
}

/**
 * The factory will call the constructor with a string. The string
 * specifies where to locate the data (e.g. a filename), but
 * interpreting the string is left up to the implementing code.
 */
TextXmlRetriever::TextXmlRetriever(const string &str): source(str){

  // BEGIN PARSING WITH NEW METHOD
  tree<Node> tr;
  string warn=buildTree(str,tr);
  // END PARSING WITH NEW METHOD

  // open the file
  doc=xmlParseFile(source.c_str());

  // check that open was successful
  if(doc==NULL){
    xmlFreeDoc(doc);
    xmlCleanupParser();
    throw runtime_error("Parsing "+source+" was not successful");
  }

  // check that the document is not empty
  xmlNode *xml_node = NULL;
  xml_node = xmlDocGetRootElement(doc);
  if(xml_node==NULL)
    throw runtime_error("Empty document ["+source+"]");
}

TextXmlRetriever::~TextXmlRetriever(){
  //cout << "~TextXmlRetriever()" << endl;

  if(doc==NULL) return;

  // close the file
  xmlFreeDoc(doc);
  xmlCleanupParser();
}

/**
 * This is the method for retrieving data from a file. The whole
 * tree will be written to the new file immediately after being
 * called. Interpreting the string is left up to the implementing
 * code.
 */
void TextXmlRetriever::getData(const string &location, tree<Node> &tr){
  cout << "TextXmlRetriever::getData(" << location << ",tree)" << endl; // REMOVE
  // check that the argument is not an empty string
  if(location.size()<=0)
    throw invalid_argument("cannot parse empty string");

  // variables for the divided string version of the location
  string str_path;
  string type;
  string str_dims;

  // convert the location to a type and (string) path
  if(starts_with(location,"/")){
    str_path=location;
    type="NX_CHAR";
    str_dims="";
  }else{
    // get the type and remove it from the location
    type=get_type(location);
    str_path=location.substr(type.size()-3,location.size());
    // get the dimensions and remove it from the location
    str_dims=get_dims(str_path);
    str_path=str_path.substr(str_dims.size(),str_path.size());
    // remove the separating colon
    str_path=str_path.substr(1,str_path.size());
  }
  //std::cout << "TYPE=" << type << " DIMS=" << str_dims << " PATH=" << str_path << std::endl; // REMOVE
  StringVec path=string_util::string_to_path(str_path);
  Node::NXtype int_type=node_type(type);

  // get the root
  xmlNode *xml_node = NULL;
  xml_node = xmlDocGetRootElement(doc);

  // open the path
  xml_node=open_path(xml_node,path.begin(),path.end());
  //cout << endl; // REMOVE
  if(xml_node==NULL)
    throw invalid_argument("path ["+location+"] does not exist in file");
  //cout << "TYPE=" << xml_node->type << endl;

  // print the children
  /*cout << "CHILDREN=" ; // REMOVE
  for( xmlNodePtr itter=xml_node->children ; itter!=xml_node->last ; itter=itter->next ){
    cout << itter->name << "[" << itter->type << "] ";
  }
  cout << endl;
  */
  // print the siblings
  /* cout << "SIBS="; // REMOVE
  for( xmlNodePtr it=xml_node->next ; it!=NULL ; it=it->next )
    cout << it->name << "[" << it->type << "] ";
  cout << endl;
  */

  // get the attributes
  //cout << "NODE:" << node.name() << ":" << value << endl; // REMOVE
  string name("type");
  string attr=getStrAttr(xml_node,name);
  //  cout << "ATTR:" << name << "=" << attr << endl; // REMOVE
  name="axis";
  attr=getStrAttr(xml_node,name);
  //cout << "ATTR:" << name << "=" << attr << endl; // REMOVE
  //  name="units";
  attr=getStrAttr(xml_node,name);
  //  cout << "ATTR:" << name << "=" << attr << endl; // REMOVE

  // get the value
  string value=getStrValue(doc, xml_node);
  if(value.size()<=0)
    throw runtime_error("Encountered empty value ["+source+","+location+"]");

  // create an empty node
  Node node(*(path.rbegin()),"empty");

  // put the data in the node
  vector<int> dims;
  if(int_type==Node::CHAR){
    dims.push_back(value.size());
  }else{
    dims=string_util::str_to_intVec(str_dims);
  }

  update_node_from_string(node,value,dims,int_type);

  tr.insert(tr.begin(),node);
}

static void openPath(  xmlNode *root_element, const std::vector<std::string> &path, int &num_group, int &num_data){

}

static void closePath(  xmlNode *root_element, int &num_group, int &num_data){

}

const string TextXmlRetriever::MIME_TYPE("text/xml");

string TextXmlRetriever::toString() const{
  return "["+MIME_TYPE+"] "+source;
}
