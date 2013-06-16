

// Date: 2011-10-23 15:07:12
// Target : M8
// Crystal: 12.000Mhz

#include <cpu.h>
#include <uart.h>


void port_init(void)
{
 PORTB = 0xFF;
 DDRB  = 0x00;

 PORTC = 0x00; //m103 output only
 DDRC  = 0x00;

 PORTD = 0x00;
 DDRD  = 0x3A;
}

void init_uart(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x4D; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x18;
}

void init_uart2()
{
 uart_init(9600);
}

void init_devices(void)
{
 CLI(); //disable all interrupts
 port_init();
 //init_uart();
 init_uart2();

 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 SEI(); //re-enable interrupts
}

void put_char(char c)
{
  while ( !(UCSRA & (1<<UDRE)) ) ;
  UDR = c;
}



int main(void)
{
  unsigned char c;
  init_devices();
  
  PORTD = 0x00;
  uart_puts("/*****************************/\r\n");

  c='a';
  while(1)
  {
    c = uart_getc();
    uart_putc(c);
  }


  while(1) PORTD = 0xff;

  return 0;
}




