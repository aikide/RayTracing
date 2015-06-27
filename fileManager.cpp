#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileManager.h"

#pragma warning(disable: 4996)

/* Format: task.exe --scene=scene.yaml --resolution_x=1024 --resolution_y=1024 --output=result.bmp --output_normals=norm.bmp --trace_depth=2 */
FileManager::FileManager(int argc, char **argv)
{
  inputInfo = 0;
  hasWrongArguments = false;

  /* Default files with min name length */
  strncpy(sceneFile, "s.yaml", 6);
  strncpy(resultFile, "r.bmp", 5);
  strncpy(normalFile, "n.bmp", 5);

  for (int i = 1; i < argc; i++) 
  {
    int len = strlen(argv[i]);

    if (strncmp(argv[i], "--scene=", 8) == 0)
    {  
      strncpy(sceneFile, argv[i] + 8, len - 8);
      sceneFile[len - 8] = 0;
      inputInfo += 1 << 0;
    }
    else if (strncmp(argv[i], "--output=", 9) == 0)
    {
      strncpy(resultFile, argv[i] + 9, len - 9);
      resultFile[len - 9] = 0;
      inputInfo += 1 << 3;
    }
    else if (strncmp(argv[i], "--output_normals=", 17) == 0)
    {
      strncpy(normalFile, argv[i] + 17, len - 17);
      normalFile[len - 17] = 0;
      inputInfo += 1 << 4;
    }
    else if (strncmp(argv[i], "--resolution_x=", 15) == 0)
    { 
      sceneInfo.resX = strtol(argv[i] + 15, NULL, 10);
      inputInfo += 1 << 1;
    }
    else if (strncmp(argv[i], "--resolution_y=", 15) == 0)
    {
      sceneInfo.resY = strtol(argv[i] + 15, NULL, 10);
      inputInfo += 1 << 2;
    }
    else if (strncmp(argv[i], "--trace_depth=", 14) == 0)
    {    
      sceneInfo.traceDepth = strtol(argv[i] + 14, NULL, 10);
      inputInfo += 1 << 5;
    }
    else
    {    
      hasWrongArguments = true;
      inputInfo = -1;
    }
  } 

  hasBadSceneFile = sceneInfo.ParseFile(sceneFile);
}


void FileManager::TraceRays()
{
  sceneInfo.TraceRays(&image, &normalImage);
  image.SaveImageToFile(resultFile);
  normalImage.SaveImageToFile(normalFile);
}


FileManager::~FileManager()
{

}