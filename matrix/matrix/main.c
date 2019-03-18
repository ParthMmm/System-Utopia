//#include <avr/io.h>
//#include <util/delay.h>
//#include "timer.h"
//
///***************************************
//
//Configure Connections
//
//****************************************/
//
//#define HC595_PORT   PORTB
//#define HC595_DDR    DDRB
//
//#define HC595_DS_POS PB0      //Data pin (DS) pin location
//
//#define HC595_SH_CP_POS PB2      //Shift Clock (SH_CP) pin location 
//#define HC595_ST_CP_POS PB1     //Store Clock (ST_CP) pin location
//
///***************************************
//Configure Connections ***ENDS***
//****************************************/
//
////Initialize HC595 System
//
//void HC595Init()
//{
   ////Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   //HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));
//}
//
//
////Low level macros to change data (DS)lines
//#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))
//
//#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))
//
////Sends a clock pulse on SH_CP line
//void HC595Pulse()
//{
   ////Pulse the Shift Clock
//
   //HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH
//
   //HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW
//
//}
//
////Sends a clock pulse on ST_CP line
//void HC595Latch()
//{
   ////Pulse the Store Clock
//
   //HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
   //_delay_loop_1(1);
//
   //HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
   //_delay_loop_1(1);
//}
//
//
///*
//
//Main High level function to write a single byte to
//Output shift register 74HC595. 
//
//Arguments:
   //single byte to write to the 74HC595 IC
//
//Returns:
   //NONE
//
//Description:
   //The byte is serially transfered to 74HC595
   //and then latched. The byte is then available on
   //output line Q0 to Q7 of the HC595 IC.
//
//*/
//void HC595Write(uint8_t data)
//{
   ////Send each 8 bits serially
//
   ////Order is MSB first
   //for(uint8_t i=0;i<8;i++)
   //{
      ////Output the data on DS line according to the
      ////Value of MSB
      //if(data & 0b10000000)
      //{
         ////MSB is 1 so output high
//
         //HC595DataHigh();
      //}
      //else
      //{
         ////MSB is 0 so output high
         //HC595DataLow();
      //}
//
      //HC595Pulse();  //Pulse the Clock line
      //data=data<<1;  //Now bring next bit at MSB position
//
   //}
//
   ////Now all 8 bits have been transferred to shift register
   ////Move them to output latch at one
   //HC595Latch();
//}
//
///*
//
//Simple Delay function approx 0.5 seconds
//
//*/
//
//void Wait()
//{
   //for(uint8_t i=0;i<30;i++)
   //{
      //_delay_loop_2(0);
   //}
//}

unsigned char t[] = {0b00000001
    ,0b00000001
    ,0b01111111
    ,0b00000001
,0b00000001};
//unsigned char LFARR[8]={0x00,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00}; //LED Matrix pattern for left arrow
    unsigned char RTARR[8]={0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x00}; //LED Matrix pattern for right arrow
    unsigned char DNARR[8]={0x00,0x10,0x30,0x7E,0x7E,0x30,0x10,0x00}; //LED Matrix pattern for down arrow
    unsigned char UPARR[8]={0x00,0x08,0x0C,0x7E,0x7E,0x0C,0x08,0x00}; //LED Matrix pattern for up arrow
    unsigned char LFARR[8]={0x00,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00};
        unsigned char DISPLAY[8]={0,0,0,0,0,0,0,0};
