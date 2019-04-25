#include <16F886.h>
#device ADC = 10 *= 16

#FUSES NOWDT     //No Watch Dog Timer
#FUSES PUT       //Power Up Timer
#FUSES NOMCLR    //Master Clear pin not enabled
#FUSES NOPROTECT //Code not protected from reading
#FUSES NOCPD     //No EE protection
#FUSES BROWNOUT  //Brownout reset
#FUSES IESO      //Internal External Switch Over mode enabled
#FUSES FCMEN     //Fail-safe clock monitor enabled
#FUSES NOLVP     //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG   //No Debug mode for ICD
#FUSES NOWRT     //Program memory not write protected
#FUSES BORV40    //Brownout reset at 4.0V
#FUSES RESERVED  //Used to set the reserved FUSE bits
#FUSES INTRC_IO

#use delay(clock = 8M)

#use rs232(baud = 9600, parity = N, xmit = PIN_C6, rcv = PIN_C7, bits = 8)

#define RUN_BUTTON PIN_B7

int16 counter = 0;
int duty = 10;
int duty2 = 10, duty3 = 10, duty4 = 10, duty5 = 10, duty6 = 10;
int check = 0;

#INT_RB
void rb_isr(void)
{
      //!      duty++;
      //!      printf("%lu\r\n", duty);
      //!      if (duty == 21) duty = 10;
}

#INT_TIMER1
void timer1_isr()
{
      set_timer1(65511);
      counter++;
      if (counter == 200)
      {
            counter = 0;
      }
      If(counter == 0)
      {
            output_high(PIN_B5);
            output_high(PIN_B6);
      } // beginning of a new period
      If(counter == duty)
      {

            output_low(PIN_B6);
      }
      If(counter == duty2)
      {
            output_low(PIN_B5);
      }
}

int button = 0;
int button1 = 0;
int button2 = 0;
int button3 = 0;
int button4 = 0;
int autoPilotButton = 0;
int autoPilot = 0;
int16 adc_Value = 0;

void main()
{
      setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4); // setup PIN A0 as analog input
      setup_adc(ADC_CLOCK_INTERNAL);
      setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
      set_timer1(65511);
      enable_interrupts(INT_TIMER1);
      enable_interrupts(GLOBAL);

      counter = 0;
      while (1)
      {
            for (int i = 0; i < 5; i++)
            {
                  set_adc_channel(i);
                  if (i == 0)
                        printf("Sensor %d value = %Lu\r\n", i, read_adc());
            }

            if (autoPilot == 1)
            {
                  set_adc_channel(0);
                  int16 val1 = read_adc();
                  set_adc_channel(1);
                  int16 val2 = read_adc();
                  set_adc_channel(2);
                  int16 val3 = read_adc();
                  set_adc_channel(3);
                  int16 val4 = read_adc();
                  if (val1 > 10)
                        duty = 20;
                  else if (val3 > 10)
                        duty2 = 20;
                  else if (val3 > 10)
                        duty2 = 20;
                  else
                  {
                        duty = 10;
                        duty2 = 10;
                  }
            }

            if (autoPilotButton == 0)
            {
                  if (!input(PIN_B7))
                  {

                        autoPilotButton = 1;
                        if (autoPilot == 1)
                              autoPilot = 0;
                        else
                              autoPilot = 1;
                  }
            }
            else if (input(PIN_B7))
            {
                  autoPilotButton = 0;
            }

            if (button1 == 0)
            {
                  if (!input(PIN_B1))
                  {

                        button1 = 1;
                        if (duty < 21)
                              duty++;
                  }
            }
            else if (input(PIN_B1))
                  button1 = 0;

            if (button2 == 0)
            {
                  if (!input(PIN_B2))
                  {

                        button2 = 1;
                        if (duty > 10)
                              duty--;
                  }
            }
            else if (input(PIN_B2))
                  button2 = 0;

            if (button3 == 0)
            {
                  if (!input(PIN_B3))
                  {

                        button3 = 1;
                        if (duty2 < 21)
                              duty2++;
                  }
            }
            else if (input(PIN_B3))
                  button3 = 0;

            if (button4 == 0)
            {
                  if (!input(PIN_B4))
                  {

                        button4 = 1;
                        if (duty2 > 10)
                              duty2--;
                  }
            }
            else if (input(PIN_B4))
                  button4 = 0;
      }
}