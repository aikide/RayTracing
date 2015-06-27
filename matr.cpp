/* Edward Burkov, 2057/2, 30.05.2013 */

#include <math.h>

#include "matr.h"


/**************************/
/** Additional functions **/
/**************************/


/* Create translation Matrix.
 * ARGUMENTS:
 * - translation arguments by X, Y, Z
 *     float Dx, Dy, Dz;
 * RETURNS: 
 * - translation matrix 
 *     Matrix ;
 */
Matrix CreateTranslation( float dX, float dY, float dZ )
{
  Matrix trans;

  trans.SetElement(3, 0, dX);
  trans.SetElement(3, 1, dY);
  trans.SetElement(3, 2, dZ);
  
  return trans;
} /* End of 'CreateTranslation' function */


/* Create 'rotation around X axis' Matrix.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float Angle;
 * RETURNS: 
 * - rotation matrix around axis X
 *     Matrix ;
 */
Matrix CreateRotationX( float angle )
{
  Matrix rot;
  float ang = D2R(angle), 
    si = sin(ang), 
    co = cos(ang);

  rot.SetElement(1, 1, co);
  rot.SetElement(1, 2, si);
  rot.SetElement(2, 1, -si);
  rot.SetElement(2, 2, co);

  return rot;
} /* End of 'CreateRotationX' function */


/* Create 'rotation around Y axis' Matrix.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float Angle;
 * RETURNS: 
 * - rotation matrix around axis Y
 *     Matrix ;
 */
Matrix CreateRotationY( float angle )
{
  Matrix rot;
  float ang = D2R(angle), si = sin(ang), co = cos(ang);

  rot.SetElement(0, 0, co);
  rot.SetElement(2, 0, si);
  rot.SetElement(0, 2, -si);
  rot.SetElement(2, 2, co);

  return rot;
} /* End of 'CreateRotationY' function */

/* Create 'rotation around Z axis' Matrix.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float Angle;
 * RETURNS: 
 * - rotation matrix around axis Z
 *     Matrix ;
 */
Matrix CreateRotationZ( float angle )
{
  Matrix rot;
  float ang = D2R(angle), si = sin(ang), co = cos(ang);

  rot.SetElement(0, 0, co);
  rot.SetElement(0, 1, si);
  rot.SetElement(1, 0, -si);
  rot.SetElement(1, 1, co);

  return rot;
} /* End of 'CreateRotationZ' function */


/* Create rotation Matrix.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float angle;
 * - rotation vector
 *     Vec radVec;
 * RETURNS: 
 * - rotation matrix
 *     Matrix ;
 */
Matrix CreateRotation( float angle, Vec &radVec )
{
  Matrix rot;
  /**/float si = sin(D2R(angle)), co = cos(D2R(angle)), len,
    radX = radVec.getX(), radY = radVec.getY(), radZ = radVec.getZ();  

  len = !radVec;
  if (len == 0)
    len = 1;
 
  radX /= len;
  radY /= len;
  radZ /= len;
		
  rot.SetElement(0, 0, co + radX * radX * (1 - co));
  rot.SetElement(0, 1, radX * radY * (1 - co) - radZ * si);
  rot.SetElement(0, 2, radX * radZ * (1 - co) + radY * si);

  rot.SetElement(1, 0, radX * radY * (1 - co) + radZ * si);
  rot.SetElement(1, 1, co + radY * radY * (1 - co));
  rot.SetElement(1, 2, radZ * radY * (1 - co) - radX * si);

  rot.SetElement(2, 0, radX * radZ * (1 - co) - radY * si);
  rot.SetElement(2, 1, radZ * radY * (1 - co) + radX * si);
  rot.SetElement(2, 2, co + radZ * radZ * (1 - co));/*


  float si = sin(D2R(angle / 2)), co = cos(D2R(angle / 2)), len,  // œŒÀŒ¬»ÕÕ€… ”√ŒÀ!!!
    radX = radVec.getX(), radY = radVec.getY(), radZ = radVec.getZ();

  len = !radVec;
  if (len == 0)
    len = 1;
 
  radX *= si / len;
  radY *= si / len;
  radZ *= si / len;
		
  rot.SetElement(0, 0, 1 - 2 * (radY * radY + radZ * radZ));
  rot.SetElement(0, 1, 2 * radX * radY - 2 * co * radZ);
  rot.SetElement(0, 2, 2 * co * radY + 2 * radX * radZ);

  rot.SetElement(1, 0, 2 * radX * radY + 2 * co * radZ);
  rot.SetElement(1, 1, 1 - 2 * (radX * radX + radZ * radZ));
  rot.SetElement(1, 2, 2 * radY * radZ - 2 * co * radX);

  rot.SetElement(2, 0, 2 * radX * radZ - 2 * co * radY);
  rot.SetElement(2, 1, 2 * co * radX + 2 * radY * radZ);
  rot.SetElement(2, 2, 1 - 2 * (radX * radX + radY * radY));/**/

  return rot;
} /* End of 'CreateRotation' function */


