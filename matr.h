/*  FILE: Matr.H 
 *  DATE: 30.056.2013
 *  INITIAL PROJECT: EAS
 *  PURPOSE: Matrix library.
 */


#ifndef _MATR_H_
#define _MATR_H_

#include "vec.h"


/*** Matrix CLASS ***/               

class Matrix
{
private:
  float M[4][4];   /* Matrix like 2-sized array */
                        /*  00 01 02 03  */
                        /*  10 11 12 13  */
                        /*  20 21 22 23  */
                        /*  30 31 32 33  */

public:
  Matrix( float M00 = 1, float M01 = 0, float M02 = 0, float M03 = 0,  
          float M10 = 0, float M11 = 1, float M12 = 0, float M13 = 0, 
          float M20 = 0, float M21 = 0, float M22 = 1, float M23 = 0, 
          float M30 = 0, float M31 = 0, float M32 = 0, float M33 = 1 );
  Matrix( float **_m ); 
  Matrix( float *_m );

  void FillData( float *matrix );

  void SetUnity( void );
  void SetElement( int row, int column, float value );
  
  Matrix &operator*=( Matrix &m );
  float operator()( int row, int column );

  Matrix &Translate( float dX, float dY, float dZ );
  Matrix &Scale( float sX, float sY, float sZ );
  Matrix &RotateX( float angle );
  Matrix &RotateY( float angle );
  Matrix &RotateZ( float angle );
  Matrix &Rotate( float angle, Vec &radVec );
  
  Matrix &Transpose( void );
  float Determinant( void );
  Matrix &Inverse( void );
  
  Vec operator*( Vec &v );
  Vec MultiplyVectorFromLeft( Vec &v );

  ~Matrix( void )
  {}
}; /* End of Matrix class */

/*** Additional Functions ***/
Matrix CreateRotationX( float angle );
Matrix CreateRotationY( float angle );
Matrix CreateRotationZ( float angle );
Matrix CreateRotation( float angle, Vec &radVec );
Matrix CreateScaling( float sX, float sY, float sZ );

#endif /* _MATR_H_ */


/******************************/
/*    END OF 'MATR.H' FILE    */ 
/******************************/