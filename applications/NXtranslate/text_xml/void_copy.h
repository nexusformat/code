#ifndef VOID_COPY_H
#define VOID_COPY_H

#include "../node.h"

namespace void_copy{
  extern bool from_float(const float* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_double(const double* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_short(const short int* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_int(const int* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_long(const long int* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_ushort(const unsigned short int* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_uint(const unsigned int* source, void *&copy, const int num_elem, const Node::NXtype type);

  extern bool from_ulong(const unsigned long int* source, void *&copy, const int num_elem, const Node::NXtype type);
}

#endif
