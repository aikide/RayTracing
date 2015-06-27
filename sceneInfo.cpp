#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>
#include <algorithm>

#include "sceneInfo.h"


SceneInfo::SceneInfo()
{
  sceneFile = NULL;
  resX = 640;
  resY = 480;
  traceDepth = 100;
}


SceneInfo::~SceneInfo()
{
}

/* true - has errors, false - doesn't have errors */
bool SceneInfo::ParseFile(char *fileName)
{
  bool exitFlag = false, 
    getNewLine = true, 
    currError = false;  
  char *returnedLine = NULL;
  int currPos = 0;
  errno_t errCode = 0;
    
  errCode = fopen_s(&sceneFile, fileName, "rb");

  if (errCode != 0)
  {
    printf("\n'SceneInfo::ParseFile': Error # %i. Can't read file %s.\n", errCode, fileName);
    return true;
  }

  while (!exitFlag)
  {
    if (buf[currPos] == '#' || buf[currPos] == '\n' || currPos >= 255) // comment or end-of-line
      getNewLine = true;
    else if (buf[currPos] == ' ')
      ++currPos;
    else if (buf[currPos] == 0)
      exitFlag = true;
    else if (buf[currPos] == '-')
    {
      if (strncmp(buf + currPos, "- camera:\n", 10) == 0) // Camera node
        currError = ParseFileCameraNode();               
      else if (strncmp(buf + currPos, "- node:\n", 8) == 0) // Usual node
        currError = ParseFileUsualNode();
      else
        ;
    }
    
    if (getNewLine)
    {    
      returnedLine = fgets(buf, 256, sceneFile);
      if (returnedLine == NULL)
        exitFlag = true;

      currPos = 0;
      getNewLine = false;
    }
    if (currError != false)
      return currError;
  }

  return false;
}


