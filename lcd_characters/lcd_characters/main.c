/*
 * lcd_characters.c
 *
 * Created: 2/25/19 9:06:38 PM
 * Author : parthmangrola
 */ 


#include <avr/io.h>
#include <timer.h>
#include <io.h>
#include "io.c"
#include "shift.c"

unsigned char portrOPN[8] = {15, 14, 12, 12, 12, 12, 14, 15};
// _1111
// _111_
// _11__
// _11__
// _11__
// _11__
// _111_
// _1111
unsigned char portrEQL[8] = { 0,  0,  0, 31,  0, 31,  0,  0};
unsigned char portrEYE[8] = { 0,  0,  4, 10, 21, 10,  4,  0};
unsigned char portrMTL[8] = { 0,  0,  1,  3,  6, 12,  8,  0};
unsigned char portrMTR[8] = { 0,  0, 16, 24, 12,  6,  2,  0};
unsigned char portrCLS[8] = {30, 14,  6,  6,  6,  6, 14, 30};
    
unsigned char Character1[8] = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };  /* Custom char set for alphanumeric LCD Module */
unsigned char Character2[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
unsigned char Character3[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 };
unsigned char Character4[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 };
unsigned char Character5[8] = { 0x01, 0x03, 0x05, 0x09, 0x09, 0x0B, 0x1B, 0x18 };
unsigned char Character6[8] = { 0x0A, 0x0A, 0x1F, 0x11, 0x11, 0x0E, 0x04, 0x04 };
unsigned char Character7[8] = { 0x00, 0x00, 0x0A, 0x00, 0x04, 0x11, 0x0E, 0x00 };
unsigned char Character8[8] = { 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00 };
    
//unsigned char u1[8] = { 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1F}; //u
//unsigned char u2[8] = { 0x1F, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}; //t
//unsigned char u3[8] = { 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}; //o
//unsigned char u4[8] = { 0x1F, 0x0B, 0x0B, 0x0F, 0x08, 0x08, 0x08, 0x1C}; //p
//unsigned char u5[8] = { 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10}; //i
//unsigned char u6[8] = { 0x1F, 0x1F, 0x13, 0x13, 0x1F, 0x13, 0x13, 0x13}; //A
//unsigned char u7[8] = { 0x0F, 0x08, 0x08, 0x08, 0x0F, 0x01, 0x01, 0x1F}; //s
//unsigned char u8[8] = { 0x00, 0x00, 0x00, 0x11, 0x0A, 0x04, 0x08, 0x10}; //y
//unsigned char u9[8] = { 0x1F, 0x19, 0x1A, 0x1E, 0x1E, 0x1A, 0x19, 0x1F}; //e
//unsigned char u10[8] = {0x11, 0x0A, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}; //m
    
    unsigned char RTARR[8]={0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x00}; //LED Matrix pattern for right arrow
int main(void)
{
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    
    LCD_init();
   //LCD_DisplayString(1, "???????????? ????????????");
    //LCD_Cursor(0x00);
    LCD_BuildCharacter(0,u1);
    LCD_BuildCharacter(1,u2);
    LCD_BuildCharacter(2,u3);
    LCD_BuildCharacter(3,u4);
    LCD_BuildCharacter(4,u5);
    LCD_BuildCharacter(5,u6);
    LCD_BuildCharacter(6,u7); //s
    LCD_BuildCharacter(7,u9); //e
    
    
    
    LCD_Cursor(0x05);
    
    
    LCD_WriteData(6);
    LCD_WriteData(121);
    LCD_WriteData(6);
    LCD_WriteData(84);
    LCD_WriteData(7);
    LCD_WriteData(77);
    //LCD_DisplayString(7, "ySTEM");
    LCD_Cursor(0x15);
    //LCD_DisplayString(18, "SySTEM");
    //LCD_BuildCharacter(0,u7);
    //LCD_BuildCharacter(1,u8);
    //LCD_BuildCharacter(2,u7);
    //LCD_BuildCharacter(3,u2);
    //LCD_BuildCharacter(4,u9);
    //LCD_BuildCharacter(5,u10);
    //for(unsigned char i = 0; i<6; i++){
        //LCD_WriteData(i);
    //}
    
    for(unsigned char i = 0; i<6; i++){
        LCD_WriteData(i);
    }
    
    

     
    
    
    //LCD_BuildCharacter(0, Character1);  /* Build Character1 at position 0 */
    //LCD_BuildCharacter(1, Character2);  /* Build Character2 at position 1 */
    //LCD_BuildCharacter(2, Character3);  /* Build Character3 at position 2 */
    //LCD_BuildCharacter(3, Character4);  /* Build Character4 at position 3 */
    //LCD_BuildCharacter(4, Character5);  /* Build Character5 at position 4 */
    //LCD_BuildCharacter(5, Character6);  /* Build Character6 at position 5 */
    //LCD_BuildCharacter(6, Character7);  /* Build Character6 at position 6 */
    //LCD_BuildCharacter(7, Character8);  /* Build Character6 at position 7 */
    
    //LCD_BuildCharacter(0, portrOPN); // stores porter open brace to memory location 0x00
    //LCD_BuildCharacter(1, portrEQL); // stores porter equal sign to memory location 0x01
    //LCD_BuildCharacter(2, portrEYE); // stores porter eyeball to memory location 0x02
    //LCD_BuildCharacter(3, portrMTL); // stores 1st half of porter mouth to location 0x03
    //LCD_BuildCharacter(4, portrMTR); // stores 2nd half of porter mouth to location 0x04
    //LCD_BuildCharacter(5, portrCLS); // stores porter close brace to mem location 0x05
    
    
    while (1) 
    {
        //LCD_DisplayString(1, "Hello World");
    }
}

