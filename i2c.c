#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "i2c.h"

// private functions
void error(int err_code){
    DDRB |= 1 << PB0;

    // err_code number of quick flashes
    for(int i=0; i<err_code; i++){
        PORTB |= 1 << PB0;
        _delay_ms(100);
        PORTB &= ~(1 << PB0);
        _delay_ms(100);
    }

    _delay_ms(1000);
}

void wait_twint_set(){
    while (!(TWCR & (1<<TWINT))){}
}

void set_internal_pullups(){
    PORTC |= (1<<PC5) | (1<<PC4);
}

void check_twi_status_start(){
    if((TWSR & 0xF8) != TW_START)
        error(1);
}

void check_twi_status_data_ack(){
    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        error(4);
}

void check_twi_status_sla_ack(){
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        error(2);
}



// API
void i2c_start() {
	set_internal_pullups();
	TWCR = (1<<TWINT | (1<<TWSTA) | (1<<TWEN)); // initiate START
	wait_twint_set();
	check_twi_status_start();
}

void i2c_send_address(int address, I2C_MODE mode) {
	TWDR  = address | mode ;                    // load slave address into TWDR Register
	TWCR = (1<<TWINT) | (1<<TWEN); 	 			// transmit address
	wait_twint_set();
	check_twi_status_sla_ack();
}

void i2c_send_byte(int data) {
	TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
	wait_twint_set();
	check_twi_status_data_ack();
}
void i2c_stop() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}




