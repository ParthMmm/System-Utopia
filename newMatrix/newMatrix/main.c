//ledmatrix7219d88 output example

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "max7219.c"
#include "ledmatrix7219d88.c"
#include "max7219.h"
#include "ledmatrix7219d88.h"
#include "max7219led8x8.c"
#include "timer.h"
#include "io.c"
unsigned char LFARR[8]={0x00,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00}; //LED Matrix pattern for left arrow
unsigned char RTARR[8]={0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x00}; //LED Matrix pattern for right arrow
unsigned char DNARR[8]={0x00,0x10,0x30,0x7E,0x7E,0x30,0x10,0x00}; //LED Matrix pattern for down arrow
unsigned char UPARR[8]={0x00,0x08,0x0C,0x7E,0x7E,0x0C,0x08,0x00}; //LED Matrix pattern for up arrow
unsigned char button1, button2,  button3;
int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    //max7219_init();
    ledmatrix7219d88_init();
    uint8_t rows[8] = {
        0b10000001,
        0b01000010,
        0b00100100,
        0b00011000,
        0b00011000,
        0b00100100,
        0b01000010,
        0b10000001
    };
    
    TimerSet(100);
    TimerOn();
    
    unsigned char LFARR[8]={0x00,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00}; //LED Matrix pattern for left arrow
    unsigned char blank[8]={0,0,0,0,0,0,0,0};
        
        
        
    
    ledmatrix7219d88_setmatrix(0, rows);
    while(1){
        //button1 = ~PINA & 0x01;
        //button2 = ~PINA & 0x02;
        //button3 = ~PINA & 0x04;
        //
        //if(button1){
           //ledmatrix7219d88_setmatrix(0, LFARR); 
        //}
        //else if(button2){
           //ledmatrix7219d88_setmatrix(0, UPARR);  
        //}
        //else if(button3){
            //ledmatrix7219d88_setmatrix(0, DNARR);  
        //}
        for(unsigned char k = 0; k < 8;k++) {
            blank[k] <<= 1;
            
        }
        
        //for(unsigned char i = 0; i < 8; i++){
           //blank[i] |= 1;
           //delay_ms(50);
           //
        //}
        blank[1] |= 1;
        blank[2] |= 1;
        delay_ms(50);
        ledmatrix7219d88_setmatrix(0, blank); 
        
        
        
        
        //ledmat1rix7219d88_setledon(1, 1);
         while(!TimerFlag);
         TimerFlag = 0;
         
         
        
    }
}
//
//int main(void) {
    //DDRB = 0xFF; PORTB = 0x00;
    //// ---- Initialization ----
    //MAX7219_init();
    //
    //// ---- Main Loop ----
    //while (1) {
        //for (uint8_t y = 0; y <= 7; y++) {
            //for (uint8_t x = 0; x <= 7; x++) {
                //MAX7219_buffer_out();   // Output the buffer
                //MAX7219_buffer_set(x, y);   // Set pixel
                //_delay_ms(1000);
            //}
        //}
        ////for (uint8_t y = 0; y <= 7; y++) {
            ////for (uint8_t x = 0; x <= 7; x++) {
                ////MAX7219_buffer_out();   // Output the buffer
                ////MAX7219_buffer_clr(x, y);   // Clear pixel
                ////_delay_ms(10);
            ////}
        ////}
        //
        //for (uint8_t y = 0; y <= 7; y++) {
            //for (uint8_t x = 0; x <= 7; x++) {
                //MAX7219_buffer_out();   // Output the buffer
                //MAX7219_buffer_set(x, y);   // Set pixel
                //_delay_ms(10);
            //}
        //}
        //
        ////MAX7219_buffer_set();
        ////MAX7219_buffer_out();
    //}
    //
    //return 0;
//}