#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "lcd.h"
#include "others.h"
volatile unsigned int flag = 0;
void Button_init(void);
void PORTA_IRQHandler(void);
void PIT_init(void);
void PIT_IRQHandler(void);
void Systick_init(void);
void SysTick_Handler(void);
void SysTick_Delay(volatile unsigned int count);
volatile static int systick_count;
void PIT_delay( int counter);
volatile unsigned int PIT_count =0; 
int main (void) {
		//Initializations
		volatile unsigned int plant1;
		volatile unsigned int plant2;
		volatile unsigned int deep;	
	  volatile unsigned int mode = 0;
		int resultdata;
		char asciresult1[16];
		char asciresult2[16];
		LCD_init();
		custom_Generator();
		Button_init();
		Servo_init();
		ADC0_init();
		Systick_init();
		DCMOTOR_init();
		
//		Symbols();
		watering(4);
		SysTick_Delay(3);
		watering (5);
		while (flag == 0){}
			if (flag == 1){
				mode = 1;		
				watering (6);
				SysTick_Delay(5);
			}
			else if (flag == 2){
				mode = 2;
				PIT_init();
				watering (7);
				SysTick_Delay(5);
				}
//		LOOP
		while (1){

			// Water Level
			ADC0->SC1[0] = 3; /* start conversion on channel 0 */
			while(!(ADC0->SC1[0] == 0x83)){}
			LCD_command(0xC7);
			deep = ADC0->R[0]; /* read conversion result and clear COCO flag .COCO is cleared automatically by reading Rn*/
			deep = ((deep*100)/1023);
			if (deep <= 20){
				watering(2);
				SysTick_Delay(1);
		//		Symbols();
			}
			if (mode == 1){
			Symbols(); //lcd first writings
			//Plant 1
			LCD_command(0x82);
			ADC0->SC1[0] = 0; /* start conversion on channel 0 */
			while(!(ADC0->SC1[0] == 0x80)){}
			plant1 = ADC0->R[0]; /* read conversion result and clear COCO flag .COCO is cleared automatically by reading Rn*/
			plant1 = (100 - (plant1*100)/1023);
			resultdata = sprintf(asciresult1, "%d", plant1);				
			for(int t = 0; t < resultdata; ++t){
					LCD_data(asciresult1[t]);
					Delay(300000);
			}
			LCD_data('%');	
			if (plant1 <= 30){
					TPM0 -> CONTROLS[4].CnV = 328;  /* set up channel value for duty1% duty cycle*/
					SysTick_Delay(5);
					PTE -> PSOR = 0x20;
					PTE -> PCOR = 0x10;
					watering(1);
					SysTick_Delay(3);
					PTE -> PCOR = 0x20;
					watering(0);		
			}
			
			//Plant 2
			LCD_command(0xC2);
			ADC0->SC1[0] = 4;	/* start conversion on channel 3 */
			while(!(ADC0->SC1[0] == 0x84)){}
			plant2 = ADC0-> R[0]; /* read conversion result and clear COCO flag .COCO is cleared automatically by reading Rn*/
			plant2 = (100 - (plant2*100)/1024);
			resultdata = sprintf(asciresult2, "%d", plant2);				
			for(int t = 0; t < resultdata; ++t){
				LCD_data(asciresult2[t]);
				Delay(300000);
			}
			LCD_data('%');	
			if (plant2 <= 20){
				TPM0 -> CONTROLS[4].CnV = 656;  /* set up channel value for duty1% duty cycle*/
				SysTick_Delay(5);
				PTE -> PSOR = 0x20;
				PTE -> PCOR = 0x10;
				watering(1);
				SysTick_Delay(5);
				PTE -> PCOR = 0x20;
				watering(0);
			}
			SysTick_Delay(4);
			}
				if (mode == 2){
						LCD_command(0x01);
						if (PIT_count == 1){
						TPM0 -> CONTROLS[4].CnV = 656;  /* set up channel value for duty1% duty cycle*/
						SysTick_Delay(5);
						PTE -> PSOR = 0x20;
						PTE -> PCOR = 0x10;
						watering(1);
						SysTick_Delay(5);
						PTE -> PCOR = 0x20;
						watering(0);  
						SysTick_Delay(3);
						PIT_count  = 0;
						}
						if (PIT_count == 2){
						TPM0 -> CONTROLS[4].CnV = 328;  /* set up channel value for duty1% duty cycle*/
						SysTick_Delay(5);
						PTE -> PSOR = 0x20;
						PTE -> PCOR = 0x10;
						watering(1);
						SysTick_Delay(3);
						PTE -> PCOR = 0x20;
						watering(0);
						SysTick_Delay(3);
						PIT_count  = 0;
						}	
				}
    
}
}

void Button_init(void){
	SIM->SCGC5 |= 0x200; /* enable clock to Port A */

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTA->PCR[1] &= ~0xF0000;//clear initial values
	PORTA->PCR[13] &= ~0xF0000;//clear initial values
	PORTA -> PCR[1] |= 0x3 | 0x100 | 0xA0000;
	PORTA -> PCR[13] |= 0x3 | 0x100 | 0xA0000;
	/* Set port A switch bit to inputs */
	PTA ->PDDR &= ~0x20002;

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 3); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
	
}
void PORTA_IRQHandler(void){
		if (PORTA -> ISFR == 0x02){
		flag = 1;	}
		else{
		flag = 2;
		}
	PORTA -> ISFR = 0xffffffff;  /* clear interrupt flag */
	}

	void PIT_init(void){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable module
	PIT -> MCR = 0; //Enable mdis
	//PIT -> MCR |= PIT_MCR_FRZ_MASK; //for debug mode
	
	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[0].LDVAL = 20971520; //every freq 1 
	//No chaining of timers
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
//	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	//Let the PIT channel generate interrupt requests
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT -> CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[1].LDVAL = 419430040; //every freq 2
	//No chaining of timers
	PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	//Let the PIT channel generate interrupt requests
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	NVIC_SetPriority(PIT_IRQn, 4); 
	//Clear any pending IRQ from PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	//Enable the PIT interrupt in the NVIC
	NVIC_EnableIRQ(PIT_IRQn);	

}
	
	void PIT_IRQHandler(void){
	
//Determine which channel triggered interrupt
	if (PIT->CHANNEL[0].TFLG == PIT_TFLG_TIF_MASK) {	
			PIT_count = 1;		
			PIT->CHANNEL[0].LDVAL = 20971520;   
		//Clear interrupt request flag for channel
			PIT->CHANNEL[0].TFLG = 1;
	}
		
	if (PIT->CHANNEL[1].TFLG == PIT_TFLG_TIF_MASK) {		
			PIT_count = 2;
			PIT->CHANNEL[1].LDVAL = 419430040; 
		
			//Clear interrupt request flag for channel
			PIT->CHANNEL[1].TFLG = 1;
	} 
}
	

//Sytick Configuration
void Systick_init(void){
SysTick->LOAD = 0xFFFFFE;
NVIC_SetPriority(SysTick_IRQn, 3);
SysTick -> VAL = 0;
}
//SysTick ISR
void SysTick_Handler(void){
	systick_count++;
}

void SysTick_Delay(volatile unsigned int count){
SysTick->CTRL = 0x7;
systick_count = 0;
while ( systick_count != count){}	
systick_count = 0;
SysTick->CTRL = 0; // Disable SysTick

}


void PIT_delay(int counter){

PIT -> CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
while ( PIT_count != counter){}	
PIT_count = 0;		
PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}
