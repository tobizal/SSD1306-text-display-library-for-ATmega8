#include <avr/io.h>
#include "display.h"
#include "i2c.h"
#include "font.h"

int v_pos = 0xB0;
int h_pos = 0x00;
int v_scroll = 0x00;

void init_display();

void clear_screen();
void clear_row();

void reset_cursor();
void reset_h_pos();
void reset_v_pos();
void reset_scroll();

void newline();
void scroll_down();

void printc(char c);
void prints(char* s);

void init_display() {
    clear_screen();
    i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_CTRL); // continous control
	i2c_send_byte(CHARGE_PUMP_SETTING);
	i2c_send_byte(ENABLE_CHARGE_PUMP);
	i2c_send_byte(DISPLAY_ON_NORMAL_MODE);
	// i2c_send_byte(DISPLAY_FULL_ON);
	i2c_stop();
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
    reset_cursor();
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

void printc(char c) {
	if (h_pos > 120) newline();
	h_pos += 8;
	int idx = 0;
	if (c >= 'A' && c <= 'Z') idx += (c - 'A' + 10);
	else if (c >= '0' && c <= '9') idx += (c - '0');
	else if (c == '\n') {
		newline();
		return;
	} else if (c == '\f') {
        clear_screen();
        return;
    }
	i2c_start();
	i2c_send_address(DISPLAY_ADDRESS, WRITE);
	i2c_send_byte(CO_DATA);
	for (int i = 0; i < 8; i++) i2c_send_byte(font[idx][i]);
	i2c_stop();
}

void prints(char* c) {
    while (*c != '\0') printc(*(c++));
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
