#ifndef MOTOR_H_
#define MOTOR_H_
#include "DIO_INTERFACE.h"
#include "MOTOR.h"
#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#define MOTOR_DDR DDRC
#define MOTOR_PORT PORTC
#define POS_PIN DIO_PIN0
#define NEG_PIN DIO_PIN1
#define MOTOR_DC_DDR DDRB
#define MOTOR_DC_PORT PORTB
#define POS_DC_PIN DIO_PIN0
#define NEG_DC_PIN DIO_PIN1
void Motor(u8 copy_istankwater,u8 copy_time,u8 copy_time_amount,u8 copy_dir);
void MOTOR_ON(u8 copy_dir);
void MOTOR_OFF();
void MOTOR_DC_ON(u8 copy_dir);
void MOTOR_DC_OFF();
#endif /* MOTOR_H_ */