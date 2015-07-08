#ifndef __AHRS_H__
#define __AHRS_H__

void AHRSupdate(float gx, float gy, float gz, 
				float ax, float ay, float az, 
				float mx, float my, float mz,
				const float q[4]);
void IMUupdate(float gx, float gy, float gz, 
			   float ax, float ay, float az, 
			   const float q[4]);
void timeUpdate(void);

#endif
