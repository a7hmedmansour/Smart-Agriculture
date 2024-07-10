#ifndef SENSORS_H_
#define SENSORS_H_
#include "STD_TYPES.h"
#include "DIO_INTERFACE.h"
#include "I2C.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL

                /*Sensors' Ports*/
#define  FlameSensor_Port          DIO_PORTD
#define  RainSensor_Port           DIO_PORTB
#define  WaterSensor_Port          DIO_PORTC
#define  Soil_MoistureSensor_Port  DIO_PORTC
#define  Thermometer_Port          DIO_PORTC
               /*Sensors' PINS*/
#define Flame_PIN                  DIO_PIN2
#define Rain_PIN                   DIO_PIN3
#define Water_PIN                  DIO_PIN2
#define Thermometer_CLOCKPIN       DIO_PIN5
#define Thermometer_DATAPIN        DIO_PIN4
#define Soil_Moisture_PIN          DIO_PIN3

#define AM2320_ADDRESS 0xB8
#define WRITE          0
#define Read           1
#define BUZZER_PORT PORTB
#define BUZZER_DDR DDRB
#define BUZZER_PIN  2
/*_________________END______________________*/

/*________________LED_MACROS_______________*/
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_PIN  7
/*_________________END____________________*/

/*________________MOTOR_MACROS_______________*/
#define MOTOR_DDR DDRC
#define MOTOR_PORT PORTC
#define POS_PIN DIO_PIN0
#define NEG_PIN DIO_PIN1
/*____________________END____________________*/

void BUZZER_alarm();
void LED_alarm();
void FIRE_ON();
void FIRE_OFF();
void FIRE_MOTOR_ON();
void FIRE_MOTOR_OFF();
void LED_alarm_off();
void BUZZER_alarm_off();
void FIRE_SYS();
void Sensors_Init();
void GET_Tempreature(u16* HUM, u16* TEMP);
u8 Check_Fire();
u8 Check_Rain();
u8 Check_Water();
u32 GET_Soil_Moisture();
// Function to initialize external interrupt INT0
void INT0_init();
#endif /* SENSORS_H_ */