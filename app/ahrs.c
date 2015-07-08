#include <Mymath.h>
#include "stm32f10x.h"

#define Kp 0.4f //5.0f   
#define Ki 0.0001f 

float halfT = 0.0015;
static float halfT_pre = 0.0015;
extern unsigned char TIM2_UPDATED;
void timeUpdate()
{
	unsigned short cnt;
	TIM_Cmd(TIM2, DISABLE);
	cnt = TIM2->CNT;
	if(TIM2_UPDATED == 0)
		halfT = cnt/2000000.0;
	else{
		halfT = (cnt+65536.0)/2000000.0;
		TIM2_UPDATED = 0;
	}
	
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE);
	
	if(my_abs(halfT-halfT_pre)*2000>10)
		halfT = halfT_pre;
	else
		halfT_pre = halfT;
}
  
float exInt = 0, eyInt = 0, ezInt = 0; 

void AHRSupdate(float gx, float gy, float gz, 
				float ax, float ay, float az, 
				float mx, float my, float mz,
				float q[4]) {
        float norm;
        float hx, hy, hz, bx, bz;
        float vx, vy, vz, wx, wy, wz;
        float ex, ey, ez;
					
		float q0 = q[0];
		float q1 = q[1];
		float q2 = q[2];
		float q3 = q[3];

        // auxiliary variables to reduce number of repeated operations
        float q0q0 = q0*q0;
        float q0q1 = q0*q1;
        float q0q2 = q0*q2;
        float q0q3 = q0*q3;
        float q1q1 = q1*q1;
        float q1q2 = q1*q2;
        float q1q3 = q1*q3;
        float q2q2 = q2*q2;   
        float q2q3 = q2*q3;
        float q3q3 = q3*q3;          
        
        // normalise the measurements
        norm = sqrt(ax*ax + ay*ay + az*az);       
        ax = ax / norm;
        ay = ay / norm;
        az = az / norm;
        norm = sqrt(mx*mx + my*my + mz*mz);          
        mx = mx / norm;
        my = my / norm;
        mz = mz / norm;         
        
        // compute reference direction of flux
        hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
        hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
        hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);         
        bx = sqrt((hx*hx) + (hy*hy));
        bz = hz;        
        
        // estimated direction of gravity and flux (v and w)
        vx = 2*(q1q3 - q0q2);
        vy = 2*(q0q1 + q2q3);
        vz = q0q0 - q1q1 - q2q2 + q3q3;
        wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
        wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
        wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
        
        // error is sum of cross product between reference direction of fields and direction measured by sensors
        ex = (ay*vz - az*vy) + (my*wz - mz*wy);
        ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
        ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
        
        // integral error scaled integral gain
        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;
        
        // adjusted gyroscope measurements
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
        
        // integrate quaternion rate and normalise
        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
        
        // normalise quaternion
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q[0] = q0 / norm;
        q[1] = q1 / norm;
        q[2] = q2 / norm;
        q[3] = q3 / norm;
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float q[4])
{
	float norm;
//  float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;
	float q0 = q[0];
	float q1 = q[1];
	float q2 = q[2];
	float q3 = q[3];

	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
	float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
    norm = sqrt(ax*ax + ay*ay + az*az);
    ax = ax /norm;
    ay = ay / norm;
    az = az / norm;

  // estimated direction of gravity and flux (v and w)  
    vx = 2*(q1q3 - q0q2);	
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) ;                           		
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    exInt = exInt + ex * Ki;						
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
    gx = gx + Kp*ex + exInt;					   				
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;				   					

  // integrate quaternion rate and normalise				
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q[0] = q0 / norm;
    q[1] = q1 / norm;
    q[2] = q2 / norm;
    q[3] = q3 / norm;

  //Q_ANGLE.Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
  //Q_ANGLE.Y  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  //Q_ANGLE.X = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}
