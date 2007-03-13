#include <libxml/parser.h>
#include <libxml/tree.h>
#include "preferences.hpp"

using std::string;
using std::vector;

static const int BUFFER_SIZE = 256;
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

  void loadPreferences(std::string &filename, std::vector<Item> &preferences) {
    if (filename.size() <= 0)
      {
        setDefaultPreferences(preferences);
      }
    else
      {
        xmlDocPtr doc = NULL; // document pointer
        xmlNodePtr root_node = NULL;
        xmlNodePtr node = NULL;
        xmlNodePtr node1 = NULL;
        char bufffer[BUFFER_SIZE];
        int i,j;
        // do nothing for now with the provided file
      }
  }
}
