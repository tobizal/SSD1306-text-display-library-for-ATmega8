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
	init_display();
	while (1) {
		prints("ABCDEFGHI\nJKLMNO\nPRSTUWVXYZ");
		for (unsigned int i = 0; i < 155; i++) {
			printc('0' + (i % 10));
			_delay_ms(20);
		}
		printc('\f');
 	}
	return 0;
}