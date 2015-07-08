#ifndef __SENSOR_H__
#define __SENSOR_H__

void sensor_init(void);
void mpu6050_getOFFSET(void);
void sensor_read(float basic[9], unsigned int loop_count, float bmp085[2]);

#endif
