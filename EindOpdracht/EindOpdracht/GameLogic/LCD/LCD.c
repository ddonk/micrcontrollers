/*
 * LCD.c
 *
 * Created: 9-2-2022 12:49:44
 *  Author: Larsl
 */ 
#include "LCD.h"

void switch_E() {
	PORTC |= (1<<E);
	_delay_ms(1);
	PORTC &= ~(1<<E);
	_delay_ms(1);
}

void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<RS);
	switch_E();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<RS);
	switch_E();
}

void display_text(char *str) {
	while (*str) {
		lcd_write_data(*str++);
	}
}

void lcd_write_command(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<RS);
	switch_E();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<RS);
	switch_E();
}

void display_set_cursor(int position) {
	//TODO
}

void display_clear(){
	lcd_write_command(0x01);
	_delay_ms(2);
	lcd_write_command(0x80);
}

void display_init() {
	// See table 13 from the HD44780U datasheet.
	DDRC = 0xFF;
	PORTC = 0x00;
	
	// Step 2 Function set
	PORTC = 0x20;
	switch_E();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	switch_E();
	PORTC = 0x80;
	switch_E();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	switch_E();
	PORTC = 0xF0;
	switch_E();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	switch_E();
	PORTC = 0x60;
	switch_E();
	
	// Set cursor to position 0 on first row.
	lcd_write_command(0x02);
}