bool SceneInfo::ParseFileCameraNode()
{
  int currPos = 0;
  bool exitFlag = false, 
    getNewLine = true, 
    currError = false;
  char *returnedLine = NULL;
  float tempFOVX = 0, tempFOVY = 0;  
  
  while (!exitFlag)
  {
    if (buf[currPos] == '#' || buf[currPos] == '\n' || currPos >= 255) // comment or end-of-line
      getNewLine = true;
    else if (buf[currPos] == ' ')
      ++currPos;
    else if (buf[currPos] == 0 || (buf[currPos] == '-' && getNewLine == false))
      exitFlag = true;
    
    /* Position */
    else if (strncmp(buf + currPos, "position:", 9) == 0)
    {
      float tempX = 0, tempY = 0, tempZ = 0;
      sscanf_s(buf + currPos + 9 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
      camera.SetEye(tempX, tempY, tempZ);
      getNewLine = true;
    }
    /* Orientation */
    else if (strncmp(buf + currPos, "orientation:", 12) == 0)
    {
      float tempH = 0, tempP = 0, tempR = 0;
      sscanf_s(buf + currPos + 12 + 1, "{h: %f, p: %f, r: %f}", &tempH, &tempP, &tempR);
      camera.SetDirAngle(tempH, tempP, tempR); // convert to degrees - DO NOT NEED!!!
      getNewLine = true;
    }
    /* FOV X */
    else if (strncmp(buf + currPos, "fov_x:", 6) == 0)
    {
      tempFOVX = strtof(buf + currPos + 6 + 1, NULL) / 2.0f;
      camera.SetFOV(tempFOVX, -1);
      getNewLine = true;
    }
    /* FOV Y */
    else if (strncmp(buf + currPos, "fov_y:", 6) == 0)
    {
      tempFOVY = strtof(buf + currPos + 6 + 1, NULL) / 2.0f;
      camera.SetFOV(-1, tempFOVY);
      getNewLine = true;
    }        
    
    if (getNewLine)
    {    
      returnedLine = fgets(buf, 256, sceneFile);
      if (returnedLine == NULL)
        exitFlag = true;

      currPos = 0;
      getNewLine = false;
    }
    if (currError != false)
      return currError;
  }
  
  SetupSceneScreen(tempFOVX, tempFOVY);
  camera.SetupCamera();

  return false;
}


void SceneInfo::SetupSceneScreen(float fovX, float fovY)
{
  float tanX = tan(fovX * PI / 180.0f), tanY = tan(fovY * PI / 180.0f),
        distX = resX / tanX, distY = resY / tanY;  

  dist = __min(distX, distY);
	width = (int)(dist * tanX);
	height = (int)(dist * tanY);
  deltaX = (int)((resX - dist * tanX) / 2.0f);
	deltaY = (int)((resY - dist * tanY) / 2.0f);	
}


bool SceneInfo::ParseFileUsualNode()
{
  bool exitFlag = false, 
    getNewLine = true, 
    currError = false,
    newLevel = true;  
  int currPos = 0;
  char *returnedLine = NULL;
  PrimType primType = PRIM_NONE;
  std::stack<SceneObject *> objectStack;
  std::stack<int> indentStack;
  SceneObject *currObject = NULL;

  while (!exitFlag)
  {
    if (buf[currPos] == '#' || buf[currPos] == '\n' || currPos >= 255) // comment or end-of-line
      getNewLine = true;
    else if (buf[currPos] == ' ')
      ++currPos;
    else if (buf[currPos] == 0)
      exitFlag = true;
    else if (buf[currPos] == '-')
    {      
      /* Analise data in node */
      if (strncmp(buf + currPos, "- lcs:", 6) == 0)
      {
        float tempX = 0, tempY = 0, tempZ = 0, 
          tempH = 0, tempP = 0, tempR = 0,
          tempSX = 1.0f, tempSY = 1.0f, tempSZ = 1.0f;
        
        sscanf_s(buf + currPos + 6 + 1, "{x: %f, y: %f, z: %f, h: %f, p: %f, r: %f, sx: %f, sy: %f, sz: %f}", 
          &tempX, &tempY, &tempZ, &tempH, &tempP, &tempR, &tempSX, &tempSY, &tempSZ);
        currObject->trans.Translate(tempX, tempY, tempZ).
          RotateX(tempH).RotateY(tempP).RotateZ(tempR).
          Scale(1.0f / tempSX, 1.0f / tempSY, 1.0f / tempSZ);                  
                
        if (objectStack.size() != 0)
          currObject->trans *= objectStack.top()->trans;
        currObject->invTrans = currObject->trans;
        currObject->invTrans.Inverse();
        
        if (newLevel)
        {
          //matrixStack.push(&currObject->trans);
          newLevel = false;
        }
          
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- node:", 7) == 0) // Child node
      {
        primType = PRIM_NONE;      

        if (currObject != NULL)
        {
          if (currPos > indentStack.top())
            objectStack.push(currObject);
          sceneObjects.push_back(currObject);
        }
        currObject = new SceneObject();

        while (indentStack.size() != 0 && currPos < indentStack.top())
        {
          indentStack.pop();
          objectStack.pop();
        }
        if (indentStack.size() == 0 || currPos > indentStack.top())
        {
          indentStack.push(currPos);        
          newLevel = true;
        }
        else
          newLevel = false;
          
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- plane:", 8) == 0) // Plane geometry
      {
        primType = PRIM_PLANE;
        currObject->prim = new Plane();
        getNewLine = true;
      }      
      else if (strncmp(buf + currPos, "- triangle:", 11) == 0) // Triangle geometry
      {
        primType = PRIM_TRIANGLE;
        currObject->prim = new Triangle();
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- sphere:", 9) == 0) // Sphere geometry
      {
        primType = PRIM_SPHERE;
        currObject->prim = new Sphere();
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- cylinder:", 11) == 0) // Cylinder geometry
      {
        primType = PRIM_CYLINDER;
        currObject->prim = new Cylinder();
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- cone:", 7) == 0) // Cone geometry
      {
        primType = PRIM_CONE;
        currObject->prim = new Cone();
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- torus:", 8) == 0) // Torus geometry
      {
        primType = PRIM_TORUS;
        currObject->prim = new Torus();
        getNewLine = true;
      }
      else if (strncmp(buf + currPos, "- obj_model:", 12) == 0) // Custom model geometry
      {
        primType = PRIM_OBJECT;
        currObject->prim = new Object();
        getNewLine = true;
      }
      else
        ;
      
    } 
    else // Other case
    {
      if (primType != PRIM_NONE)
      {
        currError = ParseFileGeometryNode(primType, currObject);
        currPos = 0;
      }
      else
        currError = true;
    }

    if (getNewLine)
    {    
      returnedLine = fgets(buf, 256, sceneFile);
      if (returnedLine == NULL)
        exitFlag = true;

      currPos = 0;
      getNewLine = false;
    }
    if (currError != false)
      return currError;
  }
  sceneObjects.push_back(currObject);
  
  return false;
}


bool SceneInfo::ParseFileGeometryNode(PrimType primType, SceneObject *currObject)
{  
  int currPos = 0, i = 0;
  bool exitFlag = false, 
    getNewLine = false, 
    currError = false;    
  char tempFileName[128], *returnedLine = NULL;
  float tempX, tempY, tempZ, tempRadius, tempHeight, tempTubeRadius;
  
  while (!exitFlag)
  {
    if (buf[currPos] == '#' || buf[currPos] == '\n' || currPos >= 255) // comment or end-of-line
      getNewLine = true;
    else if (buf[currPos] == ' ')
      ++currPos;
    else if (buf[currPos] == 0 || buf[currPos] == '-')
      exitFlag = true;
    else 
    {
      switch (primType)
      {
      case PRIM_TRIANGLE:        
        if (strncmp(buf + currPos, "p0:", 3) == 0) // point 0
        {          
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Triangle *)currObject->prim)->SetPoint(0, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else if (strncmp(buf + currPos, "p1:", 3) == 0) // point 1
        {
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Triangle *)currObject->prim)->SetPoint(1, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else if (strncmp(buf + currPos, "p2:", 3) == 0) // point 2
        {
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Triangle *)currObject->prim)->SetPoint(2, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else
          currError = true;
        break;
      case PRIM_PLANE:        
        if (strncmp(buf + currPos, "p0:", 3) == 0) // point 0
        {          
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Plane *)currObject->prim)->SetPoint(0, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else if (strncmp(buf + currPos, "p1:", 3) == 0) // point 1
        {
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Plane *)currObject->prim)->SetPoint(1, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else if (strncmp(buf + currPos, "p2:", 3) == 0) // point 2
        {
          sscanf_s(buf + currPos + 3 + 1, "{x: %f, y: %f, z: %f}", &tempX, &tempY, &tempZ);
          ((Plane *)currObject->prim)->SetPoint(2, tempX, tempY, tempZ);
          getNewLine = true;
        }
        else
          currError = true;

        break;
      case PRIM_SPHERE:        
        if (strncmp(buf + currPos, "radius:", 7) == 0) // radius
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempRadius);
          ((Sphere *)currObject->prim)->SetRadius(tempRadius);
          getNewLine = true;
        }        
        else
          currError = true;

        break;
      case PRIM_CYLINDER:
        if (strncmp(buf + currPos, "radius:", 7) == 0) // radius
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempRadius);
          ((Cylinder *)currObject->prim)->SetRadius(tempRadius);
          getNewLine = true;
        }        
        else if (strncmp(buf + currPos, "height:", 7) == 0) // height
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempHeight);
          ((Cylinder *)currObject->prim)->SetHeight(tempHeight);
          getNewLine = true;
        }        
        else
          currError = true;

        break;
      case PRIM_CONE:
       if (strncmp(buf + currPos, "radius:", 7) == 0) // radius
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempRadius);
          ((Cone *)currObject->prim)->SetRadius(tempRadius);
          getNewLine = true;
        }        
        else if (strncmp(buf + currPos, "height:", 7) == 0) // height
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempHeight);
          ((Cone *)currObject->prim)->SetHeight(tempHeight);
          getNewLine = true;
        }        
        else
          currError = true;

        break;
      case PRIM_TORUS:
       if (strncmp(buf + currPos, "radius:", 7) == 0) // radius
        {          
          sscanf_s(buf + currPos + 7 + 1, "%f", &tempRadius);
          ((Torus *)currObject->prim)->SetRadius(tempRadius);
          getNewLine = true;
        }        
        else if (strncmp(buf + currPos, "tube_radius:", 12) == 0) // tube radius
        {          
          sscanf_s(buf + currPos + 12 + 1, "%f", &tempTubeRadius);
          ((Torus *)currObject->prim)->SetTubeRadius(tempTubeRadius);
          getNewLine = true;
        }        
        else
          currError = true;

        break;
      case PRIM_OBJECT:
        if (strncmp(buf + currPos, "file_name:", 10) == 0) // file name
        {          
          while (buf[currPos + 10 + 2 + i] != '"')
          {
            tempFileName[i] = buf[currPos + 10 + 2 + i];          
            ++i;
          }
          tempFileName[i] = 0;
          ((Object *)currObject->prim)->SetFileName(tempFileName);
          getNewLine = true;
        }                        
        else
          currError = true;

        break;
      default:
        return true;
      }
    }
  
    if (getNewLine)
    {    
      returnedLine = fgets(buf, 256, sceneFile);
      if (returnedLine == NULL)
        exitFlag = true;

      currPos = 0;
      getNewLine = false;
    }    
    if (currError != false)
      return currError;
  }

  return false;
}

