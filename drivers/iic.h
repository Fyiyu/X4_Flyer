void i2c_delay(void) ;
void i2c_start(void) ;
void i2c_stop(void) ;
void i2c_send_ack(unsigned char ack) ;
unsigned char i2c_recv_ack(void) ;
void i2c_send_byte(unsigned char dat) ;
unsigned char i2c_recv_byte(void) ;

unsigned char i2c_read(unsigned char dev,
               unsigned char reg,
               unsigned char length,
               unsigned char * data);
unsigned char i2c_write(unsigned char dev,
                        unsigned char reg,
                        unsigned char len,
                        unsigned char * dat);
												
void ByteWrite24c512(unsigned char faddr,
										 unsigned char saddr,
										 unsigned char dat);
void PageWrite24c512(unsigned char faddr,
										 unsigned char saddr, 
										 unsigned char* dat, 
										 unsigned char num);										 
unsigned char RandomRead24c512(unsigned char faddr,
											unsigned char saddr);
unsigned char CurrentRead24c512(void);
void SequentialRead24c512(unsigned char faddr, 
													unsigned char saddr, 
													unsigned char *dat, 
													unsigned char num);
