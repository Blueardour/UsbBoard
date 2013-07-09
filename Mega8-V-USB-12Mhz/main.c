

#include <cpu.h>

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <usbdrv/usbdrv.h>

#define USB_LED_OFF 0
#define USB_LED_ON  1
#define USB_LED_TRIG 5

USB_PUBLIC uchar usbFunctionSetup(uchar data[8])
{
  usbRequest_t *rq = (void *)data; // cast data to correct type
   
  switch(rq->bRequest) { // custom command is in the bRequest field
    case USB_LED_ON:
        PORTC |= 1; // turn LED on
        return 0;
    case USB_LED_OFF: 
        PORTC &= ~1; // turn LED off
        return 0;
    case USB_LED_TRIG:
	PORTC = PINC ^ 0x01;
	return 0;
  }

  return 0; // do nothing for now
}

int main()
{
  uchar i;

  DDRC = 1;
  
  wdt_enable(WDTO_1S); // enable 1s watchdog timer
  
  usbInit();
          
  usbDeviceDisconnect(); // enforce re-enumeration
  for(i = 0; i<250; i++) { // wait 500 ms
    wdt_reset(); // keep the watchdog happy
    _delay_ms(2);
  }
  usbDeviceConnect();
          
  sei(); // Enable interrupts after re-enumeration
          
  while(1) {
    wdt_reset(); // keep the watchdog happy
    usbPoll();
  }
          
  return 0;
}
