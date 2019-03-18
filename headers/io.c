#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTD		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTC	// port connected to pins 4 and 6 of LCD disp.
#define RS 0			// pin number of uC connected to pin 4 of LCD disp.
#define E 1			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/
unsigned char u1[8] = { 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1F}; //u
unsigned char u2[8] = { 0x1F, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}; //t
unsigned char u3[8] = { 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}; //o
unsigned char u4[8] = { 0x1F, 0x0B, 0x0B, 0x0F, 0x08, 0x08, 0x08, 0x1C}; //p
unsigned char u5[8] = { 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10}; //i
unsigned char u6[8] = { 0x1F, 0x1F, 0x13, 0x13, 0x1F, 0x13, 0x13, 0x13}; //A
unsigned char u7[8] = { 0x0F, 0x08, 0x08, 0x08, 0x0F, 0x01, 0x01, 0x1F}; //s
unsigned char u8[8] = { 0x00, 0x00, 0x00, 0x11, 0x0A, 0x04, 0x08, 0x10}; //y
unsigned char u9[8] = { 0x1F, 0x19, 0x1A, 0x1E, 0x1E, 0x1A, 0x19, 0x1F}; //e
unsigned char u10[8] = {0x11, 0x0A, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}; //m

void LCD_BuildCharacter(unsigned char loc, unsigned char *msg);
    
void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	
    delay_ms(10);		
	LCD_BuildCharacter(0,u1);
    LCD_BuildCharacter(1,u2);
    LCD_BuildCharacter(2,u3);
    LCD_BuildCharacter(3,u4);
    LCD_BuildCharacter(4,u5);
    LCD_BuildCharacter(5,u6);
    LCD_BuildCharacter(6,u7); //s
    LCD_BuildCharacter(7,u9); //e
						 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}

void LCD_BuildCharacter (unsigned char loc, unsigned char *msg)
{
    unsigned char i;
    if(loc<8)
    {
     LCD_WriteCommand (0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++){  /* Write 8 byte for generation of 1 character */
           LCD_WriteData(msg[i]);
       }                 
    }
    LCD_WriteCommand(0x80);
      
}



