#include "vec.h"
#include "utils.h"


/* a * x^2 + b * x + c = 0 */
RootVector SolveEquation2Deg(float a, float b, float c, bool nonNegativeAnswer)
{
  RootVector roots;
  float det = b * b - 4 * a * c, x1, x2;

  if (det < -EPSILON) // No real roots
    return roots;
  else if (det >= -EPSILON && det < EPSILON) 
    roots.push_back(-b / (2.0f * a));
  else
  {
    x1 = (-b - sqrt(det)) / (2.0f * a);
    x2 = (-b + sqrt(det)) / (2.0f * a);
    if (!nonNegativeAnswer || x1 >= EPSILON)
    {
      roots.push_back(x1);
    }
    if (!nonNegativeAnswer || x2 >= EPSILON)
    {
      roots.push_back(x2);
    }
    
  }

  return roots;
}

/* a * x^3 + b * x^2 + c * x + d = 0 */
RootVector SolveEquation3Deg(float a, float b, float c, float d, bool nonNegativeAnswer)
{    
  RootVector roots;       
  double  sub;  
  double  A, B, C;  
  double  sq_A, p, q;  
  double  cb_p, D;  
  
  /* normal form: x^3 + Ax^2 + Bx + C = 0 */  
  
  A = b / a;  
  B = c / a;  
  C = d / a;  
  
  /*  substitute x = y - A/3 to eliminate quadric term: 
  x^3 +px + q = 0 */  
  
  sq_A = A * A;  
  p = 1.0/3 * (- 1.0/3 * sq_A + B);  
  q = 1.0/2 * (2.0/27 * A * sq_A - 1.0/3 * A * B + C);  
  
  /* use Cardano's formula */  
  
  cb_p = p * p * p;  
  D = q * q + cb_p;  
  
  if (D >= -EPSILON && D < EPSILON)  
  {  
    if (q >= -EPSILON && q < EPSILON) /* one triple solution */  
    {  
      roots.push_back(0);  
    }  
    else /* one single and one double solution */  
    {  
      double u = cbrt(-q);  
      roots.push_back(2 * (float)u);  
      roots.push_back(-(float)u);  
    }  
  }  
  else if (D < -EPSILON) /* Casus irreducibilis: three real solutions */  
  {  
    double phi = 1.0/3 * acos(-q / sqrt(-cb_p));  
    double t = 2 * sqrt(-p);  
  
    roots.push_back((float)t * cosf((float)phi));  
    roots.push_back(-(float)t * cosf((float)phi + PI / 3));  
    roots.push_back(-(float)t * cosf((float)phi - PI / 3));  
  }  
  else /* one real solution */  
  {  
    double sqrt_D = sqrt(D);  
    double u = cbrt(sqrt_D - q);  
    double v = - cbrt(sqrt_D + q);  
  
    roots.push_back((float)(u + v)); 
  }  
  
  /* resubstitute */    
  sub = 1.0 / 3 * A;  
  
  for (unsigned i = 0; i < roots.size(); ++i)  
    roots[i] -= (float)sub;

  return roots;
}


/* a * x^4 + b * x^3 + c * x^2 + d * x + e = 0 */
RootVector SolveEquation4Deg(float a, float b, float c, float d, float e, bool nonNegativeAnswer)
{   
  RootVector cubicRoots, quadricRoots, roots;

  double  z, u, v, sub;  
  double  A, B, C, D;  
  double  sq_A, p, q, r;  
  
  /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */    
  A = b / a;  
  B = c / a;  
  C = d / a;  
  D = e / a;  
  
  /*  substitute x = y - A/4 to eliminate cubic term: 
  x^4 + px^2 + qx + r = 0 */    
  sq_A = A * A;  
  p = - 3.0 / 8 * sq_A + B;  
  q = 1.0 / 8 * sq_A * A - 1.0/2 * A * B + C;  
  r = - 3.0 / 256 * sq_A * sq_A + 1.0 / 16 * sq_A*B - 1.0/4 * A * C + D;  
  
  if (r >= -EPSILON && r < EPSILON)  
  {  
    /* no absolute term: y(y^3 + py + q) = 0 */       
    roots = SolveEquation3Deg(1.0f, 0.0f, (float)p, (float)q);    
    roots.push_back(0.0f);
  }  
  else  
  {  
    /* solve the resolvent cubic ... */  
    cubicRoots = SolveEquation3Deg(1.0f, -(float)p / 2.0f, -(float)r, (float)(r * p / 2.0f - q * q / 8.0f));
  
    /* ... and take the one real solution ... */  
    z = cubicRoots.back();
  
    /* ... to build two quadric equations */    
    u = z * z - r;  
    v = 2 * z - p;  
  
    if (u >= -EPSILON && u < EPSILON)  
      u = 0;  
    else if (u > 0)  
      u = sqrt(u);  
    else  
      return roots;  
  
    if (v >= -EPSILON && v < EPSILON)  
      v = 0;  
    else if (v > 0)  
      v = sqrt(v);  
    else  
      return roots;  
    
    quadricRoots = SolveEquation2Deg(1, q < 0 ? -(float)v : (float)v, (float)(z - u));
    for (unsigned i = 0; i < quadricRoots.size(); ++i)  
      roots.push_back(quadricRoots[i]);

    quadricRoots = SolveEquation2Deg(1, q < 0 ? (float)v : -(float)v, (float)(z + u));
    for (unsigned i = 0; i < quadricRoots.size(); ++i)  
      roots.push_back(quadricRoots[i]);
  }  
  
  /* resubstitute */    
  sub = 1.0/4 * A;    
  for (unsigned i = 0; i < roots.size(); ++i)  
    roots[i] -= (float)sub;

  return roots;
}