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
#include "pwm.h"
#include "piano.h"
#include <avr/eeprom.h>

#include "max7219.c"
#include "ledmatrix7219d88.c"
#include "max7219.h"
#include "ledmatrix7219d88.h"

    
unsigned char gvAS[] = {71, 104, 111, 115, 116, 32, 86, 111, 105, 99, 101, 115}; //ghost voices
unsigned char plAS[] = {80, 108, 97, 121}; //4 play
unsigned char hiAS[] = {72, 105, 103, 104, 32, 83, 99, 111, 114, 101}; //10 high score
unsigned char lAS[]  = {76, 105, 111, 110, 104, 101, 97, 114, 116, 101, 100}; //Lionhearted 11
unsigned char psAS[] = {80, 111, 105, 110, 116, 115, 32, 83, 99, 111, 114, 101, 100, 33}; //points scored
unsigned char nAS[] =  {78, 101, 119};  //new
unsigned char timesDIV[songSize] = {16,  4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2,  4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4, 4, 4,  4, 4, 4, 2, 4, 2, 2, 4, 0};

double notesDIV[songSize] =        { 0, A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,CS4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,CS5,D5,B4,FS4,B4,A4,A4,A4,A4,A4,A4, 0};
// double gNotes[] =       {0,A4,A4,G4,FS4,CS4,CS4,B3,B3,A4,A4,A4,A4,G4,FS4,CS4,CS4,B3,B3,A4,A4,A4,A4,G4,FS4,CS4,CS4,B3,B3,A4,A4};
 unsigned char restsDIV[songSize] = {16,  4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  8,  2,  9,  2,  2,  4,  2,  2,  2,  4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4, 0, 0,  0, 4, 8, 2, 8, 2, 2, 0,32};
  

double gNotes[] =       {0,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3};
unsigned char gTimes[] ={8, 6, 6, 6, 4,  4,  6,  6,   6,4, 4, 4, 6, 6, 6, 4,  4,  6,  6,   6,4, 4, 4, 6, 6, 6, 4,  4,  6,  6,   6,4, 4, 4, 6, 6, 6, 4,  4,  6,  6,  6,4, 4, 4};
unsigned char gRest[] = {8, 0, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 0, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 1, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 1, 0, 0, 1,  0,  4,  0,  0, 0, 0, 0}; 
unsigned char gButton[]={0, 1, 1, 1, 2,  3,  4,  4,  4, 2, 2, 2, 1, 1, 1, 2,  3,  4,  4,  4, 2, 2, 2, 1, 1, 1, 2,  3,  4,  4,  4, 2, 2, 2, 1, 1, 1, 2,  3,  4,  4,  4, 2, 2, 2};

    
//double lNotes[] =         {0,DS3,DS3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4};
double lNotes[] =         {0,DS4,DS4,DS4,DS4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,G4,G4,G4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,G4,G4,G4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,DS4,DS4,DS4,DS4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,G4,G4,G4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,G4,G4,G4,AS4,AS4,AS4,D5,D5,DS5,DS5,DS5,DS4,DS4,DS4,DS4};
unsigned char lTimes[] =  {8,  6,  6,  4, 4,  4, 6,  6,  4, 4,  4, 6,  6,  4, 4,  4, 6,  6,  4, 4,  4};
unsigned char lRest[] =   {8,  0,  4,  4, 4,  4, 0,  4,  4, 4,  4, 0,  4,  4, 4,  4, 0,  4,  0, 0,  0};
unsigned char lButton[] = {0,  1,  1,  2, 3,  4, 1,  1,  2, 3,  4, 1,  1,  2, 3,  4, 1,  1,  2, 3,  4};

    
task tasks[7];
const unsigned short tasksNum = 7;
unsigned char tasksPeriod = 5;
unsigned char square1[] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};

enum lButton {start, press};
enum dButton {startD, pressD};
enum uButton {startU, pressU};
enum rButton {startR, pressR};
enum LCD_States {LCDstart, titleScreen, selectSong, selectSongUp, selectSongDown,songSelected, songSelectedUp, songSelectedDown, prepGame, duringGame, highScore, postGame, postGameHS, wait};
enum playSong {startP, play};
enum matrix {startM, loop1, loop2, loop3, idleState};
    
   
    
