#ifndef __SNS_HISTOGRAM_RETRIEVER_GUARD
#define __SNS_HISTOGRAM_RETRIEVER_GUARD

#include "../retriever.h"
#include <fstream>

// this is not intended to be inherited from
class SnsHistogramRetriever: public Retriever{
 public:
  SnsHistogramRetriever(const std::string &);
  ~SnsHistogramRetriever();
  void getData(const std::string &, tree<Node> &);
  std::string toString() const;
  static const std::string MIME_TYPE;
 private:
  SnsHistogramRetriever(const SnsHistogramRetriever&);
  SnsHistogramRetriever& operator=(const SnsHistogramRetriever&);
  std::string source;
  int current_line;
  std::ifstream infile;
};

#endif
