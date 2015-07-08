#include "Mymath.h"

float fast_atan_table[257] = 
{
	0.000000e+00, 3.921549e-03, 7.842976e-03, 1.176416e-02,
	1.568499e-02, 1.960533e-02, 2.352507e-02, 2.744409e-02,
	3.136226e-02, 3.527947e-02, 3.919560e-02, 4.311053e-02,
	4.702413e-02, 5.093629e-02, 5.484690e-02, 5.875582e-02,
	6.266295e-02, 6.656816e-02, 7.047134e-02, 7.437238e-02,
	7.827114e-02, 8.216752e-02, 8.606141e-02, 8.995267e-02,
	9.384121e-02, 9.772691e-02, 1.016096e-01, 1.054893e-01,
	1.093658e-01, 1.132390e-01, 1.171087e-01, 1.209750e-01,
	1.248376e-01, 1.286965e-01, 1.325515e-01, 1.364026e-01,
	1.402496e-01, 1.440924e-01, 1.479310e-01, 1.517652e-01,
	1.555948e-01, 1.594199e-01, 1.632403e-01, 1.670559e-01,
	1.708665e-01, 1.746722e-01, 1.784728e-01, 1.822681e-01,
	1.860582e-01, 1.898428e-01, 1.936220e-01, 1.973956e-01,
	2.011634e-01, 2.049255e-01, 2.086818e-01, 2.124320e-01,
	2.161762e-01, 2.199143e-01, 2.236461e-01, 2.273716e-01,
	2.310907e-01, 2.348033e-01, 2.385093e-01, 2.422086e-01,
	2.459012e-01, 2.495869e-01, 2.532658e-01, 2.569376e-01,
	2.606024e-01, 2.642600e-01, 2.679104e-01, 2.715535e-01,
	2.751892e-01, 2.788175e-01, 2.824383e-01, 2.860514e-01,
	2.896569e-01, 2.932547e-01, 2.968447e-01, 3.004268e-01,
	3.040009e-01, 3.075671e-01, 3.111252e-01, 3.146752e-01,
	3.182170e-01, 3.217506e-01, 3.252758e-01, 3.287927e-01,
	3.323012e-01, 3.358012e-01, 3.392926e-01, 3.427755e-01,
	3.462497e-01, 3.497153e-01, 3.531721e-01, 3.566201e-01,
	3.600593e-01, 3.634896e-01, 3.669110e-01, 3.703234e-01,
	3.737268e-01, 3.771211e-01, 3.805064e-01, 3.838825e-01,
	3.872494e-01, 3.906070e-01, 3.939555e-01, 3.972946e-01,
	4.006244e-01, 4.039448e-01, 4.072558e-01, 4.105574e-01,
	4.138496e-01, 4.171322e-01, 4.204054e-01, 4.236689e-01,
	4.269229e-01, 4.301673e-01, 4.334021e-01, 4.366272e-01,
	4.398426e-01, 4.430483e-01, 4.462443e-01, 4.494306e-01,
	4.526070e-01, 4.557738e-01, 4.589307e-01, 4.620778e-01,
	4.652150e-01, 4.683424e-01, 4.714600e-01, 4.745676e-01,
	4.776654e-01, 4.807532e-01, 4.838312e-01, 4.868992e-01,
	4.899573e-01, 4.930055e-01, 4.960437e-01, 4.990719e-01,
	5.020902e-01, 5.050985e-01, 5.080968e-01, 5.110852e-01,
	5.140636e-01, 5.170320e-01, 5.199904e-01, 5.229388e-01,
	5.258772e-01, 5.288056e-01, 5.317241e-01, 5.346325e-01,
	5.375310e-01, 5.404195e-01, 5.432980e-01, 5.461666e-01,
	5.490251e-01, 5.518738e-01, 5.547124e-01, 5.575411e-01,
	5.603599e-01, 5.631687e-01, 5.659676e-01, 5.687566e-01,
	5.715357e-01, 5.743048e-01, 5.770641e-01, 5.798135e-01,
	5.825531e-01, 5.852828e-01, 5.880026e-01, 5.907126e-01,
	5.934128e-01, 5.961032e-01, 5.987839e-01, 6.014547e-01,
	6.041158e-01, 6.067672e-01, 6.094088e-01, 6.120407e-01,
	6.146630e-01, 6.172755e-01, 6.198784e-01, 6.224717e-01,
	6.250554e-01, 6.276294e-01, 6.301939e-01, 6.327488e-01,
	6.352942e-01, 6.378301e-01, 6.403565e-01, 6.428734e-01,
	6.453808e-01, 6.478788e-01, 6.503674e-01, 6.528466e-01,
	6.553165e-01, 6.577770e-01, 6.602282e-01, 6.626701e-01,
	6.651027e-01, 6.675261e-01, 6.699402e-01, 6.723452e-01,
	6.747409e-01, 6.771276e-01, 6.795051e-01, 6.818735e-01,
	6.842328e-01, 6.865831e-01, 6.889244e-01, 6.912567e-01,
	6.935800e-01, 6.958943e-01, 6.981998e-01, 7.004964e-01,
	7.027841e-01, 7.050630e-01, 7.073330e-01, 7.095943e-01,
	7.118469e-01, 7.140907e-01, 7.163258e-01, 7.185523e-01,
	7.207701e-01, 7.229794e-01, 7.251800e-01, 7.273721e-01,
	7.295557e-01, 7.317307e-01, 7.338974e-01, 7.360555e-01,
	7.382053e-01, 7.403467e-01, 7.424797e-01, 7.446045e-01,
	7.467209e-01, 7.488291e-01, 7.509291e-01, 7.530208e-01,
	7.551044e-01, 7.571798e-01, 7.592472e-01, 7.613064e-01,
	7.633576e-01, 7.654008e-01, 7.674360e-01, 7.694633e-01,
	7.714826e-01, 7.734940e-01, 7.754975e-01, 7.774932e-01,
	7.794811e-01, 7.814612e-01, 7.834335e-01, 7.853983e-01,
	7.853983e-01
};

