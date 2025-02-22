#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"
#include "display.h"
#include "font.h"


int main(){
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL); // continous control
	i2c_send_byte(CHARGE_PUMP_SETTING);
	i2c_send_byte(ENABLE_CHARGE_PUMP);
	i2c_send_byte(DISPLAY_ON_NORMAL_MODE);
	// i2c_send_byte(DISPLAY_FULL_ON);
	i2c_send_byte(0xB3); // horizontal offset
	i2c_stop();
	
	clear_screen();
	//write_screen();
	printc('A');
	newline();
	printc('B');
	printc('C');
	printc('D');
	printc('E');
	printc('F');
	printc('G');
	printc('H');
	printc('I');
	printc('J');
	printc('K');
	printc('L');
	printc('M');
	printc('N');
	printc('O');
	printc('P');
	printc('R');
	printc('S');
	printc('T');
	printc('U');
	printc('W');
	printc('V');
	printc('X');
	printc('Y');
	printc('Z');
	newline();
	for (unsigned int i = 0; i < 255; i++) {
		printc('0' + (i % 10));
		_delay_ms(50);
	}

	error(10);

	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL); 
	
	// scrolling
	/*int h_scroll = 0x00;
	while (1) {
		i2c_send_byte(0xD3);
		i2c_send_byte(h_scroll++ % 64);
		_delay_ms(50);
	}*/
	i2c_stop();
	return 0;
}