#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "sceneInfo.h"

class FileManager
{
private:
  char sceneFile[64];
  char normalFile[64];
  char resultFile[64];
    
  SceneInfo sceneInfo;
  Image image, normalImage;

  int inputInfo;
  bool hasWrongArguments, hasBadSceneFile;

public:
  FileManager(int argc, char **argv);

  void TraceRays(); 

  ~FileManager();
};



#endif /* __FILE_MANAGER_H__ */