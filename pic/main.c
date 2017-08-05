#include ".\main.h"
  
#INT_RTCC
void  RTCC_isr(VOID) 
{
}

#define SI PIN_B2  
#define CLK PIN_B3
#define AN_CAMLEFT 0
#define AN_CAMRIGHT 1
#define AN_CAM 0

#define SPEED 300 
unsigned int8 rxdata[2];
#define sample_rate rxdata[1]
#define threshold rxdata[0]

void main()
{
   INT16 i;
   setup_adc_ports (AN0_TO_AN1|VSS_VDD) ;
   setup_adc (setup_adc (ADC_CLOCK_INTERNAL)) ;
   setup_psp (PSP_DISABLED) ;
   setup_spi (SPI_SS_DISABLED) ;
   setup_wdt (WDT_OFF) ;
   setup_timer_0 (RTCC_INTERNAL|RTCC_DIV_8) ;
   setup_timer_1 (T1_DISABLED) ;
   setup_timer_2 (T2_DISABLED, 0, 1) ;
   setup_timer_3 (T3_DISABLED|T3_DIV_BY_1) ;
   setup_ccp1 (CCP_OFF) ;
   setup_comparator (NC_NC_NC_NC) ;
   setup_vref (FALSE) ;
   enable_interrupts (INT_RTCC) ;
   enable_interrupts (GLOBAL) ;
   output_low (SI) ;
   output_low (CLK) ;
   
   usb_init(); //Init the usb device
   usb_task(); //Will call usb_attach().To attach USB Device to the bus. 
   //usb_wait_for_enumeration(); //infinite loop

   if(usb_enumerated()) 
   {
      printf ("USB connected\n");
   }
      
   WHILE (true)
   {
      twoCamOneShot();
      if(usb_enumerated()) 
      {
         if (usb_kbhit(1)) 
         {
            //Take an line image from sensor
            
         
            usb_get_packet(1,rxdata,2);
            printf("--> Received 2 bytes: Thresh=%U Delay=%U\r\n",threshold,sample_rate);
            usb_puts( 1, line, 256, 10 );
            
         }
      }
   
      
      
      //FOR (i = 0; i < 128; ++i)
      //{
         //printf (" %Ld = %u \n", i, line[i]);
      //}
      
      //delay_ms(1000);
   }
}

/**
*  Take a one shot image line from sensor
*/
void oneShot()
{
   int16 i;
   //########################################
   //First step : begin an controlled exposure and erase all line buffer bits
   // We don't read buffer bits because the exposure time before may be to high
   // This step process a new capture with good exposure time
   
   // Start
   output_high (SI) ;
   delay_us (SPEED) ;
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (SI) ;
   delay_us (SPEED) ;
   output_low (CLK) ;
   delay_us (SPEED) ;
   //Bits loops
   FOR (i = 1; i < 128; ++i)
   {
      output_high (CLK) ;
      delay_us (SPEED) ;
      output_low (CLK) ;
      delay_us (SPEED) ;
   }

   
   
   delay_us (128) ;
   
   //########################################
   // Read bits from last exposure
   // Start a new exposure
   output_high (SI) ;
   delay_us (SPEED) ;
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (SI) ;
   delay_us (SPEED) ;
   output_low (CLK) ;
   delay_us (SPEED) ;
   
   //Select the ADC channel to read and read first pixel 
   set_adc_channel (AN_CAM) ;
   line[0] = read_adc ();
   //Read all others pixels
   FOR (i = 1; i < 128; ++i)
   {
      output_high (CLK) ;
      delay_us (SPEED) ;
      line[i] = read_adc ();
      output_low (CLK) ;
      delay_us (SPEED) ;
   }

   //Line is saved into the line buffer
}

void twoCamOneShot()
{
   int16 i;
   //########################################
   //First step : begin an controlled exposure and erase all line buffer bits
   // We don't read buffer bits because the exposure time before may be to high
   // This step process a new capture with good exposure time
   
   // Start
   output_high (SI) ;
   delay_us (SPEED) ;
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (SI) ;
   delay_us (SPEED) ;
   output_low (CLK) ;
   delay_us (SPEED) ;
   //Bits loops
   FOR (i = 1; i < 128; i++)
   {
      output_high (CLK) ;
      delay_us (SPEED) ;
      output_low (CLK) ;
      delay_us (SPEED) ;
   }
   //129th clock
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (CLK) ;
   
   delay_us (128) ;
   
   //########################################
   // Read bits from last exposure
   // Start a new exposure
   output_high (SI) ;
   delay_us (SPEED) ;
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (SI) ;
   delay_us (SPEED) ;
   output_low (CLK) ;
   delay_us (SPEED) ;
   
   //Select the ADC channel to read and read first pixel 
   set_adc_channel (AN_CAMLEFT) ;
   line[0] = read_adc ();
   set_adc_channel (AN_CAMRIGHT) ;
   line[128+0] = read_adc ();
   //Read all others pixels
   FOR (i = 1; i < 128; i++)
   {
      output_high (CLK);
      delay_us (SPEED);
      set_adc_channel (AN_CAMLEFT) ;
      delay_us(10);
      line[i] = read_adc ();
      set_adc_channel (AN_CAMRIGHT) ;
      delay_us(10);
      line[128+i] = read_adc ();
      output_low (CLK);
      delay_us (SPEED);
   }

   //129th clock
   output_high (CLK) ;
   delay_us (SPEED) ;
   output_low (CLK);
   //Line is saved into the line buffer
}
