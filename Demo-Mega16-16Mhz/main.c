
#define Debug_lib

#ifdef Debug_lib
#include <cpu.h>
#include <uart.h>
#else
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include <util/delay.h>
#include <avr/wdt.h>

void board_test(int tick)
{
  char flag;
  DDRC = 0xf0;
  wdt_disable();
  if(tick == 0) flag = 1;
  else flag = 0;
  while(1)
  {
    _delay_ms(500);
    PORTC |= 0xf0;

    _delay_ms(500);
    PORTC &= 0x0f;

    if(flag == 0)
    {
      tick --;
      if(tick == 0) break;
    }
  }
}

void init_port()
{
  DDRC  = 0xf0;
  PORTC = 0x00;

  DDRD  = 0x02;
  PORTD = 0x00;

  DDRA  = 0x00;
  DDRB  = 0x00;
}

void put_char(char c)
{
  while ( !(UCSRA & (1<<UDRE)) ) ;
  UDR = c; 
}

void init_uart()
{
 UCSRB = 0x00;
 UCSRA = 0x00;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x67; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x18;
}

#ifdef Debug_lib
void init_uart2()
{
  uart_init(9600);
}
#endif

void init_device()
{
  init_port();
  #ifdef Debug_lib
  init_uart2();
  #else
  init_uart();
  #endif
}


int main()
{
  init_device();

  board_test(2);

  #ifdef Debug_lib
  uart_puts("Hello,world!\r\n");
  #else
  put_char('a');
  put_char('b');
  put_char('c');
  put_char('d');
  #endif

  while(1);

  return 0;
}
