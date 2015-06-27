#include "camera.h"


Camera::Camera()
{
  lookAt = Vec(-1.0f, -1.0f, -1.0f);
  eye = Vec(1.0f, 1.0f, 1.0f);
  up = Vec(0.0f, 1.0f, 0.0f);
  right = Vec(1.0f, 0.0f, 0.0f);
  UpdateCamera();
}

Camera::~Camera()
{
}

void Camera::SetFOV(float _fovX, float _fovY)
{
  if (_fovX >= 0)
    fovX = _fovX;
  
  if (_fovY >= 0)
    fovY = _fovY;
}


void Camera::UpdateCamera()
{  
  lookAt.Normalize();
  right = (up % lookAt).Normalize();
  up = (lookAt % right).Normalize();
  
  viewMatrix.SetElement(0, 0, right.getX());
  viewMatrix.SetElement(1, 0, right.getY());
  viewMatrix.SetElement(2, 0, right.getZ());
  viewMatrix.SetElement(3, 0, -(eye & right));

  viewMatrix.SetElement(0, 1, up.getX());
  viewMatrix.SetElement(1, 1, up.getY());
  viewMatrix.SetElement(2, 1, up.getZ());
  viewMatrix.SetElement(3, 1, -(eye & up));

  viewMatrix.SetElement(0, 2, lookAt.getX());
  viewMatrix.SetElement(1, 2, lookAt.getY());
  viewMatrix.SetElement(2, 2, lookAt.getZ());
  viewMatrix.SetElement(3, 2, -(eye & lookAt));

  viewMatrix.SetElement(0, 3, 0.0f);
  viewMatrix.SetElement(1, 3, 0.0f);
  viewMatrix.SetElement(2, 3, 0.0f);
  viewMatrix.SetElement(3, 3, 1.0f);

  invViewMatrix = viewMatrix;
  invViewMatrix.Inverse();
}


void Camera::SetupCamera()
{
  viewMatrix.SetUnity();
  viewMatrix.Translate(-eye.getX(), -eye.getY(), -eye.getZ());
  viewMatrix.RotateX(-dirAngle.getX()).RotateY(-dirAngle.getY()).RotateZ(-dirAngle.getZ());

  invViewMatrix = viewMatrix;
  invViewMatrix.Inverse();
}