/* Create scaling Matrix.
 * ARGUMENTS:
 * - scaling arguments
 *     float Sx, Sy, Sz;
 * RETURNS:
 * - rotation matrix
 *     Matrix ;
 */
Matrix CreateScaling( float sX, float sY, float sZ )
{
  Matrix scale;

  scale.SetElement(0, 0, sX);
  scale.SetElement(1, 1, sY);
  scale.SetElement(2, 2, sZ);

  return scale;
} /* End of 'CreateScaling' function */


/* Swap arguments elem1 and elem2 */
static void _swap( float *elem1, float *elem2 )
{
  float tempElem = *elem1;

  *elem1 = *elem2;
  *elem2 = tempElem;
} /* End of '_swap' function */


/*********************************/
/** FUNCTIONS OF CLASS 'Matrix' **/
/*********************************/


/* Constructor #1.
 * By default, a
 */
Matrix::Matrix( float M00, float M01, float M02, float M03,  
                float M10, float M11, float M12, float M13,  
                float M20, float M21, float M22, float M23,  
                float M30, float M31, float M32, float M33)
{
  M[0][0] = M00;
  M[0][1] = M01;
  M[0][2] = M02;
  M[0][3] = M03;

  M[1][0] = M10;
  M[1][1] = M11;
  M[1][2] = M12;
  M[1][3] = M13;

  M[2][0] = M20;
  M[2][1] = M21;
  M[2][2] = M22;
  M[2][3] = M23;
    
  M[3][0] = M30;
  M[3][1] = M31;
  M[3][2] = M32;
  M[3][3] = M33;
} /* End of constructor #1 */


/* —onstructor #2 
 * float **_m - double array of points (4x4)
 */
Matrix::Matrix( float **_m )
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      M[i][j] = _m[i][j];
} /* End of constructor #2 */


/* —onstructor #3
 * float *_m - array of 16 points in order: {Row1, Row2, Row3, Row4}
 */
Matrix::Matrix( float *_m )
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      M[i][j] = _m[4 * i + j];
} /* End of constructor #3 */


/* Set unit matrix.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
void Matrix::SetUnity( void )
{
  M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
  M[0][1] = M[1][0] = M[2][0] = M[3][0] = 0;
  M[0][2] = M[1][2] = M[2][1] = M[3][1] = 0;
  M[0][3] = M[1][3] = M[2][3] = M[3][2] = 0;
} /* End of 'SetUnity' function */


/* Set unit matrix.
 * ARGUMENTS: 
 * - (already created) array to fill
 *     float *matrix;
 * RETURNS: None.
*/
void Matrix::FillData( float *matrix )
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      matrix[4 * i + j] = M[i][j];
} /* End of 'FillData' function */


