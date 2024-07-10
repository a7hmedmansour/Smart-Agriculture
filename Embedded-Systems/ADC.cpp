#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ADC.h"
#include <avr/io.h>
void ADC_INIT()
{   
 // Set voltage reference to AVcc
    ADMUX |= (1 << REFS0);
    
    // Enable ADC, set prescaler to 128 for 8MHz clock
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
u16 ADC_READ(u8 channel)
{
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Return ADC result
    return ADC;
	
}