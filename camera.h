/* Edward Burkov, 11-3, 04.05.2011 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "matr.h"


/*** Camera CLASS ***/
class Camera
{
private:
  Vec eye, lookAt, up, right, 
    dirAngle; // dirAngle - (dx, dy, dz)
  float fovX, fovY;
  Matrix viewMatrix, invViewMatrix;

public:
  Camera();
  
  void SetupCamera();
  void UpdateCamera();
  
  void SetEye(Vec &_eye) { eye = _eye; }
  void SetEye(float _x, float _y, float _z) { eye = Vec(_x, _y, _z); }
  Vec &GetEye() { return eye; }

  void SetDirAngle(Vec &_dirAngle) { _dirAngle = _dirAngle; }
  void SetDirAngle(float _h, float _p, float _r) { dirAngle = Vec(_h, _p, _r); }
  Vec &GetDirAngle() { return dirAngle; }

  void SetFOV(float _fovX = 60.0f, float _fovY = 30.0f);
  void GetFOV(float *_fovX, float *_fovY) { *_fovX = fovX, *_fovY = fovY; }

  Matrix &GetViewMatrix() { return viewMatrix; }
  Matrix &GetInverseViewMatrix() { return invViewMatrix; }

  ~Camera();

}; /* End of Camera class */
  

#endif /*_CAMERA_H_*/


/**********************************/
/***** END OF 'CAMERA.H' FILE *****/
/**********************************/