unsigned char spriteName[] = { 0b11111111,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b11111111};
unsigned char spriteName2[] = { 0b00000000,0b01111110,0b01000010,0b01000010,0b01000010,0b01000010,0b01111110,0b00000000};   

    

unsigned char buffer[4];
unsigned char buffer2[4];
unsigned char clearBuffer[4] = {0};

unsigned char lHighScore[4];
unsigned char gHighScore[4];    
unsigned char gSelected = 0;
unsigned char lSelected = 0;

unsigned short currentScore = 0;
unsigned char currentSong;

unsigned char i,j,n,p = 0;
unsigned char ii = 0;

unsigned char note;
unsigned char led = 0;
unsigned char a = 0;
unsigned char bPlay = 0;
unsigned char songOver = 0;


#define leftButton ~PINA & 0x01
#define upButton ~PINA & 0x02
#define downButton ~PINA & 0x04
#define rtButton ~PINA & 0x08

unsigned char lfClick = 0;
unsigned char dnClick = 0;
unsigned char upClick = 0;
unsigned char rtClick = 0;




unsigned char highScoreAchieved = 0;

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
        }
        else{
            lfClick = 0;
        }
        break;
        default:
        break;
    }
    return state;
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
        }
        else{
            dnClick = 0;
        }
        break;
        default:
        break;
    }
    return state;
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
        }
        else{
            upClick = 0;
        }
        break;
        default:
        break;
    }
    return state;
}

int TickFct_rButton(int state){
    switch(state){
        case startR:
        state = pressR;
        break;
        case pressR:
        state = pressR;
        break;
        default:
        state = startR;
        break;
    }
    switch(state){
        case startR:
        break;
        case pressR:
        if(rtButton){
            rtClick = 1;   
        }
        else{
            rtClick = 0;
        }
        break;
        default:
        break;
    }
    return state;
}

