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

#define A4 440

#define G4 392
#define FS4 370
#define CS4 277
#define B3 247


#define DS4 311
#define D4 294
#define AS3 233
#define G3 196
#define DS3 156
double gNotes[] =       {A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3};
//double gNotes[] =       {0, A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3,A4,A4,G4,FS4, CS4,CS4,B3, B3};
//double gNotes[] =       {0, A4,A4,G4,FS4,CS4,CS4,B3,B3};
unsigned char rests[] = {16, 2, 2, 2, 2, 2, 2, 2, 2};
unsigned char times[] = {16, 4, 4, 2, 2, 4, 4, 4, 4};                                                                                   
double lNotes[] = {DS3,DS3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,G3, G3,AS3,D4,DS4,};

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
            break;
        case loop:
        if(j == 1){
            state = wait;
        }
        else{
            state = loop;
        }
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
        if(i < 16){
        //if(i < times[9]){  //g24 l20
           set_PWM(gNotes[i]);
           if(i == 15){
                j = 1;
           }
           i++;               
              
              
        }
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
   
    TimerSet(200);
    TimerOn();
    while (1) 
    {
        Tick();
        while(!TimerFlag){}
        TimerFlag = 0;
    }
}