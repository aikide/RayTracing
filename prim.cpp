/* Edward Burkov, 11-3, 18.05.2011 */


#include <math.h>

#include "prim.h"

#pragma warning(disable: 4996)

/** Prim Class Funtions **/
Prim::Prim()
{
  primType = PRIM_NONE;
}


Prim::~Prim( void )
{
}

/** Triangle Class Funtions **/
void Triangle::SetPoint(int pointNumber, Vec &point)
{
  if (pointNumber < 0 || pointNumber > 2)
    return;
  else if (pointNumber == 0)
    point0 = point;
  else if (pointNumber == 1)
    point1 = point;
  else if (pointNumber == 2)
    point2 = point;
}


void Triangle::SetPoint(int pointNumber, float pointX, float pointY, float pointZ)
{
  SetPoint(pointNumber, Vec(pointX, pointY, pointZ));
}


InterPointVector Triangle::Intersect(Ray &ray)
{
  ray.direction.Normalize();

  Vec edge1 = point1 - point0,
      edge2 = point2 - point0,
      planeNormal = edge1 % edge2,
      planeVec = point0 - ray.origin,
      temp;  
  InterPointVector result;
  InterPoint interPoint;
    
  planeNormal.Normalize();  

  if (fabs(ray.direction & planeNormal) < EPSILON) // Parallel case => empty set
    return result;
  else
  {
    interPoint.dist = (planeNormal & planeVec) / (ray.direction & planeNormal);

    if (interPoint.dist < EPSILON) // Point of intersecton is dehind camera => empty set
      return result;
    temp = ray.origin + ray.direction * interPoint.dist;

    /* In-out triangle check */
    if ((((point1 - point0) % (temp - point0)) & planeNormal) >= EPSILON &&
        (((point2 - point1) % (temp - point1)) & planeNormal) >= EPSILON &&
        (((point0 - point2) % (temp - point2)) & planeNormal) >= EPSILON)
    {
      interPoint.point = temp;
      interPoint.normal = planeNormal;
      result.push_back(interPoint);
    }    
  }

  return result;
}

/** Plane Class Funtions **/
void Plane::SetPoint(int pointNumber, Vec &point)
{
  if (pointNumber < 0 || pointNumber > 2)
    return;
  else if (pointNumber == 0)
    point0 = point;
  else if (pointNumber == 1)
    point1 = point;
  else if (pointNumber == 2)
    point2 = point;
}


void Plane::SetPoint(int pointNumber, float pointX, float pointY, float pointZ)
{
  SetPoint(pointNumber, Vec(pointX, pointY, pointZ));
}


InterPointVector Plane::Intersect(Ray &ray)
{  
  ray.direction.Normalize();

  Vec edge1 = point1 - point0,
      edge2 = point2 - point0,
      planeNormal = edge1 % edge2,
      planeVec = point0 - ray.origin;  
  InterPointVector result;
  InterPoint interPoint;
    
  planeNormal.Normalize();  

  if (fabs(ray.direction & planeNormal) < EPSILON) // Parallel case => empty set
    return result;
  else
  {
    interPoint.dist = (planeNormal & planeVec) / (ray.direction & planeNormal);

    if (interPoint.dist < EPSILON) // Point of intersecton is dehind camera => empty set
      return result;
    
    interPoint.point = ray.origin + ray.direction * interPoint.dist;
    interPoint.normal = planeNormal;
    result.push_back(interPoint);
  }

  return result;
}

/** Sphere Class Funtions **/
InterPointVector Sphere::Intersect(Ray &ray)
{
  float a = ray.direction & ray.direction,
    b = (ray.origin & ray.direction) * 2.0f,
    c = (ray.origin & ray.origin) - radius * radius;
  RootVector roots = SolveEquation2Deg(a, b, c);
  InterPointVector result;
  InterPoint interPoint;

  if (roots.size() >= 1 && roots[0] >= 0.0f)
  {
    interPoint.dist = roots[0];
    interPoint.point = ray.origin + ray.direction * roots[0];
    interPoint.normal = interPoint.point;
    interPoint.normal.Normalize();
    result.push_back(interPoint);
  }

  if (roots.size() == 2 && roots[1] >= 0.0f)
  {
    interPoint.dist = roots[1];
    interPoint.point = ray.origin + ray.direction * roots[1];
    interPoint.normal = interPoint.point;
    interPoint.normal.Normalize();
    result.push_back(interPoint);
  }
  
  return result;
}

