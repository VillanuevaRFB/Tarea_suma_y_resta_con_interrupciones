#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double cuenta=0;
void config_PCI(void){
  PCICR|=0x01;//interrupciones en B
  PCICR|=0x02;//interrupciones en C
  PCMSK0|=0x08;
  DDRB&=~0x08;
  PORTB|=0x08;//+1
  PCMSK1|=0x01;
  DDRC&=~0x01;
  PORTC|=0x01;//-1
}
ISR(PCINT0_vect){
  _delay_ms(50);
  cuenta+=0.5;
  if(int(cuenta)>99){
    cuenta=0.0;
  }
}
ISR(PCINT1_vect){
  _delay_ms(50);
  cuenta-=0.5;
  if(int(cuenta)<0){
    cuenta=99.0;
  }
}
int main (void){
  DDRD|=0xF0;//salidas displays
  DDRB|=0X03;//activar/desactivar
  PORTB&=~0x03;
  config_PCI();
  sei();
  while (1){
  PORTB&=~0x01;
  PORTB|=0X02;
  PORTD|=(int(cuenta)%10<<4);
  PORTD&=(int(cuenta)%10<<4)|0x0F;//unidades
  _delay_ms(8);
  PORTB&=~0x02;
  PORTB|=0X01;
  PORTD|=(int(cuenta)/10<<4);
  PORTD&=(int(cuenta)/10<<4)|0x0F;//decenas
  _delay_ms(8);
  }
}