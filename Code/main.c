#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
/*
const uint8_t digit_pattern[10] = {
	0xDD, //0b11011101, //0
	0x50, //0b01010000, //1
	0xCB, //0b11001011, //2
	0x5B, //0b01011011, //3
	0x56, //0b01010110, //4
	0x1F, //0b00011111, //5
	0x9F, //0b10011111, //6
	0x58, //0b01011000, //7
	0xDF, //0b11011111, //8
	0x5E, //0b01011110, //9
}
/

void setDigits(){

	while (1){
	
		PORTD |= (1 << PORTD7);
		PORTD |= (1 << PORTD6);
		PORTD |= (1 << PORTD5);

		_delay_ms(1000);
		PORTD &= ~(1 << PORTD7);
		PORTD &= ~(1 << PORTD6);
		PORTD &= ~(1 << PORTD5);
		_delay_ms(500);
		}
}

int main(void){
	//set pins for serial , clock and latch as outputs
	//DDRD = 11100000
	DDRD |= (1 << DDD7);
	DDRD |= (1 << DDD6);
	DDRD |= (1 << DDD5);
	unsigned char zero = 0b11011101;	
	while(1){
		for (int i = 0; i < 8; i++){
			unsigned char bit = (zero >> i) & 1;
			(bit) ? (PORTD |= (1 << PORTD7)) : (PORTD &= ~(1 << PORTD7));
			_delay_ms(1000);
		}
		
		PORTD &= ~(1 << PORTD7);
		_delay_ms(5000);
	}
	
	return 0;
}
*/

#define LATCH_PIN 8
#define CLOCK_PIN 12
#define DATA_PIN 2
#define LSBFIRST 0
#define MSBFIRST 1
#define HIGH 1
#define LOW 0
/*
void digitalWrite(uint8_t pin, uint8_t value) {
    if (value == HIGH) {
        PORTD |= (1 << pin); // Set pin HIGH
    } else 
    	    PORTD &= ~(1 << pin); // Set pin LOW
    }

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    digitalWrite(clockPin, LOW);
    for (i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
           *PORTD |= (1 << dataPin); // Set data pin high or low based on LSB
            PORTD &= ~(1 << clockPin); // Clock pin low
            
		digitalWrite(dataPin, val & 1);
	    	val >>= 1; // Shift bits to the right
			   //
        } else {
		
            PORTD |= (1 << dataPin); // Set data pin high or low based on MSB
            PORTD &= ~(1 << clockPin); // Clock pin low
            
		digitalWrite(dataPin,(val & 128) !=0);
	    	val <<= 1; // Shift bits to the left
        }

 	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);
    }
}
*/
void setup(){
	    // Set pins to output
    DDRB &= (0);
    DDRD &= (0);

    DDRB |= (1 << DDB0); // Latch pin
    DDRB |= (1 << DDB4); // Clock pin
    DDRD |= (1 << DDD2); // Data pin
    
}
int main (void){
    	setup();
        while(1){
	// Ground latchPin and hold low for as long as you are transmitting
		PORTB &= ~(1 << LATCH_PIN);
        // Shift out the desired value (LSB first)
        //shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 221);
	       	uint8_t num = 1;
		uint8_t val = num;	
			
		for (uint8_t i = 0; i < 8; i++){
			if ( (val&1) == HIGH) {
				PORTD |= (1 << DATA_PIN);
			
			}else {
				PORTD &= ~(1 << DATA_PIN);			
			}
			val >>=1;

			PORTB |= (1 << CLOCK_PIN);
			PORTB &= ~(1 << CLOCK_PIN);
		}
	// Return the latch pin high to signal chip that it no longer needs to listen for information
       		PORTB |= (1 << LATCH_PIN);
		_delay_ms(1000);
	}
	return 0;
}