float Matrix::operator()( int row, int column )
{
  if (column > 3 || row > 3 || column < 0 || row < 0)
    return 0.0;
  return M[row][column];
}


void Matrix::SetElement( int row, int column, float value )
{
  if (!(column > 3 || row > 3 || column < 0 || row < 0))
    M[row][column] = value;
}


/* Multiplicate 2 Matrices (*this * m)
 * ARGUMENTS:
 * - Matrix to mull from right
 *     Matrix &m;
 * RETURNS:
 * - itself
 *     & Matrix;
 */
Matrix &Matrix::operator*=( Matrix &m )
{
  Matrix tempMatr = Matrix(M[0][0] * m(0, 0) + M[0][1] * m(1, 0) + M[0][2] * m(2, 0) + M[0][3] * m(3, 0),
                           M[0][0] * m(0, 1) + M[0][1] * m(1, 1) + M[0][2] * m(2, 1) + M[0][3] * m(3, 1),
                           M[0][0] * m(0, 2) + M[0][1] * m(1, 2) + M[0][2] * m(2, 2) + M[0][3] * m(3, 2),
                           M[0][0] * m(0, 3) + M[0][1] * m(1, 3) + M[0][2] * m(2, 3) + M[0][3] * m(3, 3),

                           M[1][0] * m(0, 0) + M[1][1] * m(1, 0) + M[1][2] * m(2, 0) + M[1][3] * m(3, 0),
                           M[1][0] * m(0, 1) + M[1][1] * m(1, 1) + M[1][2] * m(2, 1) + M[1][3] * m(3, 1),
                           M[1][0] * m(0, 2) + M[1][1] * m(1, 2) + M[1][2] * m(2, 2) + M[1][3] * m(3, 2),
                           M[1][0] * m(0, 3) + M[1][1] * m(1, 3) + M[1][2] * m(2, 3) + M[1][3] * m(3, 3),

                           M[2][0] * m(0, 0) + M[2][1] * m(1, 0) + M[2][2] * m(2, 0) + M[2][3] * m(3, 0),
                           M[2][0] * m(0, 1) + M[2][1] * m(1, 1) + M[2][2] * m(2, 1) + M[2][3] * m(3, 1),
                           M[2][0] * m(0, 2) + M[2][1] * m(1, 2) + M[2][2] * m(2, 2) + M[2][3] * m(3, 2),
                           M[2][0] * m(0, 3) + M[2][1] * m(1, 3) + M[2][2] * m(2, 3) + M[2][3] * m(3, 3),

                           M[3][0] * m(0, 0) + M[3][1] * m(1, 0) + M[3][2] * m(2, 0) + M[3][3] * m(3, 0),
                           M[3][0] * m(0, 1) + M[3][1] * m(1, 1) + M[3][2] * m(2, 1) + M[3][3] * m(3, 1),
                           M[3][0] * m(0, 2) + M[3][1] * m(1, 2) + M[3][2] * m(2, 2) + M[3][3] * m(3, 2),
                           M[3][0] * m(0, 3) + M[3][1] * m(1, 3) + M[3][2] * m(2, 3) + M[3][3] * m(3, 3));
  *this = tempMatr;

  return *this;
} /* End of 'operator*=' function */


/* Translate matrix.
 * ARGUMENTS:
 * - translation arguments by X, Y, Z
 *     float dX, dY, dZ;
 * RETURNS: 
 * - translated matrix 
 *     Matrix &;
 */
