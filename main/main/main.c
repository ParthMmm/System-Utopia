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
    
unsigned char gvAS[] = {71, 104, 111, 115, 116, 32, 86, 111, 105, 99, 101, 115}; //ghost voices
unsigned char plAS[] = {80, 108, 97, 121}; //4
unsigned char hiAS[] = {72, 105, 103, 104, 32, 83, 99, 111, 114, 101}; //10


task tasks[4];
const unsigned short tasksNum = 4;
unsigned char tasksPeriod = 100;


enum lButton {start, press};
enum dButton {startD, pressD};
enum uButton {startU, pressU};
enum LCD_States {LCDstart, titleScreen, selectSong, selectSongUp, selectSongDown,songSelected, wait};
    

unsigned char button;
unsigned char currentSong;

#define leftButton ~PINA & 0x01
#define upButton ~PINA & 0x02
#define downButton ~PINA & 0x04

unsigned char a = 0;
unsigned char lfClick = 0;
unsigned char dnClick = 0;
unsigned char upClick = 0;

unsigned char cursorLOC = 0;

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

int TickFct_dButton(int state){
    switch(state){
        case startD:
        state = pressD;
        break;
        case pressD:
        state = pressD;
        break;
        default:
        state = startD;
        break;
    }
    switch(state){
        case startD:
        break;
        case pressD:
        if(downButton){
            dnClick = 1;
            //LCD_DisplayString(1, "press");
        }
        else{
            dnClick = 0;
            //LCD_DisplayString(1, "nope");
        }
        break;
        default:
        break;
    }
}

int TickFct_uButton(int state){
    switch(state){
        case startU:
        state = pressU;
        break;
        case pressU:
        state = pressU;
        break;
        default:
        state = startU;
        break;
    }
    switch(state){
        case startU:
        break;
        case pressU:
        if(upButton){
            upClick = 1;
            //LCD_DisplayString(1, "press");
        }
        else{
            upClick = 0;
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
        
        if(lfClick){
           state = titleScreen; 
           LCD_ClearScreen();        
        }

        break;
        case titleScreen:
        delay_ms(3000);
        if(lfClick){
            delay_ms(3000);
            state = selectSong;
            LCD_ClearScreen();         
        }
        else{
            state = titleScreen;
        }
 
        break;
        
        case selectSong:
        //delay_ms(50);
        if(dnClick && !upClick){  
            state = selectSongDown;
            LCD_ClearScreen();
        }
        else if(!dnClick && upClick){
            state = selectSongUp;
            LCD_ClearScreen();
        }
        else if(lfClick){
            state = songSelected;
            LCD_ClearScreen();
            currentSong = "Ghost Voices";
        }
        else{
            state = selectSong;
            
        }
        
        break;
        case selectSongUp:
        delay_ms(100);
        if(lfClick){
            state = songSelected;
            LCD_ClearScreen();  
            currentSong = "Ghost Voices";
        }
        else if(!dnClick && upClick){
            state = selectSongUp;
            LCD_ClearScreen();
        }
        else if(dnClick && !upClick){
            state = selectSongDown;
            LCD_ClearScreen();
        }
        else{
            state = selectSongUp;
        }
        
        break;
        case selectSongDown:
        delay_ms(100);
        if(lfClick){
            state = songSelected;
            LCD_ClearScreen();  
            currentSong = "Easy";
        }
        else if(!dnClick && upClick){
            state = selectSongUp;
            LCD_ClearScreen();
        }
        else if(dnClick && !upClick){
            state = selectSongDown;
            LCD_ClearScreen();
        }
        else{
            state = selectSongDown;
            
        }         
        break; 
        case songSelected:
        state = songSelected;       
        break;
        case wait:
        
        break;
        default:
        break;
    }   
    switch(state){ //Actions
        case LCDstart:
        
        delay_ms(30);
        LCD_DisplayString(1, "Welcome");
        break;
        
        case titleScreen:
         delay_ms(30);
         
         //System
         LCD_Cursor(0x05);
         LCD_WriteData(6);
         LCD_WriteData(121);
         LCD_WriteData(6);
         LCD_WriteData(84);
         LCD_WriteData(7);
         LCD_WriteData(77);
       
         LCD_Cursor(0x15);
         
         //Utopia
         LCD_WriteData(0);
         LCD_WriteData(1);
         LCD_WriteData(2);
         LCD_WriteData(3);
         LCD_WriteData(4);
         LCD_WriteData(5);
        break;
        
        case selectSong:
        delay_ms(30);
        LCD_Cursor(0x02);
        LCD_WriteData(0xA5);
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        
        LCD_Cursor(19);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        LCD_Cursor(1);
        
        break;
        
        case selectSongUp:
        delay_ms(100);
        LCD_Cursor(0x02);
        LCD_WriteData(0xA5);
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        
        LCD_Cursor(19);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        LCD_Cursor(1);
        cursorLOC = 1;
       
        break;
        
        case selectSongDown:
        delay_ms(100);
        
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        LCD_Cursor(18);
        LCD_WriteData(0xA5);
        LCD_Cursor(19);
        for(unsigned char i = 0; i<12; i++){
            LCD_WriteData(gvAS[i]);
        }
        LCD_Cursor(17);
        cursorLOC = 2;
        break;
        
        case wait:
        LCD_ClearScreen();
        break;
        case songSelected:
        delay_ms(100);
        LCD_Cursor(0x02);
        LCD_WriteData(0xA5);
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<4; i++){
            LCD_WriteData(plAS[i]);
        }
        
        LCD_Cursor(19);
        for(unsigned char i = 0; i<10; i++){
            LCD_WriteData(hiAS[i]);
        }
        LCD_Cursor(1);
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
    tasks[i].state = startD;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_dButton;
    i++;
    tasks[i].state = startU;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_uButton;
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

