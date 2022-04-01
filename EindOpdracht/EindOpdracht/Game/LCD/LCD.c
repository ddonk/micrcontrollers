/*
 * LCD.c
 *
 * Created: 9-2-2022 12:49:44
 *  Author: Larsl
 */ 
#include "LCD.h"

unsigned char fullheart[8] = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};
unsigned char emptyheart[8] = {0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00};


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


// 0x0c for turning cursor of.
void display_set_cursor(int position, int line) {
	if (line == 0){
		lcd_write_command((position * 0x1) + 0x80);
		_delay_ms(2);
	} else {
		lcd_write_command((position * 0x1) + 0xc0);
		_delay_ms(2);
	}
}

void display_clear(){
	lcd_write_command(0x01);
	_delay_ms(2);
	lcd_write_command(0x80);
}

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		lcd_write_command(0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		lcd_write_data(msg[i]);
	}
	display_clear();
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

	// Adding custom chars
	LCD_Custom_Char(0, fullheart);
	LCD_Custom_Char(1, emptyheart);
}