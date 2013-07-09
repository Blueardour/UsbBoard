
#include <avr/wdt.h>

#include "cpu.h"

#include <util/delay.h>

int main()
{
  wdt_enable(WDTO_1S);

  DDRA = 0x00;
  DDRB = 0x00;
  DDRC = 0x00;
  DDRD = 0x00;

  PORTA = 0xff;
  PORTB = 0xff;
  PORTC = 0xff;
  PORTD = 0xff;
  
  while(1)
  {
    wdt_reset();
    _delay_ms(200);
  }
}
