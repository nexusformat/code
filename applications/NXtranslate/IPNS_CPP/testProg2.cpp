#include "Runfile.h"
#include <stdlib.h>
#include <string.h>

main(int argc, char *argv[]) {
  cout << "Input file is " << argv[1] << "\n";
  Runfile runFile(argv[1]);

  Header header = runFile.getHeader();

  char userName[21];
  header.UserName( userName );
  cout << userName << "\n";




  return 0;

}
