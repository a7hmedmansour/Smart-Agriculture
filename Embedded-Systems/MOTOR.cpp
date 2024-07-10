#include "MOTOR.h"
#include <avr/io.h>
#include <util/delay.h>

static u8 test=0;
static u8 test2=0;
static u8 dir=0;
void MOTOR_DC_ON(u8 copy_dir)
{
	SET_BIT(MOTOR_DC_DDR, NEG_DC_PIN);
	SET_BIT(MOTOR_DC_DDR, POS_DC_PIN);
	dir=copy_dir;
	if (dir==1)
	{
	SET_BIT(MOTOR_DC_PORT,NEG_DC_PIN);
	CLEAR_BIT(MOTOR_DC_PORT,POS_DC_PIN);}
	else if (dir==0)
	{
		SET_BIT(MOTOR_DC_PORT,POS_DC_PIN);
		CLEAR_BIT(MOTOR_DC_PORT,NEG_DC_PIN);
	}
}


void MOTOR_DC_OFF()
{
		CLEAR_BIT(MOTOR_DC_PORT,NEG_DC_PIN);
		CLEAR_BIT(MOTOR_DC_PORT,POS_DC_PIN);

}
void MOTOR_ON(u8 copy_dir)
{
	 

    // Initialize PWM
   SET_BIT(MOTOR_DDR, NEG_PIN);
	SET_BIT(MOTOR_DDR, POS_PIN);
	dir=copy_dir;
	if (dir==1)
	{
	SET_BIT(MOTOR_PORT,NEG_PIN);
	CLEAR_BIT(MOTOR_PORT,POS_PIN);}
	else if (dir==0)
	{
		SET_BIT(MOTOR_PORT,POS_PIN);
		CLEAR_BIT(MOTOR_PORT,NEG_PIN);
	}
}



void MOTOR_OFF()
{
		CLEAR_BIT(MOTOR_PORT,NEG_PIN);
		CLEAR_BIT(MOTOR_PORT,POS_PIN);

}