static bool _sortingPattern(const InterPoint &point1, const InterPoint &point2)
{
  return point1.dist < point2.dist;
}

void SortPoints(InterPointVector &intersectionData)
{
  std::sort(intersectionData.begin(), intersectionData.end(), _sortingPattern);
}


InterPointVector SceneInfo::FindIntersection(Ray &ray)
{
  InterPointVector intersectionData, currIntersectionData;
  Matrix viewMatrix = camera.GetViewMatrix(),
    invViewMatrix = camera.GetInverseViewMatrix();
  Ray cameraRay, objectRay;    

  /* To camera */
  cameraRay.direction = viewMatrix * ray.direction;
  cameraRay.origin = viewMatrix * ray.origin;
  
  for (unsigned i = 0; i < sceneObjects.size(); i++)
  {
    /* To world */
    objectRay.direction = sceneObjects[i]->trans * cameraRay.direction;
    objectRay.origin = sceneObjects[i]->trans * cameraRay.origin;

    currIntersectionData = sceneObjects[i]->prim->Intersect(objectRay);

    /* From world */
    for (unsigned j = 0; j < currIntersectionData.size(); j++)
    {
      currIntersectionData[j].point = sceneObjects[i]->invTrans * currIntersectionData[j].point;
      currIntersectionData[j].normal = sceneObjects[i]->invTrans * currIntersectionData[j].normal;      
      intersectionData.push_back(currIntersectionData[j]);
    }             
  }  
  /* From camera */
  for (unsigned j = 0; j < intersectionData.size(); j++)
  {
    intersectionData[j].point = invViewMatrix * intersectionData[j].point;
    intersectionData[j].normal = invViewMatrix * intersectionData[j].normal;    
  }

  SortPoints(intersectionData);    

  return intersectionData;
}


