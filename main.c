#include "MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5 (pin 98)
// LED_RED = PTE29 (pin 26)

// SWICHES
// LEFT (SW1) = PTC3 (pin 73)
// RIGHT (SW2) = PTC12 (pin 88)

void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}

// LEFT_SWITCH (SW1) = PTC3
void sw1_ini()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1);
  GPIOC->PDDR &= ~(1 << 3);
}

//********************************************
void sw1_ini_irq(){

 SIM->COPC = 0;
 SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
 PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1);
 PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
 
 // IRQ#31: pin detected for ports c y d
 NVIC_SetPriority(31,1);
 NVIC_GetPriority(31);
 }
 

 
 // hay que poner el ISF a 1 


//*******************************************

// RIGHT_SWITCH (SW2) = PTC12
void sw2_ini()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1);
  GPIOC->PDDR &= ~(1 << 12);
}

int sw1_check()
{
  return( !(GPIOC->PDIR & (1 << 3)) );
}

int sw2_check()
{
  return( !(GPIOC->PDIR & (1 << 12)) );
}

// LED_GREEN = PTD5
void led_green_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5);
  GPIOD->PSOR = (1 << 5);
}

void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

// LED_RED = PTE29
void led_red_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}



void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

// LED_RED = PTE29
// LED_GREEN = PTD5
void leds_init(void)
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5);
  GPIOE->PDDR |= (1 << 29);
  // both LEDS off after init
  GPIOD->PSOR = (1 << 5);
  GPIOE->PSOR = (1 << 29);
}

 //cuando pulsamos el botón viene aquí
 void PORTDIntHandler(void){
 	//NVIC_ClearPendingIRQ(31);
 	led_green_toggle();
 	led_red_toggle();
 	delay();
 	PORTD->ISFR = (1 << 31);
 }

int main(void)
{
  leds_init();
  sw1_ini();

  led_green_toggle();
  while (1) {
    if (sw1_check()) {
      led_green_toggle();
      led_red_toggle();
      while (sw1_check()); // Improvement by Miguel Blanco Godón
    }
  }

  return 0;
}