/** Cylinder Class Funtions **/
InterPointVector Cylinder::Intersect(Ray &ray)
{
  float a = ray.direction.getX() * ray.direction.getX() + ray.direction.getY() * ray.direction.getY(),
    b = (ray.origin.getX() * ray.direction.getX() + ray.origin.getY() * ray.direction.getY()) * 2.0f,
    c = (ray.origin.getX() * ray.origin.getX() + ray.origin.getY() * ray.origin.getY()) - radius * radius,
    z1, z2, dist;
  RootVector roots = SolveEquation2Deg(a, b, c, true);
  InterPointVector result;
  InterPoint interPoint;
  Vec tempVec;
   
  if (roots.size() >= 1)
  {        
    z1 = ray.origin.getZ() + ray.direction.getZ() * roots[0];
    if (z1 >= -height / 2.0f && z1 <= height / 2.0f) // -height / 2 <= z1 <= height / 2 => has point of intersection of cylinder 
    {  
      interPoint.dist = roots[0];
      interPoint.point = ray.origin + ray.direction * roots[0];
      interPoint.normal = interPoint.point;
      interPoint.normal -= Vec(0.0f, 0.0f, interPoint.normal.getZ());
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
    else 
    {
      if (z1 < -height / 2.0f) // It is on bottom
        dist = (-height / 2.0f - ray.origin.getZ()) / ray.direction.getZ();
      else if (z1 > height / 2.0f) // It is on top
        dist = (height / 2.0f - ray.origin.getZ()) / ray.direction.getZ();
      
      tempVec = ray.origin + ray.direction * dist;
      
      if (dist >= 0 && tempVec.getX() * tempVec.getX() + tempVec.getY() * tempVec.getY() <= radius * radius)
      {
        interPoint.dist = dist;
        interPoint.point = tempVec;        
        interPoint.normal = Vec(0.0f, 0.0f, z1 < -height / 2.0f ? -1.0f : 1.0f);
        result.push_back(interPoint);
      }
    }
  }

  if (roots.size() == 2)
  {   
    z2 = ray.origin.getZ() + ray.direction.getZ() * roots[1];
    if (z2 >= -height / 2.0f && z2 <= height / 2.0f) // -height / 2 <= z2 <= height / 2 => has point of intersection of cylinder 
    {  
      interPoint.dist = roots[1];
      interPoint.point = ray.origin + ray.direction * roots[1];
      interPoint.normal = interPoint.point;
      interPoint.normal -= Vec(0.0f, 0.0f, interPoint.normal.getZ());
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
    else 
    {
      if (z2 < -height / 2.0f) // It is on bottom
        dist = (-height / 2.0f - ray.origin.getZ()) / ray.direction.getZ();
      else if (z2 > height / 2.0f) // It is on top
        dist = (height / 2.0f - ray.origin.getZ()) / ray.direction.getZ();
      
      tempVec = ray.origin + ray.direction * dist;

      if (dist >= 0 && tempVec.getX() * tempVec.getX() + tempVec.getY() * tempVec.getY() <= radius * radius)
      {
        interPoint.dist = dist;
        interPoint.point = tempVec;        
        interPoint.normal = Vec(0.0f, 0.0f, z2 < -height / 2.0f ? -1.0f : 1.0f);
        result.push_back(interPoint);
      }
    }
  }
  
  return result;
}

/** Cone Class Funtions **/
InterPointVector Cone::Intersect(Ray &ray)
{
  float coeff = (radius * radius) / (height * height),
    a = ray.direction.getX() * ray.direction.getX() + ray.direction.getY() * ray.direction.getY() - coeff * ray.direction.getZ() * ray.direction.getZ(),
    b = (ray.origin.getX() * ray.direction.getX() + ray.origin.getY() * ray.direction.getY() - coeff * ray.origin.getZ() * ray.direction.getZ()) * 2.0f,
    c = (ray.origin.getX() * ray.origin.getX() + ray.origin.getY() * ray.origin.getY() - coeff * ray.origin.getZ() * ray.origin.getZ()),
    z1, z2, dist;
  RootVector roots = SolveEquation2Deg(a, b, c, true);
  InterPointVector result;
  InterPoint interPoint;
  Vec tempVec;
   
  if (roots.size() >= 1)
  {        
    z1 = ray.origin.getZ() + ray.direction.getZ() * roots[0];
    if (z1 >= 0.0f && z1 <= height) // 0.0f <= z1 <= height => has point of intersection of cone 
    {  
      interPoint.dist = roots[0];
      interPoint.point = ray.origin + ray.direction * roots[0];
      interPoint.normal = interPoint.point;
      interPoint.normal -= Vec(0.0f, 0.0f, interPoint.normal.getZ());
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
    else if (z1 > height) // It is on top only
    {
      dist = (height  - ray.origin.getZ()) / ray.direction.getZ();      
      tempVec = ray.origin + ray.direction * dist;
      
      if (dist >= 0 && tempVec.getX() * tempVec.getX() + tempVec.getY() * tempVec.getY() <= radius * radius)
      {
        interPoint.dist = dist;
        interPoint.point = tempVec;        
        interPoint.normal = Vec(0.0f, 0.0f, 1.0f);
        result.push_back(interPoint);
      }
    }
  }

  if (roots.size() == 2)
  {   
    z2 = ray.origin.getZ() + ray.direction.getZ() * roots[1];
    if (z2 >= 0.0f && z2 <= height) // 0.0f <= z2 <= height => has point of intersection of cone 
    {  
      interPoint.dist = roots[1];
      interPoint.point = ray.origin + ray.direction * roots[1];
      interPoint.normal = interPoint.point;
      interPoint.normal -= Vec(0.0f, 0.0f, interPoint.normal.getZ());
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
    else if (z1 > height) // It is on top only
    {
      dist = (height - ray.origin.getZ()) / ray.direction.getZ();
      tempVec = ray.origin + ray.direction * dist;

      if (dist >= 0 && tempVec.getX() * tempVec.getX() + tempVec.getY() * tempVec.getY() <= radius * radius)
      {
        interPoint.dist = dist;
        interPoint.point = tempVec;        
        interPoint.normal = Vec(0.0f, 0.0f, 1.0f);
        result.push_back(interPoint);
      }
    }
  }
  
  return result;
}

/** Torus Class Funtions **/
InterPointVector Torus::Intersect(Ray &ray)
{
  ray.direction.Normalize();

  InterPointVector result;
  int numOfRoots = 0;
  float dirSquareLen = ray.direction & ray.direction,
    origSquareLen = ray.origin & ray.origin,
    dirDotOrig = ray.direction & ray.origin,
    a = dirSquareLen * dirSquareLen,
    b = 4.0f * dirDotOrig * dirSquareLen,
    c = 4.0f * dirDotOrig * dirDotOrig + 2.0f * dirSquareLen * (origSquareLen - radius * radius - tubeRadius * tubeRadius) + 
      4.0f * radius * radius * ray.direction.getZ() * ray.direction.getZ(),
    d = 4.0f * dirDotOrig * (origSquareLen - radius * radius - tubeRadius * tubeRadius) + 
      8.0f * radius * radius * ray.direction.getZ() * ray.origin.getZ(),
    e = (origSquareLen - radius * radius - tubeRadius * tubeRadius) * (origSquareLen - radius * radius - tubeRadius * tubeRadius) +
      4.0f * radius * radius * (ray.origin.getZ() * ray.origin.getZ() - tubeRadius * tubeRadius),
      temp;  
  RootVector roots = SolveEquation4Deg(a, b, c, d, e);		
  InterPoint interPoint;

  numOfRoots = roots.size();
  
  switch (numOfRoots)
  {
  case 4:
    if (roots[3] > 0)
    {
      interPoint.dist = roots[3];
      interPoint.point = ray.origin + ray.direction * roots[3];
      temp = 4.0f * ((interPoint.point & interPoint.point) - radius * radius - tubeRadius * tubeRadius);
      interPoint.normal = Vec(interPoint.point.getX() * temp, interPoint.point.getY() * temp, 
        interPoint.point.getZ() * (temp +  2.0f * radius * radius));
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
  case 3:
    if (roots[2] > 0)
    {
      interPoint.dist = roots[2];
      interPoint.point = ray.origin + ray.direction * roots[2];
      temp = 4.0f * ((interPoint.point & interPoint.point) - radius * radius - tubeRadius * tubeRadius);
      interPoint.normal = Vec(interPoint.point.getX() * temp, interPoint.point.getY() * temp, 
        interPoint.point.getZ() * (temp +  2.0f * radius * radius));
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
  case 2:
    if (roots[1] > 0)
    {
      interPoint.dist = roots[1];
      interPoint.point = ray.origin + ray.direction * roots[1];
      temp = 4.0f * ((interPoint.point & interPoint.point) - radius * radius - tubeRadius * tubeRadius);
      interPoint.normal = Vec(interPoint.point.getX() * temp, interPoint.point.getY() * temp, 
        interPoint.point.getZ() * (temp +  2.0f * radius * radius));
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }
  case 1:
    if (roots[0] > 0)
    {
      interPoint.dist = roots[0];
      interPoint.point = ray.origin + ray.direction * roots[0];
      temp = 4.0f * ((interPoint.point & interPoint.point) - radius * radius - tubeRadius * tubeRadius);
      interPoint.normal = Vec(interPoint.point.getX() * temp, interPoint.point.getY() * temp, 
        interPoint.point.getZ() * (temp +  2.0f * radius * radius));
      interPoint.normal.Normalize();
      result.push_back(interPoint);
    }  
  default:
    break;
  }

  return result;
}

/** Object Class Funtions **/
Object::Object(char *_fileName, bool autoLoad)
{
  primType = PRIM_OBJECT;
  isLoaded = hasNormals = false;
  if (_fileName == NULL)
    return;
  
  strncpy(fileName, _fileName, 128);
  if (autoLoad)
    isLoaded = LoadOBJFile();
}

Object::~Object()
{
  vertices.empty();
  normals.empty();
  facets.empty();
}


bool Object::LoadOBJFile()
{
  bool loadComplete = false, hasExternMaterial = false;
  char buf[256] = {0}, *returnedLine = NULL;  
  FILE *objFile = NULL;
  errno_t errCode;
  int vertexIndex[3] = {0}, normalIndex[3] = {0}, dump = 0;
  float tempX, tempY, tempZ;
  Triangle currFacet;
  
  errCode = fopen_s(&objFile, fileName, "r");
  if (errCode != 0)
  {
    printf("\n'Object::LoadOBJFile': Error # %i. Can't read file %s.\n", errCode, fileName);
    return true;
  }

	while (!loadComplete)
	{		
		returnedLine = fgets(buf, 256, objFile);
    if (returnedLine == NULL)
    {
      loadComplete = true;
      continue;
    }

    switch(buf[0])
    {
    case '#': // Comment. Fall through
    case '\n':
    case 's': // Fall through
    case 'o': // Fall through
    case 'g': // Group. Not supported. Fall through
      break;  // Skip to the next line    
    case 'u': // Check for usemtl
      if ((strncmp(buf, "usemtl ", 7) == 0))
        hasExternMaterial = true;
      break;
    case 0:   // End of file reached. We be done.
      loadComplete = true;
      break;

    case 'v': // Loading vertices      
      switch(buf[1])
      {
      case ' ': // Loading vertices
        sscanf_s(buf + 2, "%f %f %f", &tempX, &tempY, &tempZ);        
        vertices.push_back(Vec(tempX, tempY, tempZ));        
        break;

      case 'n': // Loading normals
        hasNormals = true;
        sscanf_s(buf + 2, " %f %f %f", &tempX, &tempY, &tempZ);        
        normals.push_back(Vec(tempX, tempY, tempZ));
        break;
      
      case 't': // Loading UVs - skip              
        break;

      default: 
        printf("Invalid vertex type: %c Should be of type \"v \", \"vn\" or \"vt\".", buf[1]);
        return false;
      }
      break;
    
    case 'f': // Loading facets  
      if(hasNormals && !hasExternMaterial)
      {                      
        sscanf_s(buf + 1, " %d//%d %d//%d %d//%d\n", 
          &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);        
 
        currFacet.SetPoint(0, vertices[vertexIndex[0] - 1]);
        currFacet.SetPoint(1, vertices[vertexIndex[1] - 1]);
        currFacet.SetPoint(2, vertices[vertexIndex[2] - 1]);
        facets.push_back(currFacet);       
      }
      else if (!hasExternMaterial)
      {
        sscanf_s(buf + 1, " %d %d %d\n", 
          &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
 
        currFacet.SetPoint(0, vertices[vertexIndex[0] - 1]);
        currFacet.SetPoint(1, vertices[vertexIndex[1] - 1]);
        currFacet.SetPoint(2, vertices[vertexIndex[2] - 1]);      
        facets.push_back(currFacet);       
      }
      else
      {
        sscanf_s(buf + 1, " %d/%d %d/%d %d/%d\n", 
          &vertexIndex[0], &dump, &vertexIndex[1], &dump, &vertexIndex[2], &dump);        
 
        currFacet.SetPoint(0, vertices[vertexIndex[0] - 1]);
        currFacet.SetPoint(1, vertices[vertexIndex[1] - 1]);
        currFacet.SetPoint(2, vertices[vertexIndex[2] - 1]);      
        facets.push_back(currFacet);       
      }
      break;
    
    default:
      printf("Invalid vertex type: %c Should be of type \"v \", \"vn\" or \"vt\".", buf[1]);
      return false;
    }    
  }

  return loadComplete;
}


void Object::SetFileName(char *_fileName, bool autoLoad)
{
  isLoaded = false;
  if (fileName == NULL)
    return;
  
  strncpy(fileName, _fileName, 128);
  if (autoLoad)
    isLoaded = LoadOBJFile();
}


InterPointVector Object::Intersect(Ray &ray)
{
  InterPointVector result, currResult;

  if (!isLoaded)
    return result;

  for (unsigned i = 0; i < facets.size(); i++)
  {
    currResult = facets[i].Intersect(ray);
    if (currResult.size() != 0)
      result.push_back(currResult[0]); // we have only 1 point - on triangle
  }

  return result;
}