PointData SceneInfo::GetPointColor(Ray &ray)
{
  PointData pointData;
  InterPointVector intersectionData = FindIntersection(ray);

  if (intersectionData.size() != 0) // Or else, no intersections or intersection is dehind camera (< 0) => Black pixel
  {
    float color = !intersectionData[0].point;
    pointData.point = Color((unsigned char)(color > 255.0f ? 255.0f : color));
    pointData.normal = Color((int)fabs(intersectionData[0].normal.Normalize().getZ() * 255.0f));
  }

  return pointData;
}


void SceneInfo::TraceRays(Image *image, Image *normalImage)
{
  float x = 0, y = 0;
	Ray ray;
  PointData color;
	
  image->SetupImage(width, height);
  normalImage->SetupImage(width, height);
	for (int i = 0; i < width; i++) 
  {
		for (int j = 0; j < height; j++) 
    {
			x = i - width / 2.0f;
			y = j - height / 2.0f;
			ray.direction = Vec(x, dist, y);
						
      color = GetPointColor(ray);
			image->SetPixel(i + deltaX, j + deltaY, color.point);
			normalImage->SetPixel(i + deltaX, j + deltaY, color.normal);
		}
	}	
}


/** Image Class Functions **/
Image::Image(int _width, int _height)
{
  SetupImage(_width, _height);
}


void Image::SetupImage(int _width, int _height)
{
  width = _width;
  height = _height;
  pixels = new Color[width * height];
}


void Image::SetPixel(int x, int y, Color color)
{
  if (x >= 0 && x < width && y >= 0 && y < height)
    pixels[y * width + x] = color;
}


Color Image::GetPixel(int x, int y)
{
  if (x >= 0 && x < width && y >= 0 && y < height)
    return pixels[y * width + x];
  return Color();
}

/* Save to BMP file */
void Image::SaveImageToFile(char *fileName)
{
  FILE *imageFile;
  int imageSize = height * width, 
    fileSize = 54 + 3 * imageSize; // size of header + picture data
  unsigned char fileHeaderData[14] = 
    {'B', 'M', 
    (unsigned char)fileSize, (unsigned char)(fileSize >> 8), (unsigned char)(fileSize >> 16), (unsigned char)(fileSize >> 24),
    0, 0, 0, 0, 54, 0, 0, 0},
	infoHeaderData[40] = 
    {40, 0, 0, 0, 
    (unsigned char)width, (unsigned char)(width >> 8), (unsigned char)(width >> 16), (unsigned char)(width >> 24), 
    (unsigned char)height, (unsigned char)(height >> 8), (unsigned char)(height >> 16), (unsigned char)(height >> 24), 
    1, 0, 24, 0},
	endlinePaddingData[3] = {0, 0, 0}, paddingLen = (4 - (width * 3) % 4) % 4,
  imagePixelData[3] = {0, 0, 0};
  errno_t errCode = 0;
    
  errCode = fopen_s(&imageFile, fileName, "wb");

  if (errCode != 0)
  {
    printf("\n'Image::SaveImageToFile': Error # %i. Can't open file %s to write.\n", errCode, fileName);
    return;
  }	

	fwrite(fileHeaderData, 1, 14, imageFile);
	fwrite(infoHeaderData, 1, 40, imageFile);

	for (int j = height - 1; j >= 0; j--)
  {
    for (int i = 0; i < width; i++)
    {
      imagePixelData[0] = (unsigned char)pixels[j * width + i].r;
      imagePixelData[1] = (unsigned char)pixels[j * width + i].g;
      imagePixelData[2] = (unsigned char)pixels[j * width + i].b;
		  fwrite(imagePixelData, 1, 3, imageFile);		  
    }
    fwrite(endlinePaddingData, 1, paddingLen, imageFile);
	}

	fclose(imageFile);
}

Image::~Image()
{
  delete[] pixels;
}