float my_abs(float f)
{
	if (f >= 0.0)
	{
		return f;
	}

	return -f;
}

REAL Myatan2(REAL y, REAL x) 
{
	REAL x_abs, y_abs, z;
	REAL alpha, angle, base_angle;
	int index;

	/* don't divide by zero! */
	if ((y == 0.0) && (x == 0.0))
		angle = 0.0;
	else 
	{
		/* normalize to +/- 45 degree range */
		y_abs = my_abs(y);
		x_abs = my_abs(x);
		//z = (y_abs < x_abs ? y_abs / x_abs : x_abs / y_abs);
		if (y_abs < x_abs)
			z = y_abs / x_abs;
		else
			z = x_abs / y_abs;
		/* when ratio approaches the table resolution, the angle is */
		/*      best approximated with the argument itself...       */
		if (z < TAN_MAP_RES)
			base_angle = z;
		else 
		{
			/* find index and interpolation value */
			alpha = z * (REAL) TAN_MAP_SIZE - .5;
			index = (int) alpha;
			alpha -= (REAL) index;
			/* determine base angle based on quadrant and */
			/* add or subtract table value from base angle based on quadrant */
			base_angle = fast_atan_table[index];
			base_angle += (fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
		}

		if (x_abs > y_abs) 
		{        /* -45 -> 45 or 135 -> 225 */
			if (x >= 0.0) 
			{           /* -45 -> 45 */
				if (y >= 0.0)
					angle = base_angle;   /* 0 -> 45, angle OK */
				else
					angle = -base_angle;  /* -45 -> 0, angle = -angle */
			} 
			else
			{                  /* 135 -> 180 or 180 -> -135 */
				angle = 3.14159265358979323846;

				if (y >= 0.0)
					angle -= base_angle;  /* 135 -> 180, angle = 180 - angle */
				else
					angle = base_angle - angle;   /* 180 -> -135, angle = angle - 180 */
			}
		} 
		else 
		{                    /* 45 -> 135 or -135 -> -45 */
			if (y >= 0.0) 
			{           /* 45 -> 135 */
				angle = 1.57079632679489661923;

				if (x >= 0.0)
					angle -= base_angle;  /* 45 -> 90, angle = 90 - angle */
				else
					angle += base_angle;  /* 90 -> 135, angle = 90 + angle */
			} 
			else
			{                  /* -135 -> -45 */
				angle = -1.57079632679489661923;

				if (x >= 0.0)
					angle += base_angle;  /* -90 -> -45, angle = -90 + angle */
				else
					angle -= base_angle;  /* -135 -> -90, angle = -90 - angle */
			}
		}
	}


#ifdef ZERO_TO_TWOPI
	if (angle < 0)
		return (angle + TWOPI);
	else
		return (angle);
#else
	return (angle);
#endif
}

float Myatan(float x, float y)
{
	return Myatan2(y, x);
}

float Mysqrt(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );

	y = * ( float * ) &i;
	y = y * ( f - ( x * y * y ) );
	y = y * ( f - ( x * y * y ) );
	return number * y;
}

