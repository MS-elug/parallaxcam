#include <18F4550.h>
#device adc=8
#use delay(clock=48000000)
#use rs232(baud=9600, xmit=PIN_C6,rcv=PIN_C7,parity=N,bits=8)

#define USB_HID_DEVICE     FALSE
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK  //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK  //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    64  //size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    64   //size to allocate for the rx endpoint 1 buffer

#include <pic18_usb.h>
#include <usb_desc.h>   //USB Configuration and Device descriptors for this UBS device
#include <usb.c>           //handles usb setup tokens and get descriptor reports




#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES HSPLL                    //High Speed Crystal/Resonator with PLL enabled
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV20                   //Brownout reset at 2.0V
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES PBADEN                   //PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES MCLR                     //Master Clear pin enabled
#FUSES LPT1OSC                  //Timer1 configured for low-power operation
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL5                    //Divide By 5(20MHz oscillator input)
#FUSES CPUDIV1                  //System Clock 96Mhz by 2
#FUSES USBDIV                   //USB clock source comes from PLL divide by 2

//#FUSES PLL12                    //Divide By 12(48MHz oscillator input)
//#FUSES CPUDIV4                  //System Clock by 4
//#FUSES USBDIV                   //USB clock source comes from PLL divide by 2
#FUSES VREGEN                   //USB voltage regulator enabled
#FUSES ICPRT                    //ICPRT enabled




unsigned int8 line[256];

void oneShot();
void twoCamOneShot();
