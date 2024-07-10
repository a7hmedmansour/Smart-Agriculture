#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "I2C.h"
#include <avr/io.h>
#include <util/delay.h>

void I2C_init() 
{
    TWSR = 0;               // prescaler = 0
    TWBR = 72;              // division factor = 72 (SCK freq = 100kHz)
    TWCR = (1<<TWEN);       // enable TWI
}

void I2C_START() {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   // transmit START condition
    while (!(TWCR & (1<<TWINT))); 
              // wait for end of transmission
}

void I2C_write(uint8_t data) {
    TWDR = data;                              // receive byte via data
    TWCR = (1<<TWINT)|(1<<TWEN);              // transmit SLA+W
    while (!(TWCR & (1<<TWINT)));  
             // wait for end of transmission
}

void I2C_STOP() {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);  // transmit STOP condition
}

u8 I2C_read() {
  
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);   // enable TWI & ACK
    while (!(TWCR & (1<<TWINT)));   
       
            // wait for data byte to be read
         /*   
     SET_BIT(TWCR,TWEA);
		SET_BIT(TWCR,TWINT);
		while(GET_BIT(TWCR,TWINT)==0);
    */
    return TWDR;                            // store received byte
}



u8 I2C_read_NACK() {
    TWCR = (1<<TWINT)|(1<<TWEN);             // enable TWI & NACK
    while (!(TWCR & (1<<TWINT)));            // wait for data byte to be read
    return TWDR;                             // store received byte
}