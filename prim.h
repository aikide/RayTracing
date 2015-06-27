/*  FILE: PRIM.H 
 *  DATE: 07.07.2011
 *  INITIAL PROJECT: EAS
 *  PURPOSE: Basic primitive definitions.
 */

#ifndef _PRIM_H_
#define _PRIM_H_

#include <windows.h>
#include <vector>

#include "vec.h"
#include "matr.h"
#include "utils.h"

/* Useful classes and definitions */
typedef enum {PRIM_NONE = 0, PRIM_TRIANGLE, PRIM_SPHERE, PRIM_PLANE, PRIM_CYLINDER, PRIM_CONE, PRIM_TORUS, PRIM_OBJECT} PrimType;

struct Ray
{
  Vec origin, direction; // Ray goes from 'origin' point along 'direction'
};

/* Class for storing intersected point data */
struct InterPoint 
{  
  float dist;                // Value, which helps to distinguish between front and back surfaces (always >= 0, because points which are < 0 is always behind camera)
  Vec point, normal;
};

typedef std::vector<InterPoint> InterPointVector;

/*** MAIN PRIMITIVE CLASS ***/
class Prim
{
protected:
  PrimType primType;

public:
  Prim();
  
  virtual InterPointVector Intersect(Ray &ray) = 0 {};

  ~Prim(void);
	
}; /* End of Prim class */


/*** PLANE CLASSS ***/
class Plane : public Prim
{
private:
  Vec point0, point1, point2;

public:
  Plane() { primType = PRIM_PLANE; }
  Plane(Vec &_point0, Vec &_point1, Vec &_point2) : point0(_point0), point1(_point1), point2(_point2) { primType = PRIM_PLANE; }

  void SetPoint(int pointNumber, Vec &point);
  void SetPoint(int pointNumber, float pointX, float pointY, float pointZ);
  InterPointVector Intersect(Ray &ray);

  ~Plane() {};
}; /* End of Plane class */


/*** TRIANGLE CLASSS ***/
class Triangle : public Prim
{
private:
  Vec point0, point1, point2;

public:
  Triangle() { primType = PRIM_TRIANGLE; }
  Triangle(Vec &_point0, Vec &_point1, Vec &_point2) : point0(_point0), point1(_point1), point2(_point2) { primType = PRIM_TRIANGLE; }

  void SetPoint(int pointNumber, Vec &point);
  void SetPoint(int pointNumber, float pointX, float pointY, float pointZ);
  InterPointVector Intersect(Ray &ray);

  ~Triangle() {};
}; /* End of Triangle class */


/*** SPHERE CLASSS ***/
class Sphere : public Prim
{
private:
  float radius;

public:  
  Sphere(float _radius = 1.0f) : radius(_radius) { primType = PRIM_SPHERE; }

  void SetRadius(float _radius) { radius = _radius; }
  InterPointVector Intersect(Ray &ray);

  ~Sphere() {};
}; /* End of Sphere class */


/*** CYLINDER CLASSS ***/
class Cylinder : public Prim
{
private:
  float radius, height;

public:
  Cylinder(float _radius = 1.0f, float _height = 1.0f) : radius(_radius), height(_height) { primType = PRIM_CYLINDER; }

  void SetRadius(float _radius) { radius = _radius; }
  void SetHeight(float _height) { height = _height; }
  InterPointVector Intersect(Ray &ray);

  ~Cylinder() {};
}; /* End of Cylinder class */


/*** CONE CLASSS ***/
class Cone : public Prim
{
private:
  float radius, height;

public:
  Cone(float _radius = 1.0f, float _height = 1.0f) : radius(_radius), height(_height) { primType = PRIM_CONE; }

  void SetRadius(float _radius) { radius = _radius; }
  void SetHeight(float _height) { height = _height; }
  InterPointVector Intersect(Ray &ray);

  ~Cone() {};
}; /* End of Cone class */


/*** TORUS CLASSS ***/
class Torus : public Prim
{
private:
  float radius, tubeRadius;

public:
  Torus(float _radius = 1.0f, float _tubeRadius = 1.0f) : radius(_radius), tubeRadius(_tubeRadius) { primType = PRIM_TORUS; }

  void SetRadius(float _radius) { radius = _radius; }
  void SetTubeRadius(float _tubeRadius) { tubeRadius = _tubeRadius; }
  InterPointVector Intersect(Ray &ray);

  ~Torus() {};
}; /* End of Torus class */

typedef std::vector<Vec> VertexVector;
typedef std::vector<Triangle> TriangleVector;

/*** OBJECT CLASSS ***/
class Object : public Prim
{
private:
  bool isLoaded, hasNormals;
  char fileName[128];
  VertexVector vertices, normals;
  TriangleVector facets;

  bool LoadOBJFile();
public:
  Object(char *_fileName = NULL, bool autoLoad = true);

  void SetFileName(char *_fileName = NULL, bool autoLoad = true);
  InterPointVector Intersect(Ray &ray);

  ~Object();
}; /* End of Object class */


#endif /* _PRIM_H_ */


/******************************/
/*    END OF 'PRIM.H' FILE    */ 
/******************************/