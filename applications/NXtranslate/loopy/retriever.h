#ifndef __LOOPY_RETRIEVER_GUARD
#define __LOOPY_RETRIEVER_GUARD

#include "../retriever.h"

// this is not intended to be inherited from
class LoopyRetriever: public Retriever{
 public:
  LoopyRetriever(const std::string &);
  ~LoopyRetriever();
  void getData(const std::string &, tree<Node> &);
  std::string toString() const; // done
  static const std::string MIME_TYPE; // done
 private:
  LoopyRetriever(const LoopyRetriever&); //copy constructor
  LoopyRetriever& operator=(const LoopyRetriever&); //operator "=" overloading
};

#endif
