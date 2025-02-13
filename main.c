#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"
#include "display.h"

void write_screen();

int main(){
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL); // continous control
	i2c_send_byte(CHARGE_PUMP_SETTING);
	i2c_send_byte(ENABLE_CHARGE_PUMP);
	i2c_send_byte(DISPLAY_ON_NORMAL_MODE);
	// i2c_send_byte(DISPLAY_FULL_ON);
	// i2c_send_byte(0xB2); // horizontal offset
	i2c_stop();

	write_screen();

	error(10);

	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL); 
	int h_scroll = 0x00;
	while (1) {
		i2c_send_byte(0xD3);
		i2c_send_byte(h_scroll++ % 64);
		_delay_ms(50);
	}
	return 0;
}

void write_screen() {
	uint8_t h_offset = 0xB0;
	for (uint8_t i = 0; i < 8; i++) {
		i2c_start();
		i2c_send_address(DISPLAY_ADDRESS, WRITE);
		i2c_send_byte(SINGLE_CTRL);
		i2c_send_byte(h_offset++);
		i2c_send_byte(CO_DATA);
		for (uint8_t i = 0; i < 128; i++) {
			if ((h_offset + i) % 8 < 4) i2c_send_byte(0x0F);
			else i2c_send_byte(0xF0);
		}
		i2c_stop();
	}
}
