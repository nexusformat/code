#ifndef __TEXT_XML_RETRIEVER_GUARD
#define __TEXT_XML_RETRIEVER_GUARD

#include "../retriever.h"
#include <fstream>
#include <libxml/parser.h>

// this is not intended to be inherited from
class TextXmlRetriever: public Retriever{
 public:
  TextXmlRetriever(const std::string &);
  ~TextXmlRetriever();
  void getData(const std::string &, tree<Node> &);
  std::string toString() const;
  static const std::string MIME_TYPE;
 private:
  TextXmlRetriever(const TextXmlRetriever&);
  TextXmlRetriever& operator=(const TextXmlRetriever&);
  std::string source;
  xmlDocPtr doc;
};
#endif
