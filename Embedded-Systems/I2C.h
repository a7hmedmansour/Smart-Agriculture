#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_
#include "STD_TYPES.h"

void I2C_init();

void I2C_START();

void I2C_write(u8 data);

void I2C_STOP();
u8 I2C_read();
u8 I2C_read_NACK();
 

#endif