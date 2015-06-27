/* Edward Burkov, 2057/2, 14.05.2013 */

#ifndef _VEC_H_
#define _VEC_H_

#define PI 3.14159265358979323846f
#define D2R(a) ((PI / 180.0f) * a)

/*** VECTOR CLASS ***/
class Vec
{
private:
  float coord[4];

public:
  Vec( float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f );

  float getX( void ) { return coord[0]; }
  float getY( void ) { return coord[1]; }
  float getZ( void ) { return coord[2]; }
  float getW( void ) { return coord[3]; }

  Vec &Normalize( void );
  float operator!( void );
  float operator&( Vec &v );
  float Angle( Vec &v );
  float Distance( Vec &v );
  
  Vec &operator*=( float n );
  Vec &operator/=( float n );
  Vec &operator+=( Vec &v );
  Vec &operator-=( Vec &v );
  Vec &operator%=( Vec &v );

  Vec operator*( float n );
  Vec operator/( float n );
  Vec operator+( Vec &v );
  Vec operator-( Vec &v );
  Vec operator%( Vec &v );
}; /* End of Vec class */

#endif /* _VEC_H_ */


/*******************************/
/***** END OF 'VEC.H' FILE *****/
/*******************************/