#define ONE_PI   (3.14159265)
#define TWO_PI   (2.0 * 3.14159265)
#define ANGLE_UNIT (TWO_PI/10.0)

double mx_sin(double rad)
{   
	double sine;
	if (rad < 0)
		sine = rad*(1.27323954 + 0.405284735 * rad);
	else
		sine = rad * (1.27323954 - 0.405284735 * rad);
	if (sine < 0)
		sine = sine*(-0.225 * (sine + 1) + 1);
	else
		sine = sine * (0.225 *( sine - 1) + 1);
	return sine;
}

double Mysin(double rad)
{
	char flag = 1;

	if (rad >= ONE_PI)
	{
		rad -= ONE_PI;
		flag = -flag;
	}

	return mx_sin(rad) * flag;
}

float Mycos(double rad)
{
	char flag = 1;
	rad += ONE_PI/2.0;

	if (rad >= ONE_PI)
	{
		flag = -flag;
		rad -= ONE_PI;
	}

	return Mysin(rad)*flag;
}


float xatan(float x)
{
    float x2;
    x2=x*x;
    return x*(6.36918871*x2+1.26599861e1)/(x2*(x2+1.05891113e1)+1.26599865e1);
}

float yatan(float x)
{
    if (x<1.41421356-1.0) return xatan(x);
    if (x>1.41421356+1.0) return 0.5*3.1415926535898 -xatan(1.0/x);
    return 0.25*3.1415926535898 +xatan((x-1.0)/(x+1.0));
}

float Myasin(float x) 
{ 
    unsigned char neg=0;
    float t;

    if ((x<-1.0) || (x>1.0)) return 0;
    if (x<0.0) {x=-x; neg=1;}; 
    t=Mysqrt(1.0-x*x); 
    if (x>7.07106781e-1) t=0.5*3.1415926535898 -yatan(t/x); 
    else t=yatan(x/t); 
    if (neg) return -t; 
    return t; 
}

//CRC-16-CCITT
//http://www.dzjs.net/html/qianrushixitong/2007/0530/2162.html
uint16_t  math_crc16(const void * data,uint16_t len)
{
    const static uint16_t crc_tab[16] =
    {
        0x0000 , 0x1021 , 0x2042 , 0x3063 , 0x4084 , 0x50A5 , 0x60C6 , 0x70E7 ,
        0x8108 , 0x9129 , 0xA14A , 0xB16B , 0xC18C , 0xD1AD , 0xE1CE , 0xF1EF
    };
    uint8_t h_crc;
	uint16_t crc = 0;
    const uint8_t * ptr = (const uint8_t *)data;
    //
    while(len --)
    {
        h_crc = (uint8_t)(crc >> 12);
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) >> 4)];
        //
        h_crc = crc >> 12;
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) & 0x0F)];
        //
        ptr ++;
    }
    //
    return crc;
}

uint8_t  math_xor(const void * data,uint16_t len)
{
	uint8_t crc = 0xff;
    const uint8_t * ptr = (const uint8_t *)data;
    //
    while(len --)
    {
        crc ^= *ptr;
        ptr ++;
    }
    //
    return crc;
}

/*
 *向量×积
*/
void math_vector_cross(float result[3],const float left[3],const float right[3])
{
    result[0] = left[1]*right[2] - left[2]*right[1];
    result[1] = left[2]*right[0] - left[0]*right[2];
    result[2] = left[0]*right[1] - left[1]*right[0];
}

/*
 *向量点积
*/
float math_vector_dot(const float left[3],const float right[3])
{
    return left[0]*right[0] + left[1]*right[1] + left[2]*right[2];
}

