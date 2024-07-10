#include "ADC.h"
#include "BIT_MATH.h"
#include "DIO_INTERFACE.h"
#include "STD_TYPES.h"
#include "Sensors_Interface.h"
#include "MOTOR.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
void Sensors_Init()
{
	/* configure sensors' pins*/
	DIO_u8SetPinDir(FlameSensor_Port,Flame_PIN,DIO_PIN_IN);
	DIO_u8SetPinDir(RainSensor_Port,Rain_PIN,DIO_PIN_IN);
	DIO_u8SetPinDir(WaterSensor_Port,Water_PIN,DIO_PIN_IN);
	DIO_u8SetPinDir(Thermometer_Port,Thermometer_CLOCKPIN,DIO_PIN_IN);
	DIO_u8SetPinDir(Thermometer_Port,Thermometer_DATAPIN,DIO_PIN_IN);
	
	DIO_u8SetPinValue(WaterSensor_Port,Water_PIN,1);
		ADC_INIT();
INT0_init();
}

u8 Check_Fire()
{
	u8 state=0;
	DIO_u8GetPinValue(FlameSensor_Port,Flame_PIN,&state);
return state;
	}

u8 Check_Rain()
{
	u8 state=0;
	DIO_u8GetPinValue(RainSensor_Port,Rain_PIN,&state);
return state;
}

u8 Check_Water()
{
	u8 state=0;
	DIO_u8GetPinValue(WaterSensor_Port,Water_PIN,&state);
return state;
}

u32 GET_Soil_Moisture()
{
	s32 Analog_Value=ADC_READ(3);
	s32 Soil_Moisture_Value=(u32)(Analog_Value*5000)/1024;
        Soil_Moisture_Value /=10;
	return Soil_Moisture_Value;
}
void GET_Tempreature(u16*HUM,u16*TEMP)
{
  u8 humHigh,humLow,tempHigh,tempLow;
	I2C_START();                //send START condition/
  I2C_write(0xB8);           //send write address of AM2320/
  I2C_STOP();                 //send STOP condition/

  //write function code to point to data register of sensor/
  I2C_START();               //send START condition/
  I2C_write(0xB8);            //send write address of AM2320/
  I2C_write(0x03);            //send function code: read data reg/
  I2C_write(0x00);            //send start address of data reg/
  I2C_write(0x04);            //send number of registers to read/
  I2C_STOP();                 //send STOP condition/

  _delay_ms(2);                //wait 2ms/

  //read & store humidity & temp bytes from sensor/
  I2C_START();                //send START condition/
  I2C_write(0xB9);            //send read address of AM2320/
  //------------------------------------------------------------------
  I2C_read();                 //read function code (0x30)/
  I2C_read();                 //read number of registers (4)/
  //------------------------------------------------------------------
  I2C_read();                 //read humidity high byte/
  humHigh = TWDR;            //copy humidity high byte from TWDR/
  I2C_read();                 //read humidity low byte/
  humLow = TWDR;              //copy humidity low byte from TWDR/
  //------------------------------------------------------------------
  I2C_read();                //read temp high byte/
  tempHigh = TWDR;           //copy temp high byte from TWDR/
  I2C_read();                 //read temp low byte/
  tempLow = TWDR;             //copy temp low byte from TWDR/
  //------------------------------------------------------------------
  I2C_read();                 //read low byte CRC/
  I2C_read_NACK();            //read high byte CRC & send NACK pulse/
  //------------------------------------------------------------------
  I2C_STOP();                 //send STOP condition/

  //compute 16-bit humidity & temp values//
  *HUM  = ((humHigh << 8) | humLow)/10;   //compute 16-bit humidity/
  *TEMP = ((tempHigh << 8) | tempLow)/10; //compute 16-bit temp/
  
  _delay_ms(2000);
}
ISR(INT0_vect) {
    // Handle the interrupt triggered by the flame sensor
    // For example, you can toggle an LED connected to PB0
 FIRE_SYS();

}

// Function to initialize external interrupt INT0
void INT0_init(void) {
    // Configure PD2 (INT0) as input
    DDRD &= ~(1 << PD2);

    // Enable pull-up resistor on PD2
    PORTD |= (1 << PD2);

    // Enable external interrupt on INT0 with rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // Enable INT0 interrupt
    EIMSK |= (1 << INT0);

    // Enable global interrupts
    sei();
}

void FIRE_SYS()
{
		
	while (1)
	{
		_delay_ms(250);
		if (GET_BIT(PIND,2)==0)
		{
			FIRE_OFF();
			break;
		}
		else
		{
		FIRE_ON();
		}
	}
}

void FIRE_ON()
{	
	
	BUZZER_alarm();
	LED_alarm();
	//FIRE_MOTOR_ON();	
	_delay_ms(250);
}

void FIRE_OFF()
{
	BUZZER_alarm_off();
	//FIRE_MOTOR_OFF();
	LED_alarm_off();
}

void BUZZER_alarm()
{
	 SET_BIT(BUZZER_DDR, BUZZER_PIN);
	TOGGLE_BIT(BUZZER_PORT,BUZZER_PIN);
	
}



void BUZZER_alarm_off()
{

	CLEAR_BIT(BUZZER_PORT,BUZZER_PIN);
	
}

void LED_alarm()
{
	SET_BIT(LED_DDR, LED_PIN);
	TOGGLE_BIT(LED_PORT,LED_PIN);
}


void LED_alarm_off()
{
	
	CLEAR_BIT(LED_PORT,LED_PIN);
}

void FIRE_MOTOR_ON()
{
	SET_BIT(MOTOR_DDR, NEG_PIN);
	SET_BIT(MOTOR_DDR, POS_PIN);
	
	SET_BIT(MOTOR_PORT,POS_PIN);
	CLEAR_BIT(MOTOR_PORT,NEG_PIN);
	
}



void FIRE_MOTOR_OFF()
{
	CLEAR_BIT(MOTOR_PORT,NEG_PIN);
	CLEAR_BIT(MOTOR_PORT,POS_PIN);

}