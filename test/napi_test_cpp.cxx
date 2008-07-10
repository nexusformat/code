#include <iostream>
#include "napiconfig.h"
#include "NeXusFile.hpp"

int main(int argc, char** argv)
{
  NeXus::File file("napi_cpp.nxs", NXACC_CREATE);
  std::cout << "Hello World" << std::endl;
  std::cout << "And Again" << std::endl;
  return 0;
}
