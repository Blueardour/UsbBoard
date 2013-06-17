

#include <cpu.h>

#include <uart.h>
#include <spi.h>
#include <sd-mmc.h>



void init_port(void)
{
  DDRC  = 0xf0;
  PORTC = 0x00;

  DDRD  = 0x02;
  PORTD = 0x00;

  DDRA  = 0x00;
  PORTA = 0x00;

  DDRB  = 0x00;
  PORTB = 0xff;
}

void init_uart(void)
{
  uart_init(9600);
}

void init_spi(void)
{
  spi_init(1);
}

void init_devices(void)
{
  cli();
  init_port();
  init_uart();
  init_spi();

  MCUCR = 0x00;
  GICR  = 0x00;
  TIMSK = 0x00;
  sei();
}

void SD_test()
{
  int i;
  u08 buffer[8];
  u08 c;

  uart_puts("\r\n**** SD Test ******\r\n");

  // pull up the cs
  spi_cs_on();

  // SPI SCK: F_CPU/128, MSB
  if((c=spi_master_setup(128,0)) != 0)
  {
    uart_puts("Init SPI Failed:0x");
    uart_puth(c);
    uart_puts("\r\n");
    return;
  }
  else
    uart_puts("Init SPI OK!\r\n");

  //DDRB = 0xff;
  //PORTB = 0x00;
  //while(1);

  // send 74+ cycles
  uart_puts("Send 74+ cycles...");
  spi_cs_on();
  for(i=0;i<15;i++) 
  {
    spi_master_SendByte(0x00);
  }
  uart_puts("OK\r\n");

  //DDRB = 0xff;
  //PORTB = 0x00;
  //while(1);

  // reset and try to enter the SPI mode
  uart_puts("Send reset command...");
  //SD_SendCommand(0x40,0x00000000,0x95,buffer,8);
  spi_cs_off();
  spi_master_SendByte(0x40);
  spi_master_SendByte(0x00);
  spi_master_SendByte(0x00);
  spi_master_SendByte(0x00);
  spi_master_SendByte(0x00);
  spi_master_SendByte(0x95);

  // wait for response
  while(spi_master_SendByte(0xff) == 0xff);
  spi_cs_on();
  uart_puts("OK\r\n");

  for(i=0;i<8;i++)
  {
    uart_puth(buffer[i]);
    uart_putc(' ');
  }
  uart_puts("\r\n");
 
  SD_SendCommand(0x41,0x00000000,0xff,buffer,8);
  for(i=0;i<8;i++)
  {
    uart_puth(buffer[i]);
    uart_putc(' ');
  }
  uart_puts("\r\n");
  
  SD_SendCommand(0x50,0x00000000,0xff,buffer,8);
  for(i=0;i<8;i++)
  {
    uart_puth(buffer[i]);
    uart_putc(' ');
  }
  uart_puts("\r\n");

  while(1)
  {
    c = uart_getc();
    uart_putc(c);
    uart_puts(":\r\n ");
    switch(c)
    {      
      case '1':SD_SendCommand(0x40,0x00000000,0x95,buffer,8);
               for(i=0;i<8;i++)
               {
                 uart_puth(buffer[i]); uart_putc(' ');
               }
               uart_puts("\r\n");
  	       break;
      case '2':SD_SendCommand(0x41,0x00000000,0xff,buffer,8);
    	       for(i=0;i<8;i++)
               {
                 uart_puth(buffer[i]); uart_putc(' ');
               }
               uart_puts("\r\n");
    	       break;
      case '3':SD_SendCommand(0x50,0x00000000,0xff,buffer,8);
	       for(i=0;i<8;i++)
               {
                 uart_puth(buffer[i]); uart_putc(' ');
               }
               uart_puts("\r\n");
	       break;
      case 'q':
      case 'Q': return;
    }
  }
}

void spi_test()
{
  PORTB = 0xff;
  DDRB  = 0xB0;

  SPCR  = 0x7f;
  SPSR  = 0x00;

  PORTB &= 0xBF;  
  
  uart_puts("In spi_test....\r\n");  

  SPDR = 0x55;
  while(!(SPSR & (1<<SPIF)));
  uart_puts("Send one Byte\r\n");

  SPDR = 0xaa;
  while(!(SPSR & (1<<SPIF)));
  uart_puts("Send one Byte\r\n");

  SPDR = 0xff;
  while(!(SPSR & (1<<SPIF)));
  uart_puts("Send one Byte\r\n");

}

void port_test(int bit)
{
  DDRB = 0xbf;

  if(bit == 0)
    PORTB = 0x00;
  else
    PORTB = 0xff;

  while(1);
}

int main()
{
  init_devices();

  //port_test(0);

  SD_test();

  while(1);
  return 0;
}



