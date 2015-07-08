#ifndef __RC_H__
#define __RC_H__

void ppm_isr(void);
void calc_rc_mid(void);
void get_rc(unsigned short rc[6]);
int isReady(void);
void getTarAngle(float tarAngle[3]);
void getTarRate(float tarRate[3]);
float getTarAltitude(void);
void doRcData(void);

#endif