Matrix &Matrix::Translate( float dX, float dY, float dZ )
{
  Matrix temp = CreateTranslation(dX, dY, dZ);
  
  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'Translate' function */


/* Rotate matrix around X axis.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float angle;
 * RETURNS: 
 * - rotated matrix around axis X
 *     Matrix &;
 */
Matrix &Matrix::RotateX( float angle )
{
  Matrix temp = CreateRotationX(angle);

  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'RotateX' function */


/* Rotate matrix around Y axis.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float angle;
 * RETURNS: 
 * - rotated matrix around axis Y
 *     Matrix &;
 */
Matrix &Matrix::RotateY( float angle )
{
  Matrix temp = CreateRotationY(angle);

  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'RotateY' function */


/* Rotate matrix around Z axis.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float angle;
 * RETURNS: 
 * - rotated matrix around axis Z
 *     Matrix &;
 */
Matrix &Matrix::RotateZ( float angle )
{
  Matrix temp = CreateRotationZ(angle);

  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'RotateZ' function */


/* Rotate matrix.
 * ARGUMENTS:
 * - rotation angle in degrees
 *     float angle;
 * - rotation vector
 *     Vec &radVec;
 * RETURNS: 
 * - rotated matrix
 *     Matrix &;
 */
Matrix &Matrix::Rotate( float angle, Vec &radVec )
{
  Matrix temp = CreateRotation(angle, radVec);

  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'Rotate' function */


/* Scale matrix.
 * ARGUMENTS:
 * - translation arguments by X, Y, Z
 *     float sX, sY, sZ;
 * RETURNS: 
 * - scaled matrix 
 *     Matrix &;
 */
Matrix &Matrix::Scale( float sX, float sY, float sZ )
{
  Matrix temp = CreateScaling(sX, sY, sZ);

  temp *= *this;
  *this = temp;

  return *this;
} /* End of 'Scale' function */


/* Transpose matrix.
 * ARGUMENTS: None.
 * RETURNS: 
 * - transposed matrix
 *     Matrix &;
 */
Matrix &Matrix::Transpose( void )
{
  _swap(&M[0][1], &M[1][0]);
  _swap(&M[0][2], &M[2][0]);
  _swap(&M[0][3], &M[3][0]);
  _swap(&M[1][2], &M[2][1]);
  _swap(&M[1][3], &M[3][1]);
  _swap(&M[2][3], &M[3][2]);

  return *this;
} /* End of 'Transpose' function */


/* Count determinamt of matrix 3x3.
 * ARGUMENTS:
 * - matrix elements
 *     float M00, M01, M02, 
 *         M10, M11, M12, 
 *         M20, M21, M22;
 * RETURNS:
 * - determinant value
 *     float;
 */
float _Det3x3( float M00, float M01, float M02, 
               float M10, float M11, float M12,
               float M20, float M21, float M22 )
{
  return M00 * M11 * M22 +
         M01 * M12 * M20 +
         M02 * M10 * M21 - 
         M02 * M11 * M20 -
         M01 * M10 * M22 -
         M00 * M12 * M21;
} /* End of '_Det3x3' function */


/* Count determinamt of Matrix 4x4.
 * ARGUMENTS: None.
 * RETURNS:
 * - determinant value
 *     float;
 */
float Matrix::Determinant( void )
{
 return M[0][0] * _Det3x3(M[1][1], M[1][2], M[1][3], M[2][1], M[2][2], M[2][3], M[3][1], M[3][2], M[3][3]) -
	M[0][1] * _Det3x3(M[1][0], M[1][2], M[1][3], M[2][0], M[2][2], M[2][3], M[3][0], M[3][2], M[3][3]) + 
	M[0][2] * _Det3x3(M[1][0], M[1][1], M[1][3], M[2][0], M[2][1], M[2][3], M[3][0], M[3][1], M[3][3]) -
	M[0][3] * _Det3x3(M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2], M[3][0], M[3][1], M[3][2]);
         
} /* End of 'Determinant' function */


/* Find inverse Matrix.
 * ARGUMENTS: None.
 * RETURNS: 
 * - inversed matrix
 *     Matrix &;
 */
Matrix &Matrix::Inverse( void )
{
  Matrix res;
  float Determ;

  if((Determ = this->Determinant()) == 0)
    return *this;
  
  res = Matrix(_Det3x3(M[1][1], M[1][2], M[1][3], M[2][1], M[2][2], M[2][3], M[3][1], M[3][2], M[3][3]) / Determ,
               -_Det3x3(M[1][0], M[1][2], M[1][3], M[2][0], M[2][2], M[2][3], M[3][0], M[3][2], M[3][3]) / Determ,
               _Det3x3(M[1][0], M[1][1], M[1][3], M[2][0], M[2][1], M[2][3], M[3][0], M[3][1], M[3][3]) / Determ,
               -_Det3x3(M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2], M[3][0], M[3][1], M[3][2]) / Determ,
               
               -_Det3x3(M[0][1], M[0][2], M[0][3], M[2][1], M[2][2], M[2][3], M[3][1], M[3][2], M[3][3]) / Determ,
               _Det3x3(M[0][0], M[0][2], M[0][3], M[2][0], M[2][2], M[2][3], M[3][0], M[3][2], M[3][3]) / Determ,
               -_Det3x3(M[0][0], M[0][1], M[0][3], M[2][0], M[2][1], M[2][3], M[3][0], M[3][1], M[3][3]) / Determ,               
               _Det3x3(M[0][0], M[0][1], M[0][2], M[2][0], M[2][1], M[2][2], M[3][0], M[3][1], M[3][2]) / Determ,

               _Det3x3(M[0][1], M[0][2], M[0][3], M[1][1], M[1][2], M[1][3], M[3][1], M[3][2], M[3][3]) / Determ,
               -_Det3x3(M[0][0], M[0][2], M[0][3], M[1][0], M[1][2], M[1][3], M[3][0], M[3][2], M[3][3]) / Determ,
               _Det3x3(M[0][0], M[0][1], M[0][3], M[1][0], M[1][1], M[1][3], M[3][0], M[3][1], M[3][3]) / Determ,
               -_Det3x3(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2], M[3][0], M[3][1], M[3][2]) / Determ,

               -_Det3x3(M[0][1], M[0][2], M[0][3], M[1][1], M[1][2], M[1][3], M[2][1], M[2][2], M[2][3]) / Determ,
               _Det3x3(M[0][0], M[0][2], M[0][3], M[1][0], M[1][2], M[1][3], M[2][0], M[2][2], M[2][3]) / Determ,
               -_Det3x3(M[0][0], M[0][1], M[0][3], M[1][0], M[1][1], M[1][3], M[2][0], M[2][1], M[2][3]) / Determ,               
               _Det3x3(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2]) / Determ);
  res.Transpose();
  *this = res;

  return *this;
} /* End of 'Inverse' function */


/* Multiplication vector and matrix. (*this * v)
 * ARGUMENTS:
 * - vector to multiply from right
 *     Vec v;  
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Matrix::operator*( Vec &v )
{
  Vec res = Vec(v.getX() * M[0][0] + v.getY() * M[1][0] + v.getZ() * M[2][0] + M[3][0],
  	            v.getX() * M[0][1] + v.getY() * M[1][1] + v.getZ() * M[2][1] + M[3][1],
	              v.getX() * M[0][2] + v.getY() * M[1][2] + v.getZ() * M[2][2] + M[3][2]);

  return res;
} /* End of 'operator*' function */



/* Multiplication vector and matrix. (vec * *this)
 * ARGUMENTS:
 * - vector to multiply from right
 *     Vec &v;  
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Matrix::MultiplyVectorFromLeft( Vec &v )
{  
  Vec res = Vec(v.getX() * M[0][0] + v.getY() * M[0][1] + v.getZ() * M[0][2] + M[0][3],
  	            v.getX() * M[1][0] + v.getY() * M[1][1] + v.getZ() * M[1][2] + M[1][3],
	              v.getX() * M[2][0] + v.getY() * M[2][2] + v.getZ() * M[2][2] + M[2][3]);
             
  return res;
} /* End of 'operator*' function */



