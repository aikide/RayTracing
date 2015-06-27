#include <stdio.h>
#include <stdlib.h>

#include "fileManager.h"

int main(int argc, char **argv)
{
  FileManager fileManager(argc, argv);

  fileManager.TraceRays();

  return 0;
}