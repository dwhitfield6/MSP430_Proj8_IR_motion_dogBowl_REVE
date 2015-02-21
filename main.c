#include "msp430.h"


#define LED0 BIT0
#define IRLED BIT6
#define IRrec BIT5
#define switch1 BIT3
#define IN1 BIT3
#define IN2 BIT4

int distance =16;
int motortimeopen =1400;
int motortimeclose =1200;

void ir_sense(int distance);

int closetimecounter=0;
int opentimecounter=0;
int IRreceiveCount =0;
int IRreceiveCountMAX =0;
int loopcount =0;
int object =0;
int objectcount =10;
int objectTimer =0;
int NobjectTimer =0;
int upcount =0;
int upcountloop =0;
int closetimer =20;
int opentimer =0;


void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer
  P1DIR |= (LED0 + IRLED);
  P2DIR |= (IN1 + IN2);								// Set all pins but RXD to output
  P1REN |= (switch1);												//enable pullup/pulldown resistor
  P1OUT |= (switch1);
	P2OUT |= (IN1);
	P2OUT |= (IN2);

  P2IE |= IRrec; // P1.3 interrupt enabled
  P2IES &= ~IRrec; // P1.3 interrupt enabled
  P2IFG &= ~IRrec; // P1.3 IFG cleared


  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts


  while(1)
  {

	  _delay_cycles(1000);

    if(loopcount == 10)
    {
    ir_sense(distance);
    if(upcountloop >4)
    {
    	objectcount++;
    }
    else
    {
    	objectcount =0;
    }
    }

if(objectcount > 1)
{
	objectTimer=0;
}
else
{
	NobjectTimer =0;
}

if(NobjectTimer <300)
{
	closetimer=1;
}
else
{
	closetimer=0;
}

if(objectTimer <300)
{
	P1OUT |= LED0;
	opentimer=1;
}
else
{
	P1OUT &= ~LED0;
	opentimer=0;
}
if(opentimer ==1)
{
	opentimecounter++;
	if(opentimecounter == 10)
	{
		P2OUT |= (IN1);
		P2OUT &= ~(IN2);
	}
	else if(opentimecounter ==  motortimeopen)
	{
		P2OUT |= (IN1);
		P2OUT |= (IN2);
	}
	else
	{
		closetimecounter =0;
	}
}
if(closetimer ==1)
{
	closetimecounter++;
	if(closetimecounter == 400)
	{
		P2OUT |= (IN2);
		P2OUT &= ~(IN1);
	}
	else if(closetimecounter ==  motortimeclose)
	{
		P2OUT |= (IN1);
		P2OUT |= (IN2);

	}
	else
	{
		opentimecounter=0;
	}
}
NobjectTimer++;
objectTimer++;
if(opentimecounter>4000)
{
	opentimecounter=4000;
}
if(closetimecounter>4000)
{
	closetimecounter=4000;
}
if(NobjectTimer >1000)
{
	NobjectTimer =1000;
}
if(objectTimer >1000)
{
	objectTimer =1000;
	NobjectTimer =0;
}


    loopcount++;
    if(loopcount>40)
    {
    	loopcount =0;
    }

}
}

void ir_sense(int distance)
{

	int i,ii,iii;
	upcount =0;
	P2IE |= IRrec; // P1.3 interrupt enabled
	for(i=20;i>0;i--)
	{
		for(ii =40;ii>0;ii--)
		{
P1OUT |= IRLED;
_delay_cycles(13);
P1OUT &= ~IRLED;

		}
		for(iii=distance;iii>0;iii--)
		{
			_delay_cycles(1);
		}

	}

upcountloop = upcount;
P2IE &= ~IRrec; // P1.3 interrupt enabled
}

// Port 1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	upcount++;
	P2IFG &= ~IRrec; // P1.3 IFG cleared
}


