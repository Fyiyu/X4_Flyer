#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <stdint.h>
#include <math.h>

typedef float REAL;

#define REAL   float
#define TAN_MAP_RES     0.003921569     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293
#define TAN_MAP_SIZE    256
#define MY_PPPIII   	3.14159
#define MY_PPPIII_HALF  1.570796

typedef  struct
{
    float w;
    float x;
    float y;
    float z;
}quaternion;

float my_abs(float f);
REAL Myatan(REAL y, REAL x);
REAL Myatan2(REAL y, REAL x);
float Mysqrt(float number);
double mx_sin(double rad);
double Mysin(double rad);
float Mycos(double rad);
float xatan(float x);
float yatan(float x);
float Myasin(float x);
uint16_t  math_crc16(const void * data,uint16_t len);
uint8_t  math_xor(const void * data,uint16_t len);
void math_vector_cross(float result[3],const float left[3],const float right[3]);
float math_vector_dot(const float left[3],const float right[3]);
void quaternion_mult(quaternion * result,const quaternion * left,const quaternion * right);
void quaternion_rotateVector(const quaternion * rotation,const float from[3],float to[3]);
void quaternion_fromTwoVectorRotation(quaternion * result,const float from[3],const float to[3]);
void quaternion_fromFourVectorRotation(quaternion * result,const float from1[3],const float to1[3],
										const float from2[3],const float to2[3]);

#endif
