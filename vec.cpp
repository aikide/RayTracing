#include <math.h>

#include "vec.h"


/* Vector constructor. 
 * ARGUMENTS:
 * - vector coordinates( default, 0 )
 *     float X, Y, Z;
 * RETURNS: None. 
 */
Vec::Vec( float x, float y, float z, float w )
{
  coord[0] = x;
  coord[1] = y;
  coord[2] = z;
  coord[3] = w;
} /* End of constructor */


float Vec::Angle( Vec &v )
{
  return (*this & v) / (!*this * !v);
}


float Vec::Distance( Vec &v )
{
  return !(v - *this);
}


/* Count vector's length.
 * ARGUMENTS: None.
 * RETURNS:
 * - vector's lenght
 *     float;
 */
float Vec::operator!( void )
{
  return sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
} /* End of 'operator!' function */


/* Self-multiplication vector and number.
  * ARGUMENTS:
  * - number
  *     float n;
  * RETURNS:
  * - pointer to itself
  *     * Vec;
  */
Vec &Vec::operator*=( float n ) 
{
  coord[0] *= n;
  coord[1] *= n;
  coord[2] *= n; 

  return *this;
} /* End of 'operator*=' function */


/* Self-division vector and number.
 * ARGUMENTS:
 * - number
 *   float n;
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
Vec &Vec::operator/=( float n ) 
{
  coord[0] /= n;
  coord[1] /= n;
  coord[2] /= n; 

  return *this;
} /* End of 'operator/=' function */


/* Vectors self-addition. 
 * ARGUMENTS:
 * - adding vector
 *     Vec v;
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
Vec &Vec::operator+=( Vec &v ) 
{
  coord[0] += v.coord[0];
  coord[1] += v.coord[1];
  coord[2] += v.coord[2];

  return *this;
} /* End of 'operator+=' function */

  
/* Vector self-subligation.
 * ARGUMENTS:
 * - subligated vector
 *     Vec v;
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
Vec &Vec::operator-=( Vec &v ) 
{
  coord[0] -= v.coord[0];
  coord[1] -= v.coord[1];
  coord[2] -= v.coord[2];

  return *this;
} /* End of 'operator-=' function */


/* Scalar production of vectors.
 * ARGUMENTS:
 * - vector to product
 *     Vec v;
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
float Vec::operator&( Vec &v )
{
  return coord[0] * v.coord[0] + coord[1] * v.coord[1] + coord[2] * v.coord[2];
} /* End of 'operator&' function */


/* Self-vector production of vectors.
 * ARGUMENTS:
 * - vector to product 
 *     Vec v;
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
Vec &Vec::operator%=( Vec &v ) 
{     
  coord[0] = coord[1] * v.coord[2] - coord[2] * v.coord[1];
  coord[1] = coord[2] * v.coord[0] - coord[0] * v.coord[2];
  coord[2] = coord[0] * v.coord[1] - coord[1] * v.coord[0];

  return *this;
} /* End of 'operator%=' function */


/* Normilize vector. 
 * ARGUMENTS: None.
 * RETURNS:
 * - pointer to itself
 *     * Vec;
 */
Vec &Vec::Normalize( void )
{
  float len = !Vec(coord[0], coord[1], coord[2]);

  if (len != 0)
  {  
    coord[0] /= len;
    coord[1] /= len;
    coord[2] /= len;
  }

  return *this;
} /* End of 'Normalize' function */



/*** Version 2 ***/

/* Multiplication vector and number. Version 2.
 * ARGUMENTS:
 * - number
 *     float n;
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Vec::operator*( float n ) 
{ 
  return Vec(coord[0] * n, coord[1] * n, coord[2] * n);
} /* End of 'operator*' function */


/* Division vector and number. Version 2.
 * ARGUMENTS:
 * - number
 *   float n;
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Vec::operator/( float n ) 
{
  return Vec(coord[0] / n, coord[1] / n, coord[2] / n);
} /* End of 'operator/' function */


/* Vectors addition. Version 2.
 * ARGUMENTS:
 * - adding vector
 *     Vec &v;
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Vec::operator+( Vec &v ) 
{
  return Vec(coord[0] + v.getX(), coord[1] + v.getY(), coord[2] + v.getZ());
} /* End of 'operator+' function */

  
/* Vector subligation. Version 2.
 * ARGUMENTS:
 * - subligated vector
 *     Vec &v;
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Vec::operator-( Vec &v ) 
{
  return Vec(coord[0] - v.getX(), coord[1] - v.getY(), coord[2] - v.getZ());
} /* End of 'operator-' function */

  
/* Vector production of vectors. Version 2.
 * ARGUMENTS:
 * - vector to product 
 *     Vec &v;
 * RETURNS:
 * - result vector
 *     Vec;
 */
Vec Vec::operator%( Vec &v ) 
{     
  return Vec(coord[1] * v.getZ() - coord[2] * v.getY(), 
             coord[2] * v.getX() - coord[0] * v.getZ(), 
             coord[0] * v.getY() - coord[1] * v.getX());
} /* End of 'operator%' function */


/*********************************/
/***** END OF 'VEC.CPP' FILE *****/
/*********************************/