int TickFct_LCD(int state){
    switch(state){ //Transitions
        
        case LCDstart:
        state = titleScreen;

        break;
        case titleScreen:

        if(rtClick){

            state = selectSong;
            LCD_ClearScreen();         
        }
        else{
            state = titleScreen;
        }
 
        break;
        
        case selectSong:

        if(dnClick && !upClick){  
            state = selectSongDown;
            LCD_ClearScreen();
        }
        else if(!dnClick && upClick){
            state = selectSongUp;

        }
        else if(rtClick){
            state = songSelected;
            LCD_ClearScreen();
            gSelected = 1;
        }
        else{
            state = selectSong;
            
        }
        
        break;
        case selectSongUp:
        delay_ms(100);
        if(rtClick){
            state = songSelected;
            LCD_ClearScreen();  
            lSelected = 1;
            
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
        if(rtClick){
            state = songSelected;
            LCD_ClearScreen();  
            lSelected = 1;
   
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
        delay_ms(100);
        if(rtClick){
            state = prepGame;
            LCD_ClearScreen();
        }
        else if(lfClick){
            state = selectSong;
            LCD_ClearScreen();
        }
        else if(!dnClick && upClick){
            state = songSelectedUp;
        }
        else if(dnClick && !upClick){
            state = songSelectedDown;
            LCD_ClearScreen();
        }
        else{
            state = songSelected;   
        }    
        break;
        
        case songSelectedUp: //play
        delay_ms(100);
        if(rtClick){
            state = prepGame;
            LCD_ClearScreen();
        }
        else if(lfClick){
            state = selectSong;
            LCD_ClearScreen();
        }
        else if(!dnClick && upClick){
            state = songSelectedUp;

        }
        else if(dnClick && !upClick){
            state = songSelectedDown;
            LCD_ClearScreen();
        }
        else{
            state = songSelectedUp;
        }
        break;
        
        case songSelectedDown: //highscore
        delay_ms(100);
        if(rtClick){
            state = highScore;
            LCD_ClearScreen();  
        }
        else if(lfClick){
            state = selectSong;
            LCD_ClearScreen();
        }
        else if(!dnClick && upClick){
            state = songSelectedUp;
            LCD_ClearScreen();
        }
        else if(dnClick && !upClick){
            state = songSelectedDown;

        }
        else{
            state = songSelectedDown;
        }
        break;
        
        case highScore:
        delay_ms(100);
        if(lfClick){
            state = songSelected;
            LCD_ClearScreen();  
        }
        else{
            state = highScore;
        }
                  
        break;
        
        case prepGame:
        delay_ms(100);
        if(p == 10){
           state = duringGame; 
           PWM_on();
           LCD_ClearScreen();
           bPlay = 1;
        }
        else{
            state = prepGame;
            p++;
        }
        break;
        
        case duringGame:
        if(songOver && !bPlay){
            state = postGame;
            LCD_ClearScreen();
        }
        else{
            state = duringGame;
        }      
        break;
        
        case postGame:
        delay_ms(300);
        if(lfClick || rtClick || dnClick || upClick){
            if(highScoreAchieved){
                state = postGameHS;
            }
            else{
               state = LCDstart;
               //LCD_ClearScreen(); 
            }
            LCD_ClearScreen();
        }
        else{
            state = postGame;
        }
        break;
        
        case postGameHS:
        if(lfClick || rtClick || dnClick || upClick){ 
            state = LCDstart;
            LCD_ClearScreen();
        }
        else{
            state = postGameHS;
        }
        break;

        case wait:
        
        break;
        default:
        break;
    }   
    switch(state){ //Actions/////////////
        case LCDstart:
        
        LCD_Cursor(0);
        gSelected = 0;
        lSelected = 0;
        bPlay = 0;
        songOver = 0;
        currentScore = 0;
        p = 0;
        j = 0;
        i = 0;
        ii = 0;
        n = 0;
        highScoreAchieved = 0;
        
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
         
         
         LCD_Cursor(32);
         LCD_WriteData(126);
         LCD_Cursor(0);
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
        for(unsigned char i = 0; i<11; i++){
            LCD_WriteData(lAS[i]);
        }
        LCD_Cursor(16);
        LCD_WriteData(126);
        LCD_Cursor(0);
        
        
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
        for(unsigned char i = 0; i<11; i++){
            LCD_WriteData(lAS[i]);
        }
        
        LCD_Cursor(16);
        LCD_WriteData(126);
        LCD_Cursor(0);


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
        for(unsigned char i = 0; i<11; i++){
            LCD_WriteData(lAS[i]);
        }
        LCD_Cursor(32);
        LCD_WriteData(126);
        LCD_Cursor(0);

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
        LCD_Cursor(16);
        LCD_WriteData(126);
        LCD_Cursor(0);
        
       
        break;
        
        case songSelectedUp:
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
        LCD_Cursor(16);
        LCD_WriteData(126);
        LCD_Cursor(0);
        break;
        
        case songSelectedDown:
        delay_ms(100);
        
        LCD_Cursor(0x03);
        for(unsigned char i = 0; i<4; i++){
            LCD_WriteData(plAS[i]);
        }
        
        LCD_Cursor(18);
        LCD_WriteData(0xA5);
        LCD_Cursor(19);
        for(unsigned char i = 0; i<10; i++){
            LCD_WriteData(hiAS[i]);
        }
        LCD_Cursor(32);
        LCD_WriteData(126);
        LCD_Cursor(0);
        break;
        
        case highScore:
        delay_ms(100);

        eeprom_read_block((void*)&gHighScore, (const void*)1, 3);
        eeprom_read_block((void*)&lHighScore, (const void*)7, 3);
        
        if(gSelected){
            LCD_DisplayString(1,gHighScore);
        }
        if(lSelected){
            LCD_DisplayString(1,lHighScore);
        }

        LCD_Cursor(32);
        LCD_WriteData(127);
        LCD_Cursor(0);

        break;
        
        case prepGame:
        

        LCD_Cursor(0);
        
        
        
        break;
        
        case duringGame:
        
        
        if(gSelected){
           itoa(currentScore, buffer, 10);
           LCD_DisplayString(1, buffer);
           if(atoi(gHighScore) < currentScore){
            eeprom_write_block ((void*)&buffer, (const void*)1, 3);
            highScoreAchieved = 1;
           }           
        
        }
        if(lSelected){
            itoa(currentScore, buffer2, 10);
            LCD_DisplayString(1, buffer2); 
            if(atoi(lHighScore) < currentScore){
             eeprom_write_block ((void*)&buffer2, (const void*)7, 3);  
             highScoreAchieved = 1;
            }

        }
        
        LCD_Cursor(17);
        for(unsigned char i = 5; i<10; i++){
            LCD_WriteData(hiAS[i]);
        }
        LCD_Cursor(0);
        
              
        break;
  
        case postGame:
        
        if(gSelected){
            LCD_DisplayString(1,itoa(currentScore, buffer, 10));
        }
        if(lSelected){
            LCD_DisplayString(1,buffer2);
        }
        LCD_Cursor(17);
        for(unsigned char i = 0; i<14; i++){
            LCD_WriteData(psAS[i]);
        }
        
        LCD_Cursor(32);
        LCD_WriteData(126);
        LCD_Cursor(0);

        break;
        
        case postGameHS:
        LCD_Cursor(1);
        for(unsigned char i = 0; i<3; i++){
            LCD_WriteData(nAS[i]);
        }
        LCD_Cursor(17);
        for(unsigned char i = 0; i<10; i++){
            LCD_WriteData(hiAS[i]);
        }
        LCD_WriteData(33);
        LCD_Cursor(32);
        LCD_WriteData(126);
        LCD_Cursor(0);
        break;

        default:
        break;
    }
    
    return state;
}    


int TickFct_play(int state){
    switch(state){
       case startP:
       if(bPlay){
          state = play; 
       }
       else{
           state = startP;
       }
       
       break;
       case play:
       state = play;
       break;
       
       default:
       state = startP;
       break;
    }
    switch(state){
        case startP:
        i = 0;
        ii = 0;
        j = 0;
        n = 0;
        
        break;
        case play:
        if(gSelected){
            if(ii < gTimes[n]){
                set_PWM(gNotes[n]);
                ii++;
                
            }
            else{
                if(j < gRest[n]){
                    set_PWM(0);
                    ++j;
                }
                else{
                    ii = 0;
                    j = 0;
                    if(n < 40){ //fix
                        n++;
                    }
                    else{
                        PWM_off();
                        songOver = 1;
                        bPlay = 0;
                    }
                }
            }
        }
        if(lSelected){
            if(ii < lTimes[n]){
                set_PWM(lNotes[n]);
                ii++;
                
            }
            else{
                if(j < lRest[n]){
                    set_PWM(0);
                    ++j;
                }
                else{
                    ii = 0;
                    j = 0;
                    if(n < 20){ //fix
                        n++;
                    }
                    else{
                        PWM_off();
                        songOver = 1;
                        bPlay = 0;
                    }
                }
            }
        }
        break;
        
        default:
        break;
    }
    return state;
}

int TickFct_matrix(int state){
    switch(state){
        
        
        case idleState:
        if(bPlay){
            state = loop1;
        }
        else{
            state = idleState;  
        }   
        break;
        
        case loop1:
        delay_ms(10);
        if(songOver && !bPlay){
            state = idleState;
        }
        else{
            state = loop1;
        }
        break;
        default:
        state = idleState;
        break;

    }
     switch(state){
         
         case loop1:
         
          for(unsigned char k = 0; k < 8;k++) {
              square1[k] = square1[k] << 1;
              
          }
         
         if(gSelected){
             if (i < gTimes[n]) {
                 note = gButton[n];
                 
                 if (note == 1){
                     square1[0] = square1[0] | 1;
                     square1[1] = square1[1] | 1;
                     if(lfClick && !rtClick && !dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 2){
                     square1[2] = square1[2] | 1;
                     square1[3] = square1[3] | 1;
                     if(!lfClick && !rtClick && dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 3){
                     square1[4] = square1[4] | 1;
                     square1[5] = square1[5] | 1;
                     if(!lfClick && !rtClick && !dnClick && upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 4){
                     square1[6] = square1[6] | 1;
                     square1[7] = square1[7] | 1;
                     if(!lfClick && rtClick && !dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 ++i;
             }
             else {
                 if (j < gRest[n]){
                     ++j;
                 }
                 else{
                     i = 0;
                     j = 0;
                 }
             }
         }
         if(lSelected){
             if (i < lTimes[n]) {
                 note = lButton[n];
                 
                 if (note == 1){
                     square1[0] = square1[0] | 1;
                     square1[1] = square1[1] | 1;
                     if(lfClick && !rtClick && !dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 2){
                     square1[2] = square1[2] | 1;
                     square1[3] = square1[3] | 1;
                     if(!lfClick && !rtClick && dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 3){
                     square1[4] = square1[4] | 1;
                     square1[5] = square1[5] | 1;
                     if(!lfClick && !rtClick && !dnClick && upClick){
                         currentScore++;
                     }
                 }
                 else if (note == 4){
                     square1[6] = square1[6] | 1;
                     square1[7] = square1[7] | 1;
                     if(!lfClick && rtClick && !dnClick && !upClick){
                         currentScore++;
                     }
                 }
                 ++i;
             }
             else {
                 if (j < lRest[n]){
                     ++j;
                 }
                 else{
                     i = 0;
                     j = 0;
                 }
             }
         }
         
         ledmatrix7219d88_setmatrix(0, square1);
           
         break;
         
         case idleState:
            if(lfClick){
                for(led = 39; led > 31; led--){
                    ledmatrix7219d88_setledon(i, led);
                    ledmatrix7219d88_setledon(i, led-8);
                    _delay_ms(450);
                }
                _delay_ms(500);
                for(led = 39; led > 31; led--){
                    ledmatrix7219d88_setledoff(i, led);
                    ledmatrix7219d88_setledoff(i, led-8);
                    _delay_ms(450);
                }
            }                
             else if(rtClick){
                for(led = 32; led < 40; led++){
                    ledmatrix7219d88_setledon(i, led);
                    ledmatrix7219d88_setledon(i, led-8);
                    _delay_ms(450);
                }
                _delay_ms(500);
                for(led = 32; led < 40; led++){
                    ledmatrix7219d88_setledoff(i, led);
                    ledmatrix7219d88_setledoff(i, led-8);
                    _delay_ms(450);
                }
             }
             
             else if(dnClick){
                 for(led = 7; led < 60; led+=8){
                    ledmatrix7219d88_setledon(i, led);
                     _delay_ms(450);
                 }
                 _delay_ms(500);
                 for(led = 7; led < 60; led+=8){
                     ledmatrix7219d88_setledoff(i, led);
                     _delay_ms(450);
                 }
             }

             else if(upClick){
                 for(led = 64; led >0; led-=8){
                     ledmatrix7219d88_setledon(i, led);
                     _delay_ms(450);
                 }
                 _delay_ms(500);
                 for(led = 64; led > 0; led-=8){
                     ledmatrix7219d88_setledoff(i, led);
                     _delay_ms(450);
                 }   
             }
             else{
                 ledmatrix7219d88_resetmatrix(0);
             }             
             
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
    
    ledmatrix7219d88_init();
    ledmatrix7219d88_setintensity(0,1);
    
    //eeprom_write_block ((void*)&clearBuffer, (const void*)1, 3);
    //eeprom_write_block ((void*)&clearBuffer, (const void*)7, 3);
    
    eeprom_read_block((void*)&gHighScore, (const void*)1, 3);
    eeprom_read_block((void*)&lHighScore, (const void*)7, 3);

    
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
    tasks[i].state = startU;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_rButton;
    i++;
    tasks[i].state = LCDstart;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_LCD;
    i++;
    tasks[i].state = startP;
    tasks[i].period = 25;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_play;
    i++;
    tasks[i].state = idleState;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_matrix;
    
    
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
      
      //sleep_mode();
    }
}
