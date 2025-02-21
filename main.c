#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"
#include "display.h"
#include "font.h"

void write_screen();
void clear_screen();
void reset_cursor();
void putchar(char c);
void reset_h_pos();
void reset_v_pos();
void clear_row();
void newline();
void scroll_down();
void reset_scroll();

int v_pos = 0xB0;
int h_pos = 0x00;
int v_scroll = 0x00;

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
	reset_cursor();
	//write_screen();
	putchar('A');
	newline();
	putchar('B');
	putchar('C');
	putchar('D');
	putchar('E');
	putchar('F');
	putchar('G');
	putchar('H');
	putchar('I');
	putchar('J');
	putchar('K');
	putchar('L');
	putchar('M');
	putchar('N');
	putchar('O');
	putchar('P');
	putchar('R');
	putchar('S');
	putchar('T');
	putchar('U');
	putchar('W');
	putchar('V');
	putchar('X');
	putchar('Y');
	putchar('Z');
	newline();
	for (unsigned int i = 0; i < 255; i++) {
		putchar('0' + (i % 10));
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

void write_screen() {
	for (uint8_t i = 0; i < 15; i++) {
		i2c_start();
		i2c_send_address(DISPLAY_ADDRESS, WRITE);
		i2c_send_byte(CO_DATA);
		//for (uint16_t i = 0; i < 300; i++) {
		//	if (i % 8 < 4) i2c_send_byte(0x0F);
		//	else i2c_send_byte(0xF0);
		//}
		//for (int j = 0; j < 8; j++) {
		//	i2c_send_byte(font[i][j]);
		//}
		i2c_stop();
	}
}

void clear_screen() {
	reset_cursor();
    for (uint8_t i = 0; i < 8; i++) {
        i2c_start();
        i2c_send_address(DISPLAY_ADDRESS, WRITE);
        i2c_send_byte(SINGLE_CTRL);
        i2c_send_byte(v_pos++);
        i2c_send_byte(CO_DATA);
        for (uint8_t i = 0; i < 128; i++) i2c_send_byte(0x00);
        i2c_stop();
    }   
}

void reset_cursor() {
	reset_v_pos();
	reset_h_pos();
	reset_scroll();
}

void reset_scroll() {
	v_scroll = 0x00;
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL);
	i2c_send_byte(0xD3);  // COM vertical shift
	i2c_send_byte(v_scroll);
	i2c_stop();
}

void scroll_down() {
	v_scroll = (v_scroll + 8) % 64;
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL);
	i2c_send_byte(0xD3);  // COM vertical shift
	i2c_send_byte(v_scroll);
	i2c_stop();
}

void reset_v_pos() {
	v_pos = 0xB0;
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL);
	i2c_send_byte(v_pos); // Page addressing mode
	i2c_stop();
}

void reset_h_pos() {
	h_pos = 0x00;
   	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL);
	i2c_send_byte(0x00);
	i2c_send_byte(0x10);
	i2c_stop();
}

void putchar(char c) {
	if (h_pos > 120) newline();
	h_pos += 8;
	int idx = 0;
	if (c >= 'A' && c <= 'Z') idx += (c - 'A' + 10);
	else if (c >= '0' && c <= '9') idx += (c - '0');
	else if (c == '\n') {
		newline();
		return;
	}
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_DATA);
	for (int i = 0; i < 8; i++) i2c_send_byte(font[idx][i]);
	i2c_stop();
}

void newline() {
	reset_h_pos();
	if (v_pos >= 0xB7 - v_scroll ) {  // current page is the last one
		// scroll down 
		scroll_down();
		// wrap v_pos
		if (v_pos >= 0xB7) v_pos = 0xB0 - 1;
	}
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL);
	i2c_send_byte(++v_pos);
	i2c_stop();
	reset_h_pos();
	clear_row();
}

void clear_row() {
	reset_h_pos();
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_DATA);
	for (int i = 0; i < 127; i++) i2c_send_byte(0x00);
	i2c_stop();
	reset_h_pos();
}
