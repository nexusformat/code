#ifndef __IPNS_RETRIEVER_GUARD
#define __IPNS_RETRIEVER_GUARD

#include "Runfile.h"
#include "../retriever.h"

// this is not intended to be inherited from
class IpnsRetriever: public Retriever{
 public:
  IpnsRetriever(const std::string &);
  ~IpnsRetriever();
  void getData(const std::string &, tree<Node> &);
  std::string toString() const;
  std::string fixDate(char *, char *);
  static const std::string MIME_TYPE;
 private:
  void IpnsRetriever::getData(Node &, const std::string &);
  IpnsRetriever(const IpnsRetriever&);
  IpnsRetriever& operator=(const IpnsRetriever&);
  //  NXhandle *handle;
  std::string source;
  Runfile *runFile;
};
#endif
