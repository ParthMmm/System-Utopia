/*
 * main.c
 *
 * Created: 2/27/19 11:07:54 AM
 * Author : parthmangrola
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <timer.h>
#include <io.h>
#include "io.c"
#include "shift.c"
#include "scheduler.h"

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
    
unsigned char gvAS[12] = {71, 104, 111, 115, 116, 32, 86, 111, 105, 99, 101, 115};


task tasks[2];
const unsigned short tasksNum = 2;
unsigned char tasksPeriod = 100;


enum lButton {start, press};
enum LCD_States {LCDstart, titleScreen, selectSong, wait};
    

unsigned char button;

#define leftButton ~PINA & 0x01
unsigned char a = 0;
unsigned char lfClick = 0;

int TickFct_lButton(int state){
    switch(state){
        case start:
        state = press;
        break;
        case press:
        state = press;
        break;
        default:
        state = start;
        break;
    }
    switch(state){
        case start:
        break;
        case press:
        if(leftButton){
            lfClick = 1;
            //LCD_DisplayString(1, "press");
        }
        else{
            lfClick = 0;
            //LCD_DisplayString(1, "nope");
        }
        break;
        default:
        break;
    }
}

int TickFct_LCD(int state){
    switch(state){ //Transitions
        
        case LCDstart:
        //if(button){
           //state = titleScreen; 
        //}
        //else{
            //state = LCDstart;
        //}
        if(lfClick){
           
           state = titleScreen; 
           LCD_ClearScreen();
           
        }
        else{
            state = LCDstart;
        }
        
        
        break;
        case titleScreen:
        delay_ms(5000);
        if(lfClick){
            
            state = selectSong;
            LCD_ClearScreen();
            
        }
        else{
            state = LCDstart;
        }
        
        
        break;
        case selectSong:
        
        break;
        case wait:
        if(lfClick){
            state = selectSong;
        }
        else{
            state = wait;
        }
        break;
        default:
        break;
    }   
    switch(state){ //Actions
        case LCDstart:
        
        delay_ms(30);
        LCD_DisplayString(1, "start");
        
        break;
        case titleScreen:
         
         
         delay_ms(30);
         
         
         LCD_Cursor(0x05);
         LCD_WriteData(6);
         LCD_WriteData(121);
         LCD_WriteData(6);
         LCD_WriteData(84);
         LCD_WriteData(7);
         LCD_WriteData(77);
         
         LCD_Cursor(0x15);
         
         //for(unsigned char i = 0; i<6; i++){
             //LCD_WriteData(i);
         //}
         LCD_WriteData(0);
         LCD_WriteData(1);
         LCD_WriteData(2);
         LCD_WriteData(3);
         LCD_WriteData(4);
         LCD_WriteData(5);
        break;
        case selectSong:
        delay_ms(30);
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<12; i++){
        LCD_WriteData(gvAS[i]);
        }
        LCD_Cursor(0x00);
        
        //CD_DisplayString(1, "Song 1");
        //LCD_Cursor(0x11);
        //LCD_DisplayString(20, "Song 2");
        break;
        case wait:
        LCD_ClearScreen();
        break;
        default:
        break;
    }
    
    return state;
}    
int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    LCD_init();
    
    unsigned char i = 0;
    
    tasks[i].state = start;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_lButton;
    i++;
    tasks[i].state = LCDstart;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_LCD;
    
    
    
    
    TimerSet(5);
    TimerOn();
    while (1) 
    {
        unsigned char i;
        for (i = 0; i < tasksNum; ++i) {
            if ( tasks[i].elapsedTime >= tasks[i].period ) {
                tasks[i].state = tasks[i].TickFct(tasks[i].state);
                tasks[i].elapsedTime = 0;
            }
            tasks[i].elapsedTime += 5;
        }
      while(!TimerFlag);
      TimerFlag = 0;  
      
      sleep_mode();
    }
}