unsigned char all[8] = {0xF0, 0x0F, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char arrL[] = {0b0111111100100000, 0b1011111101100000, 0b1101111111100000, 0b1110111111111111, 0b1111011111111111, 0b1111101111100000, 0b1111110101100000, 0b1111111000100000};
    unsigned char arrow[] = {0b0111111100000000,
        0b1011111100100000,
        0b1101111101100000,
        0b1110111111111111,
        0b1111011111111111,
        0b1111101101100000,
        0b1111110100100000,
    0b1111111000000000};
    unsigned char x[] = {0b00010010};
    //unsigned char spriteName[] = { 0b00010000,0b00100000,0b01000000,0b11111111,0b11111111,0b01000000,0b00100000,0b00010000,0b00010000,0b00100000,0b01000000,0b11111111,0b11111111,0b01000000,0b00100000,0b00010000};
        unsigned char spriteName[8] = {0b00000001,0b00000010,0b00000100,0b11111111,0b11111111,0b00000100,0b00000010,0b00000001};
    unsigned char led_pattern[8]={
        0b10000001,
        0b11000011,
        0b11100111,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00011000,
        0b11111111,
    };
//void main()
//{
   //uint8_t led_pattern[8]={
                        //0b10000001,
                        //0b11000011,
                        //0b11100111,
                        //0b11111111,
                        //0b01111110,
                        //0b00111100,
                        //0b00011000,
                        //0b11111111,
                     //};
//
   ////Initialize HC595 system
//
   //HC595Init();
//
   //while(1)
   //{
      //for(uint8_t i=0;i<8;i++)
      //{
         //HC595Write(led_pattern[i]);   //Write the data to HC595
         //Wait();                 //Wait 
//
      //}
   //}
//
//
//}
//enum States {start, wait, wait2} state;
//void Tick(){
    //
    //switch(state){
       //case start:
       //state = wait;
       //break;
       //case wait:
       //state = wait;
       //break;
       //case wait2:
       //state = wait;
       //break;
    //}
    //switch(state){
        //case start:
        ////HC595Write(255);
        //break;
        //case wait:
        //for(unsigned char i =0; i<8; i++){
            ////HC595Write(128>>i);
            //HC595Write(~LFARR[i]);
        //}
        //break;
        //case wait2:
        ////HC595Write(255);
        //break;
    //}
    //
//}
//
//int main(void){
    //DDRB = 0xFF; PORTB = 0x00;
    //HC595Init();
    ////TimerSet(1000);
    ////TimerOn();
 //
    //
    //
    ////for(unsigned char i =0; i<8; i++){
        ////HC595Write(LFARR[i]);
    ////}
    //while(1){
        ////Tick();
        ////while(!TimerFlag){}
        ////TimerFlag = 0;
        ////HC595Write(0xFF);
        ////for(unsigned char i =0; i<8; i++){
            ////
            ////HC595Write(led_pattern[i]);
        ////}
        //for(unsigned char i =0; i<8; i++){
            ////HC595Write(128>>i);
            //HC595Write(x[i]);
            ////HC595Write(LFARR[i]);
        //}
           //
    //}
//}

#include <avr/io.h>
#include <util/delay.h>
#include "io.c"
#include "io.h"
#include <util/delay_basic.h>
#include "timer.h"

//BIT OPERATIONS
#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

//CONNECTIONS
#define SHIFT_PORT	PORTB
#define SHIFT_DDR	DDRB

#define DATA_LINE 0		//Data pin location

#define SHIFT_LINE 1	//Shift Clock pin location
#define STORE_LINE 2	//Store Clock pin location


//Initialize Shift Register
void ShRegInit() {
    // Data, Shift clock, Store Clock output
    SET_BIT(SHIFT_DDR,SHIFT_LINE);
    SET_BIT(SHIFT_DDR,STORE_LINE);
    SET_BIT(SHIFT_DDR,DATA_LINE);
}

//Set Data line high
void ShRegDataHigh() {
    SET_BIT(SHIFT_PORT,DATA_LINE);
}

//Set Data line low
void ShRegDataLow() {
    CLR_BIT(SHIFT_PORT,DATA_LINE);
}

//Pulse Shift Clock
void ShRegPulse() {
    SET_BIT(SHIFT_PORT,SHIFT_LINE);
    CLR_BIT(SHIFT_PORT,SHIFT_LINE);
}

//Pulse Store Clock
void ShRegLatch() {
    SET_BIT(SHIFT_PORT,STORE_LINE);
    _delay_loop_1(1);

    CLR_BIT(SHIFT_PORT,STORE_LINE);
    _delay_loop_1(1);
}


void ShRegWrite(unsigned char data) {
    for(unsigned char i=0;i<8;i++) {
        if(data & 0b10000000) {
            ShRegDataHigh();
            } else {
            ShRegDataLow();
        }

        ShRegPulse();
        data=data<<1;
    }

    ShRegLatch();
}

int main(void){
    ShRegInit();
    DDRB = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    unsigned char button = 0;
    //ShRegWrite(255);
    TimerSet(200);
    TimerOn();
    while(1){
        button = ~PINA & 0x01;
        //if(button){
        for(unsigned char i =0; i<8; i++){
            
            ShRegWrite(arrow[i]);
            //ShRegWrite(128>>i);
            delay_ms(5);
            
            }
    
    //while(!TimerFlag){}
    //TimerFlag = 0;
        //
    }
}

enum PORT_TRANSMIT { PORT_B_A, PORT_B_B };
void transmit_data(unsigned char data, unsigned char p) {
    switch(p) {
        case PORT_B_A:
        for (int i = 0; i < 8 ; ++i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTB = 0x08;
            // set SER = next bit of data to be sent.
            PORTB |= ((data >> i) & 0x01);
            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTB |= 0x02;
        }
        // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
        PORTB |= 0x04;
        // clears all lines in preparation of a new transmission
        PORTB = 0x00;
        break;
        case PORT_B_B:
        for (int i = 0; i < 8 ; ++i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTB = 0x08 << 4;
            // set SER = next bit of data to be sent.
            PORTB |= ((data >> i) & 0x01) << 4;
            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTB |= 0x02 << 4;
        }
        // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
        PORTB |= 0x04 << 4;
        // clears all lines in preparation of a new transmission
        PORTB = 0x00;
        break;
        default:
        break;
    }
}

//int main(void){
    //DDRB = 0xFF; PORTB = 0x00;
    //
    //while(1){
        ////transmit_data(0xFF, PORT_B_A);
        ////transmit_data(0xFF, PORT_B_B);
        //for(unsigned char i =0; i<8; i++){
            ////ShRegWrite(128>>i);
            ////ShRegWrite(LFARR[i]);
            ////delay_ms(5);
            //transmit_data(UPARR[i], PORT_B_A);
            //transmit_data(UPARR[i], PORT_B_B);
            //
        //}
    //
    //}
//}
