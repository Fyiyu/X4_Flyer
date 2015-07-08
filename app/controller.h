#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


void nav(float point1[2],float point2[2],float speed[2],float output[3]);
void speed_loop(float tarSpeed[3],float realSpeed[3],float output[3]);
void angle_loop(float tarAngle[3],float realAngle[3],float realRate[3],float output[3]);
void angle2rate(float angle[3],float rate[3]);
void rate_loop(float tarRate[3],float realRate[3],float output[3]);
float altitude_loop(float tarAltitude,float realAltitude,float speed,int speed_flag);

#endif
