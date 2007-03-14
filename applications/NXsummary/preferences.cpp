#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sstream>
#include <stdexcept>
#include "preferences.hpp"

using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::ostringstream;
using std::vector;

static const int BUFFER_SIZE = 256;
static const xmlChar *root_name = xmlCharStrdup("nxsummary");
static const xmlChar *item_name = xmlCharStrdup("item");
static const xmlChar *path_name = xmlCharStrdup("path");
static const xmlChar *label_name = xmlCharStrdup("label");
static const xmlChar *operation_name = xmlCharStrdup("operation");

namespace nxsum {
  void addItem(vector<Item> &preferences, const string &path,
               const string &label) {
    Item item;
    item.path = path;
    item.label = label;
    preferences.push_back(item);
  }

  void addItem(vector<Item> &preferences, const string &path,
               const string &label, const string &operation) {
    Item item;
    item.path = path;
    item.label = label;
    item.operation = operation;
    preferences.push_back(item);
  }

  void setDefaultPreferences(vector<Item> &preferences) {
    addItem(preferences, "/entry/title", "TITLE");
    addItem(preferences, "/entry/notes", "NOTES");
    addItem(preferences, "/entry/start_time", "START TIME");
    addItem(preferences, "/entry/end_time", "END TIME");
    addItem(preferences, "/entry/duration", "DURATION");
    addItem(preferences, "/entry/proton_charge", "PROTON CHARGE");
    addItem(preferences, "/entry/sample/name", "SAMPLE NAME");
    addItem(preferences, "/entry/sample/nature", "SAMPLE NATURE");
    addItem(preferences, "/entry/sample/type", "SAMPLE TYPE");
    addItem(preferences, "/entry/sample/identifier", "SAMPLE IDENTIFIER");
  /*
static const char*  TOTAL_COUNTS_PATH = "";
static const string TOTAL_COUNTS_LABEL = "TOTAL COUNTS";
static const char*  TOTAL_MON_COUNTS_PATH = "";
static const string TOTAL_MON_COUNTS_LABEL = "MONITOR COUNTS";
static const char*  PI_PATH = "";
static const string PI_LABEL = "PRICINPLE INVESTIGATOR";
  */
  }

  void loadPreferences(std::string &filename, xmlDocPtr &doc) {
    xmlLineNumbersDefault(1);
    doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL)
      {
        ostringstream s;
        s << "Could not read configuration file \"" << filename << "\"";
        throw runtime_error(s.str());
      }
  }

  void cleanupXml(xmlDocPtr &doc) {
    xmlFreeDoc(doc);
    xmlCleanupParser();
  }

  void loadPreference(xmlNodePtr &item_node, vector<Item> &preferences) {
    if (item_node == NULL)
      {
        return;
      }
    if (item_node->type != XML_ELEMENT_NODE)
      {
        return;
      }
    if (! xmlStrEqual(item_node->name, item_name))
      {
        return;
      }
    if (item_node->properties == NULL)
      {
        return;
      }
    Item item;
    if (xmlHasProp(item_node, path_name))
      {
        item.path = (char *) xmlGetProp(item_node, path_name);
      }
    else
      {
        ostringstream s;
        s << "Could not find path attribute in configuration file (line ";
        s << xmlGetLineNo(item_node) << ")";
        throw runtime_error(s.str());
      }
    if (xmlHasProp(item_node, label_name))
      {
        item.label = (char *) xmlGetProp(item_node, label_name);
      }
    else
      {
        ostringstream s;
        s << "Could not find label attribute in configuration file (line ";
        s << xmlGetLineNo(item_node) << ")";
        throw runtime_error(s.str());
      }
    if (xmlHasProp(item_node, operation_name))
      {
        item.operation = (char *)xmlGetProp(item_node, operation_name);
      }

    preferences.push_back(item);
  }

  void loadPreferences(xmlNodePtr &root_node, vector<Item> &preferences) {
    xmlNodePtr cur_node = NULL;

    for (cur_node = root_node; cur_node; cur_node = cur_node->next) {
      if ((cur_node->type == XML_ELEMENT_NODE)
                                && xmlStrEqual(cur_node->name, root_name)) {
        for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next) {
          loadPreference(cur_node, preferences);
        }
        return;
      }
    }
  }

  void loadPreferences(std::string &filename, std::vector<Item> &preferences) {
    if (filename.size() <= 0)
      {
        setDefaultPreferences(preferences);
      }
    else
      {
        xmlDocPtr doc = NULL; // document pointer
        loadPreferences(filename, doc);
        xmlNodePtr root_node = xmlDocGetRootElement(doc);
        loadPreferences(root_node, preferences);
        cleanupXml(doc);
      }
  }
}
