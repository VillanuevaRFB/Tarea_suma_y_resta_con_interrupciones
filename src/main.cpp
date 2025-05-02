#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double cuenta=0;
int guardado=0;
char operacion=0;

void config_PCI(void) {
  PCICR|=0x01;
  PCICR|=0x02;
  PCMSK0|=0x08;
  DDRB&=~0x08;
  PORTB|=0x08;//+1
  PCMSK1|=0x01;
  DDRC&=~0x01;
  PORTC|=0x01;//-1
  PCMSK0|=0x04;
  DDRB&=~0x04;
  PORTB|=0x04;//suma
  PCMSK1|=0x02;
  DDRC&=~0x02;
  PORTC|=0x02;//resta
  PCMSK1|=0x04;
  DDRC&=~0x04;
  PORTC|=0x04;//enter
}

ISR(PCINT0_vect){
  _delay_ms(50);
  if(!(PINB&0x08)){
    cuenta+=0.5;
    if((int)cuenta>99)
    cuenta=0;
  }
  else if(!(PINB&0x04)){
    guardado=(int)cuenta;
    cuenta=0;
    operacion=1;
  }
}

ISR(PCINT1_vect){
  _delay_ms(50);
  if(!(PINC&0x01)){
    cuenta-=0.5;
    if((int)cuenta<0){
    cuenta=99;
    }
  }
  else if(!(PINC&0x02)){
    guardado=(int)cuenta;
    cuenta=0;
    operacion=2;
  }
  else if(!(PINC&0x04)){
    if(operacion==1){
    cuenta=guardado+(int)cuenta;
    }
    else if(operacion==2){
    cuenta=guardado-(int)cuenta;
    }
    else cuenta=0;
    if((int)cuenta>99){
    cuenta=99;
    }
    if((int)cuenta<0){
    cuenta=0;
    }
    operacion=0;
  }
}

int main(void){
  DDRD|=0xF0;//salidas display
  DDRB|=0x03;//activar/desactivar
  PORTB&=~0x03;
  config_PCI();
  sei();
  while (1) {
    PORTB&=~0x01;
    PORTB|=0x02;
    PORTD|=((int)cuenta%10)<<4;
    PORTD&=(((int)cuenta%10)<<4)|0x0F;//unidades
    _delay_ms(8);
    PORTB&=~0x02;
    PORTB|=0x01;
    PORTD|=((int)cuenta/10)<<4;
    PORTD&=(((int)cuenta/10)<<4)|0x0F;//decenas
    _delay_ms(8);
  }
}