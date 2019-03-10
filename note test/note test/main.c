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
#include "piano.h"


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
    TCCR1B = 0x0B;
    
    OCR1A = 125;
    
    TIMSK1 = 0x02;
    
    TCNT1 = 0;
    
    _avr_timer_cntcurr = _avr_timer_M;
    
    SREG |= 0x80;
}

void TimerOff(){
    TCCR1B = 0x00;
}

void TimerISR(){
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M){
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}
// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
    static double current_frequency; // Keeps track of the currently set frequency
    // Will only update the registers when the frequency changes, otherwise allows
    // music to play uninterrupted.
    if (frequency != current_frequency) {
        if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
        else { TCCR0B |= 0x03; } // resumes/continues timer/counter
        
        // prevents OCR3A from overflowing, using prescaler 64
        // 0.954 is smallest frequency that will not result in overflow
        if (frequency < 0.954) { OCR0A = 0xFFFF; }
        
        // prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
        else if (frequency > 31250) { OCR0A = 0x0000; }
        
        // set OCR3A based on desired frequency
        else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

        TCNT0 = 0; // resets counter
        current_frequency = frequency; // Updates the current frequency
    }
}

void PWM_on() {
    TCCR0A = (1 << COM0A0) | (1 << WGM00);
    // COM3A0: Toggle PB3 on compare match between counter and OCR0A
    TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
    // WGM02: When counter (TCNT0) matches OCR0A, reset counter
    // CS01 & CS30: Set a prescaler of 64
    set_PWM(0);
}

void PWM_off() {
    TCCR0A = 0x00;
    TCCR0B = 0x00;
}

enum States{start, wait, loop} state;
   
unsigned char button1 = 0;
unsigned char button2 = 0;
unsigned char button3 = 0;

unsigned char n = 0;
unsigned char noteCount = 0;



double notesDIV[songSize] =        { 0, A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,CS4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,FS4,A4,D4,D4,D4,D4,D4,D4,D4,A4,FS4,FS4,FS4,FS4,FS4,CS5,D5,B4,FS4,B4,A4,A4,A4,A4,A4,A4, 0};
unsigned char timesDIV[songSize] = {16,  4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2,  4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4,  2,  2,  2, 4, 4, 2, 4, 2, 2, 4, 8, 4,  4,  2,  4,  2,  2,  4, 4, 4,  4, 4, 4, 2, 4, 2, 2, 4, 0};
unsigned char restsDIV[songSize] = {16,  4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  8,  2,  9,  2,  2,  4,  2,  2,  2,  4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4,  2,  2,  2, 4, 8, 2, 8, 2, 2, 9, 0, 4,  9,  2,  8,  1,  2,  4, 0, 0,  0, 4, 8, 2, 8, 2, 2, 0,32};
double notesEON[songSize] =        { 0,A3,C4,E4,G4,A4,A4,A4,A4,G4,G4,G4,G4,A4,E4,E4,E4,E4,C5,B4,G4,E4,D4,B3,C5,B4,G4,E4,D4,B3,D4,G4,E4,E4,E4,E4,D4,D4,D4,C4,B3,A3,A3,A3,A3,A3,A3,A3,C4,E4,G4,A4,A4,A4,A4,G4,G4,G4,G4,A4,E4,E4,E4,E4,C5,B4,G4,E4,D4,B3,C5,B4,G4,E4,D4,B3,D4,G4,E4,E4,E4,E4,D4,D4,D4,E4,C4,B3,A3,A3,A3,A3,A3, 0};

//double gNotes[] =       {A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3};
double eNotes[] = {B6, B6, B6, A6, D6, D6, D6, D6, B7, D6, E6, E6, B7, B7, B7, 0, B7, D6, B7, D6, E6, B7, B7, B7, B6, B6, B6, A6, D6, D6, D6, D6, B7, E6, E6, E6, E6, E6, E6, E6};
    //double eNotes[] = {B5, B5, B5, A5, D5, D5, D5, D5};
//double gNotes[] =       {0, A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3};
//double gNotes[] =       {0, A4,A4,G4,FS4,CS4,CS4,B3,B3};
unsigned char rests[] = {16, 2, 2, 2, 2, 2, 2, 2, 2};
unsigned char times[] = {16, 4, 4, 2, 2, 4, 4, 4, 4};                                                                                   
//double lNotes[] = {DS4,DS4,AS4,D5,DS5,G4, G4,AS4,D5,DS5,G4, G4,AS4,D5,DS5,G4, G4,AS4,D5,DS5,};
double lNotes[] = {DS4,DS4,DS4, DS4,AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, G4, G4, G4, AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, G4, G4, G4, AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, DS4, DS4, DS4, DS4,AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, G4, G4, G4, AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, G4, G4, G4, AS4,AS4, AS4,D5,D5, DS5, DS5,DS5, DS4, DS4, DS4, DS4 };
//double gNotes[] = {A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3, A4,A4,A4, G4,FS4, CS4,CS4,CS4,B3, B3, B3};
double gNotes[] = {A4, A4, A4, G4, G4, FS4, FS4, FS4, CS4, CS4, CS4, CS4, B3, B3, B3, B3, A4, A4, A4, G4, G4, FS4, FS4, FS4, CS4, CS4, CS4, CS4, B3, B3, B3, B3};
//double lNotes[] = {DS3,DS3,DS3,AS3,AS3,AS3,D4,D4,D4,DS4,DS4,DS4};
unsigned char j = 0;
unsigned char i = 0;
void Tick(){
    button1 = ~PINA & 0x01;
    button2 = ~PINA & 0x02;
    button3 = ~PINA & 0x04;
    switch(state){
        case start:
            state = wait;
            break;
        case wait:
        
       
            if(button1){
                PWM_on();
                state = loop;
            }
            else{
                state = wait;
                set_PWM(0);
                
            }
            state = loop;
            break;
        case loop:
        state = loop;
        break;
       
    }
    switch(state){
        case start:
        i = 0;
        break;
        case wait:
        i = 0;
        j = 0;
        n = 0;
        break;
        case loop:
        //if(i < timesDIV[noteCount]){
            if(i < 73){
        //if(i < times[9]){  //g24 l20
           set_PWM(lNotes[i]);
           
           i++;   
           noteCount++;            
              
              
        }
        /*else{
            if(j < restsDIV[noteCount]){
                set_PWM(0);
                j++;
            }
            else{
              i = 0;
              j = 0;
              if(noteCount < 73){
                  noteCount++;
              }
              else{
                  break;
              }  
            }
        }
        */
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
         break;   
        
        default:
        state = start;
        break;              
            
    }
}    

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
   
    TimerSet(100);
    TimerOn();
    while (1) 
    {
        Tick();
        while(!TimerFlag){}
        TimerFlag = 0;
    }
}
