#ifndef __SCENE_INFO_H__
#define __SCENE_INFO_H__

#include "prim.h"
#include "camera.h"

class Color
{
public:
  int r, g, b;

  Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {};
  Color(int _mono = 0) : r(_mono), g(_mono), b(_mono) {};

  ~Color() {};
};


struct PointData
{
  Color point, normal;
};


class Image
{
private:
  int width, height;
  Color *pixels; // y * width + x

public:
  Image(int _width = 640, int _height = 480);

  void SetupImage(int _width = 640, int _height = 480);
  void SetPixel(int x, int y, Color color);
  Color GetPixel(int x, int y);

  void SaveImageToFile(char *fileName);

  ~Image();
};


struct SceneObject
{
  Matrix trans, invTrans;
  Prim *prim;
};

typedef std::vector<SceneObject *> SceneObjectVector;

class SceneInfo
{
private:  
  FILE *sceneFile;  
  char buf[256];
  Camera camera;
  float dist;
  int deltaX, deltaY;
  int width, height;
  SceneObjectVector sceneObjects;   
  
  void SetupSceneScreen(float fovX = 90.0f, float fovY = 60.0f);
  bool ParseFileCameraNode();
  bool ParseFileUsualNode();
  bool ParseFileGeometryNode(PrimType primType, SceneObject *currObject);
  
  PointData GetPointColor(Ray &ray);
  InterPointVector FindIntersection(Ray &ray);  
public:
  int resX, resY;
  int traceDepth;
  
  SceneInfo();

  bool ParseFile(char *fileName);

  void TraceRays(Image *image, Image *normalImage);  

  ~SceneInfo();
};



#endif /* __SCENE_INFO_H__ */