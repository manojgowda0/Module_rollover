 #include<avr/io.h>
 #include<util/delay.h>
#include <avr/io.h>
#include<avr/interrupt.h>
#define set_bit(port,pin)  port|=(1<<pin)
#define clr_bit(port,pin)  port&=~(1<<pin)

struct
{
 volatile unsigned int Flag_ISR1:1; 
}Flag_Bit;

void adc_int()			//adc intiliztion
{
   ADMUX|=(1<<MUX0);		//connect two pots to A1 and A2
   ADMUX|=(1<<REFS0);					
   ADCSRA |=(1<<ADEN);
   ADCSRA |=(1<<ADPS1)|(1<<ADPS2);
}
 
void igntion_int()			//ignation intilization	
{
  EICRA|=(1<<ISC10);		//ingtion switch at -3 pin
    EICRA&=~(1<<ISC11);
    EIMSK|=(1<<INT1);
  sei();

}

void pin_dir()				//pin dirction intiliztion
{
  set_bit(DDRD,PD7);		//	two leds two 7 and 6
   set_bit(DDRD,PD6);
   set_bit(DDRD,PD2);		//motor to pin 2
}


int main()
{
   uint16_t ADC_VALUE;
  adc_int();
  igntion_int();
  while(1)
  {
    
   ADCSRA|=(1<<ADSC);
    while(ADCSRA& (1<<ADSC)&&Flag_Bit.Flag_ISR1==1);
    ADC_VALUE=ADC;
    if(ADC_VALUE>500&&Flag_Bit.Flag_ISR1==1)
    {
     set_bit(PORTD,PD7);
     clr_bit(ADMUX,MUX0);
     ADMUX|=(1<<MUX1);
    ADCSRA|=(1<<ADSC);
    while(ADCSRA& (1<<ADSC)&&Flag_Bit.Flag_ISR1==1);
      {
      ADC_VALUE=ADC;
      }
    if(ADC_VALUE>500&&Flag_Bit.Flag_ISR1==1)
    {
      set_bit(PORTD,PD6);
       set_bit(PORTD,PD2);
      _delay_ms(10);
      clr_bit(PORTD,PD2);
    }
    }
  	clr_bit(PORTD,PD7);
    clr_bit(PORTD,PD6);
    clr_bit(ADMUX,MUX1);
    ADMUX|=(1<<MUX0);
    }
}
ISR(INT1_vect)
{
	 cli();
     Flag_Bit.Flag_ISR1=!Flag_Bit.Flag_ISR1;
     sei(); 
}

