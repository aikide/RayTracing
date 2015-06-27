#ifndef __UTILS_H__
#define __UTILS_H__

#include <math.h>
#include <vector>

#define EPSILON 1.192092896e-07F

typedef std::vector<float> RootVector;

/* a * x^2 + b * x + c = 0 */
RootVector SolveEquation2Deg(float a, float b, float c, bool nonNegativeAnswer = false); 

/* a * x^3 + b * x^2 + c * x + d = 0 */
RootVector SolveEquation3Deg(float a, float b, float c, float d, bool nonNegativeAnswer = false);

/* a * x^4 + b * x^3 + c * x^2 + d * x + e = 0 */
RootVector SolveEquation4Deg(float a, float b, float c, float d, float e, bool nonNegativeAnswer = false);


#endif /* __UTILS_H__ */