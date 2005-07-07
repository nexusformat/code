#include <stdexcept>
#include "retriever.h"
#include "nexus_retriever.h"

#ifdef IPNS_RETRIEVER
#include "IPNS_CPP/ipns_retriever.h"
#endif
#ifdef TEXT_PLAIN_RETRIEVER
#include "text_plain/retriever.h"
#endif
#ifdef TEXT_XML_RETRIEVER
#include "text_xml/retriever.h"
#endif
#ifdef TEXT_COLLIST_RETRIEVER
#include "text_collist/collist_retriever.h"
#endif

using std::string;
using std::invalid_argument;

// Implementation of a pure virtual destructor. This makes the compiler happy.
Retriever::~Retriever(){}

// factory method
Retriever::RetrieverPtr Retriever::getInstance(const string & type, const string &source){
#ifdef TEXT_COLLIST_RETRIEVER
printf("collist retriever defined\n");
#endif 
  // return appropriate retriever based on type
  if(type==NexusRetriever::MIME_TYPE){
    RetrieverPtr ptr(new NexusRetriever(source));
    return ptr;
#ifdef IPNS_RETRIEVER
  }else if(type==IpnsRetriever::MIME_TYPE){
    RetrieverPtr ptr(new IpnsRetriever(source));
    return ptr;
#endif
#ifdef TEXT_PLAIN_RETRIEVER
  }else if(type==TextPlainRetriever::MIME_TYPE){
    RetrieverPtr ptr(new TextPlainRetriever(source));
    return ptr;
#endif
#ifdef TEXT_COLLIST_RETRIEVER
  }else if(type==TextCollistRetriever::MIME_TYPE){
    RetrieverPtr ptr(new TextCollistRetriever(source));
    return ptr;
#endif
#ifdef TEXT_XML_RETRIEVER
  }else if(type==TextXmlRetriever::MIME_TYPE){
    RetrieverPtr ptr(new TextXmlRetriever(source));
    return ptr;
#endif
  }

  // if it gets this far the type is not understood
  throw invalid_argument("do not understand mime_type ("+type+") in retriever_factory");
}