/*
 *四元数相乘更新
*/
void quaternion_mult(quaternion * result,const quaternion * left,const quaternion * right)
{
    result->w = left->w * right->w - left->x * right->x - left->y * right->y - left->z * right->z;
    result->x = left->x * right->w + left->w * right->x + left->y * right->z - left->z * right->y;
    result->y = left->y * right->w + left->w * right->y + left->z * right->x - left->x * right->z;
    result->z = left->z * right->w + left->w * right->z + left->x * right->y - left->y * right->x;
}

/*
 *用四元数旋转向量
*/
void quaternion_rotateVector(const quaternion * rotation,const float from[3],float to[3])
{
    float x2  = rotation->x * 2;
    float y2  = rotation->y * 2;
    float z2  = rotation->z * 2;
    float wx2 = rotation->w * x2;
    float wy2 = rotation->w * y2;
    float wz2 = rotation->w * z2;
    float xx2 = rotation->x * x2;
    float yy2 = rotation->y * y2;
    float zz2 = rotation->z * z2;
    float xy2 = rotation->x * y2;
    float yz2 = rotation->y * z2;
    float xz2 = rotation->z * x2;
    //
    to[0] = from[0]*(1 - yy2 - zz2) + from[1]*(xy2 - wz2)     + from[2]*(xz2 + wy2);
    to[1] = from[0]*(xy2 + wz2)     + from[1]*(1 - xx2 - zz2) + from[2]*(yz2 - wx2);
    to[2] = from[0]*(xz2 - wy2)     + from[1]*(yz2 + wx2)     + from[2]*(1 - xx2 - yy2);
}

/*
 *一对向量旋转重合
*/
void quaternion_fromTwoVectorRotation(quaternion * result,const float from[3],const float to[3])
{
	float cos_theta,sin_half_theta,cross_x,cross_y,cross_z,sin_half_theta_div_cross_norm;
	
    float from_norm = fabsf(from[0]*from[0] + from[1]*from[1] + from[2]*from[2]);
    float to_norm = fabsf(to[0]*to[0] + to[1]*to[1] + to[2]*to[2]);
    //
    from_norm = sqrtf(from_norm);
    to_norm = sqrtf(to_norm);
    cos_theta = (from[0]*to[0] + from[1]*to[1] + from[2]*to[2]) / (from_norm*to_norm);
    result->w = sqrtf((1.0f + cos_theta) / 2); // cos(theta/2)
    sin_half_theta = sqrtf((1 - cos_theta) / 2);
    cross_x = from[1]*to[2] - from[2]*to[1];
    cross_y = from[2]*to[0] - from[0]*to[2];
    cross_z = from[0]*to[1] - from[1]*to[0];
    if(cos_theta < 0)
    {
        cross_x = - cross_x;
        cross_y = - cross_y;
        cross_z = - cross_z;
    }
    sin_half_theta_div_cross_norm = sin_half_theta /
        sqrtf(cross_x*cross_x + cross_y*cross_y + cross_z*cross_z);
    result->x = cross_x * sin_half_theta_div_cross_norm;
    result->y = cross_y * sin_half_theta_div_cross_norm;
    result->z = cross_z * sin_half_theta_div_cross_norm;
}

/*
 * 两对向量旋转重合
*/
void quaternion_fromFourVectorRotation(quaternion * result,const float from1[3],
    const float to1[3],const float from2[3],const float to2[3])
{
    
    float mid_from[3],mid_to[3],cross_from[3],cross_to[3];
	int i;
	float cross_from_1[3];
	quaternion rotation_1,rotation_2;
    math_vector_cross(cross_from,from1,from2);
    math_vector_cross(cross_to,to1,to2);
    for(i=0;i<3;i++)
    {
        mid_from[i] = from1[i] + from2[i];
        mid_to[i] = to1[i] + to2[i];
    }
    quaternion_fromTwoVectorRotation(&rotation_1,mid_from,mid_to);

    quaternion_rotateVector(&rotation_1,cross_from,cross_from_1);
    quaternion_fromTwoVectorRotation(&rotation_2,cross_from_1,cross_to);

    quaternion_mult(result,&rotation_2,&rotation_1);
}
