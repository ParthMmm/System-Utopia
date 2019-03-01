/*
 * button test.c
 *
 * Created: 2/27/19 10:26:22 AM
 * Author : parthmangrola
 */ 

#include <avr/io.h>
#include "timer.h"
enum States {start, wait, next}state;
    
  
unsigned char button = 0;

void Tick(){
    switch(state){
        
    
        case start:
        state = wait;
        break;
        case wait:
        button = ~PINA & 0x01;
        if(button){
            PORTB = 0x03;
            
        }
        state = next;
        case next:
        PORTB = 0x00;
        state = start;
        
        default:
        break;
    
    
    }
    switch(state){
    case start:
    PORTB = 0x00;
    break;
    case wait:
    break;
    default:
    break;
    }    
}    
int main(void)
{
    
    DDRA = 0x00; PORTA = 0xFF;
    PORTB = 0xFF; PORTB = 0x00;
    /* Replace with your application code */
    TimerSet(1000);
    TimerOn();
    while (1) 
    {
      Tick();
      while(!TimerFlag){
          TimerFlag = 0;
      }
    }
}

