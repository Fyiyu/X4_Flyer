#include "controller.h"
#include "parameter.h"
#include "Mymath.h"


void nav(float point1[2],float point2[2],float speed[2],float output[3])
{
	const param_t *p = getParam();
	
	float d = sqrt((point1[0]-point2[0])*(point1[0]-point2[0])+(point1[1]-point2[1])*(point1[1]-point2[1]));
	if(d > p->nav.R)
	{
		
	}
	else
	{
		
	}
}

static float speed_loop_I[3] = {0};
static float speed_loop_errpre[3] = {0};
void speed_loop(float tarSpeed[3],float realSpeed[3],float output[3])
{
	float err[3];
	float p_o,i_o,d_o;
	int i;
	const param_t *p = getParam();
	
	for(i=0;i<3;i++)
	{
		err[i] = tarSpeed[i] - realSpeed[i];
		p_o = err[i] * p->speed_pid[i].p;
		//
		speed_loop_I[i] += err[i] * p->speed_pid[i].i;
		if(speed_loop_I[i] > p->speed_pid[i].imax)
			speed_loop_I[i] = p->speed_pid[i].imax;
		else if(speed_loop_I[i] < p->speed_pid[i].imin)
			speed_loop_I[i] = p->speed_pid[i].imin;
		i_o= speed_loop_I[i];
		//
		d_o = (speed_loop_errpre[i] - err[i]) * p->speed_pid[i].d;
		//
		output[i] = p_o + i_o +d_o;
		//
		speed_loop_errpre[i] = err[i];
	}
	
}


static float angle_loop_I[3] = {0};
//static float angle_loop_errpre[3] = {0};
void angle_loop(float tarAngle[3],float realAngle[3],float realRate[3],float output[3])
{
	float err[3];
	float p_o,i_o,d_o;
	int i;
	
	const param_t *p = getParam();

	for(i=0;i<2;i++)
	{
		err[i] = tarAngle[i] - realAngle[i];
		//p_o = Mysin(realAngle[i]*0.0174)/0.0174*9.8;
		p_o = err[i] * p->angle_pid[i].p ;
		//
		if(err[i] > p->angle_pid[i].imin)
		{
			angle_loop_I[i] += 0;
		}
		else
		{
			angle_loop_I[i] += err[i] * p->angle_pid[i].i;
		}
		if(angle_loop_I[i] > p->angle_pid[i].imax)
			angle_loop_I[i] = p->angle_pid[i].imax;
		else if(angle_loop_I[i] < -1*p->angle_pid[i].imax)
			angle_loop_I[i] = -1*p->angle_pid[i].imax;
		i_o= angle_loop_I[i];
		//
		d_o = realRate[i] * p->angle_pid[i].d;
		//
		output[i] = p_o + i_o +d_o;
		//
		//angle_loop_errpre[i] = err[i];
		//Æ«º½¿ØÖÆ
		err[2] = tarAngle[2] - realRate[2];
		p_o = err[2]*p->angle_pid[2].p;/*
		if(err[2] > p->angle_pid[2].imin)
		{
			angle_loop_I[2] += 0;
		}
		else
		{
			angle_loop_I[2] += err[2] * p->angle_pid[2].i;
		}
		if(angle_loop_I[2] > p->angle_pid[2].imax)
			angle_loop_I[2] = p->angle_pid[2].imax;
		else if(angle_loop_I[2] < -1*p->angle_pid[2].imax)
			angle_loop_I[2] = -1*p->angle_pid[2].imax;*/
		i_o= 0;//angle_loop_I[2];
		//
		d_o = 0;//realRate[2] * p->angle_pid[2].d;
		//
		output[2] = p_o + i_o +d_o;
	}
}


void angle2rate(float angle[3],float rate[3])
{}


static float rate_loop_I[3] = {0,0,0};
static float rate_loop_errpre[3] = {0,0,0};
void rate_loop(float tarRate[3],float realRate[3],float output[3])
{
	float err[3];
	float p_o,i_o,d_o;
	int i;

	const param_t *p = getParam();

	for(i=0;i<3;i++)
	{
		err[i] = tarRate[i] - realRate[i];
		p_o = err[i] * p->rate_pid[0].p;
		//
		rate_loop_I[i] += err[i] * p->rate_pid[0].i;
		if(rate_loop_I[i] > p->rate_pid[0].imax)
			rate_loop_I[i] = p->rate_pid[0].imax;
		else if(rate_loop_I[i] < p->rate_pid[0].imin)
			rate_loop_I[i] = p->rate_pid[0].imin;
		i_o= rate_loop_I[i];
		//
		d_o = (rate_loop_errpre[i] - err[i]) * p->rate_pid[0].d;
		//
		output[i] = p_o + i_o +d_o;
		//
		rate_loop_errpre[i] = err[i];
	}
}


static float altitude_loop_I = 0;
static float altitude_loop_errpre = 0;
float altitude_loop(float tarAltitude,float realAltitude,float speed,int speed_flag)
{
	float err;
	float p_o,i_o,d_o;
	float output;

	const param_t *p = getParam();
	
	err = tarAltitude - realAltitude;
	p_o = err * p->altitude_pid.p;
	//
	altitude_loop_I += err * p->altitude_pid.i;
	if(altitude_loop_I > p->altitude_pid.imax)
		altitude_loop_I = p->altitude_pid.imax;
	else if(altitude_loop_I < p->altitude_pid.imin)
		altitude_loop_I = p->altitude_pid.imin;
	i_o = altitude_loop_I;
	//
	if(speed_flag == 0)
		d_o = (altitude_loop_errpre - err) * p->altitude_pid.d;
	else
		d_o = speed * p->altitude_pid.d;
	//
	output = p_o + i_o +d_o;
	//
	altitude_loop_errpre = err;
	
	return output;
}

