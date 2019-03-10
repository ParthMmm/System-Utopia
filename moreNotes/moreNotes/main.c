/*Name & E-mail: Parth Mangrola pmang002@ucr.edu
* Lab Section: 021
* Assignment: Lab #9 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding
* template or example code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include "scheduler.h"
#include "piano.h"
#include "pwm.h"
#include "io.c"
#include "max7219.c"
#include "ledmatrix7219d88.c"
#include "max7219.h"
#include "ledmatrix7219d88.h"



enum States{startP, wait, loop} ;
   enum matrix {startM, loop1, loop2, loop3} ;
       enum lButton {start, press} ;


unsigned char button1 = 0;
unsigned char button2 = 0;
unsigned char button3 = 0;
task tasks[3];
const unsigned short tasksNum = 3;
unsigned char tasksPeriod = 5;

#define leftButton ~PINA & 0x01
unsigned char lfClick = 0;
unsigned char n = 0;

unsigned char bPlay = 0;
double notesDIV[songSize] =        { 0, A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,CS4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,CS5,D5,B4,FS4,B4,A4,A4,A4,A4,A4,A4, 0};
double notesEON[songSize] =        { 0,A3,C4,E4,G4,A4,A4,A4,A4,G4,G4,G4,G4,A4,E4,E4,E4,E4,C5,B4,G4,E4,D4,B3,C5,B4,G4,E4,D4,B3,D4,G4,E4,E4,E4,E4,D4,D4,D4,C4,B3,A3,A3,A3,A3,A3,A3,A3,C4,E4,G4,A4,A4,A4,A4,G4,G4,G4,G4,A4,E4,E4,E4,E4,C5,B4,G4,E4,D4,B3,C5,B4,G4,E4,D4,B3,D4,G4,E4,E4,E4,E4,D4,D4,D4,E4,C4,B3,A3,A3,A3,A3,A3, 0};
unsigned char blank[8]={0,0,0,0,0,0,0,0};

unsigned char spriteName[] = { 0b11111111,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b11111111};

double gNotes[] =       {0,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3,A4,A4,A4,G4,FS4,CS4,CS4,CS4,B3,B3,B3};
unsigned char gRest[] = {8, 0, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 0, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 1, 0, 0, 1,  0,  1,  0,  0, 1, 0, 0, 1, 0, 0, 1,  0,  1,  0,  0, 0, 0, 0};
double eNotes[] = {B6, B6, B6, A6, D6, D6, D6, D6, B7, D6, E6, E6, B7, B7, B7, 0, B7, D6, B7, D6, E6, B7, B7, B7, B6, B6, B6, A6, D6, D6, D6, D6, B7, E6, E6, E6, E6, E6, E6, E6};
    //double eNotes[] = {B5, B5, B5, A5, D5, D5, D5, D5};
//double gNotes[] =       {0, A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3};
//double gNotes[] =       {0, A4,A4,G4,FS4,CS4,CS4,B3,B3};
unsigned char rests[] = {16, 2, 2, 2, 2, 2, 2, 2, 2};
unsigned char times[] = {16, 4, 4, 2, 2, 4, 4, 4, 4};                                                                                   
double lNotes[] = {DS3,DS3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,};

//double lNotes[] = {DS3,DS3,DS3,AS3,AS3,AS3,D4,D4,D4,DS4,DS4,DS4};
unsigned char j = 0;
unsigned char i = 0;

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
            LCD_DisplayString(1, "press");
            
            
        }
        else{
            lfClick = 0;
            LCD_DisplayString(1, "nope");
        }
        break;
        default:
        break;
    }
}
int TickFct_play(int state){
   
    switch(state){
        case startP:
            state = wait;
            break;
        case wait:
        PWM_on();
        bPlay = 1;
        state = loop;
       
            //if(lfClick){
                //PWM_on();
                //bPlay = 1;
                //state = loop;
            //}
            //else{
                //state = wait;
                //set_PWM(0);
                //
            //}
            break;
        case loop:
        state = loop;
        break;
       
    }
    switch(state){
        case startP:
        i = 0;
        break;
        case wait:
        i = 0;
        j = 0;
        n = 0;
        break;
        case loop:
        //if(j < gRest[n]){
            //set_PWM(0);
            //++j;
        //}
        //else{
            //j = 0;
            //if(i < 70){
            ////if(i < times[9]){  //g24 l20
               //set_PWM(gNotes[n]);
               //
               //i++;
               //n++;               
              //
              //
            //}
        //}        
        /*else{
            if(j < rests[9]){
                set_PWM(0);
                j++;
            }
            else{
                i = 0;
                j = 0;
                if(n < 9){
                    n++;
                }
            }
            set_PWM(0);
        }*/
        if(i < 70){
            set_PWM(gNotes[i]);
            
            i++;
            n++;
            
        }
         break;   
        
        default:
        state = startP;
        break;              
            
    }
}

int TickFct_matrix(int state){
    switch(state){
        case startM:
        delay_ms(10);
        if(bPlay){
            state = loop1;
        }
        else{
            state = startM;
        }
        //state = loop1;
        break;
        
        case loop1:
        delay_ms(10);
        state = loop2;
        break;
        
        case loop2:
        delay_ms(10);
        state = loop3;
        break;
        
        case loop3:
        delay_ms(10);
        state = loop1;
        break;
        
        default:
        state = startM;
        break;
        
    }
    switch(state){
        case startM:
        
        break;
        case loop1:
        
        for(unsigned char k = 0; k < 8;k++) {
            blank[k] <<= 1;
            
        }
        blank[1] |= 1;
        blank[2] |= 1;
        delay_ms(50);
        ledmatrix7219d88_setmatrix(0, blank);
        
        break;
        
        case loop2:
        for(unsigned char k = 0; k < 8;k++) {
            blank[k] <<= 1;
            
        }
        blank[3] |= 1;
        blank[4] |= 1;
        delay_ms(50);
        ledmatrix7219d88_setmatrix(0, blank);
        break;
        
        case loop3:
        for(unsigned char k = 0; k < 8;k++) {
            blank[k] <<= 1;
            
        }
        blank[5] |= 1;
        blank[6] |= 1;
        delay_ms(50);
        ledmatrix7219d88_setmatrix(0, blank);
        
        default:
        break;
        
    }
    
}
    

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    ledmatrix7219d88_init();
    ledmatrix7219d88_setmatrix(0, spriteName);
    unsigned char i = 0;
    
    tasks[i].state = start;
    tasks[i].period = 40;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_lButton;
    i++;
    tasks[i].state = startP;
    tasks[i].period = 15;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_play;
    i++;
    
    tasks[i].state = startM;
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
        
      
    }
}