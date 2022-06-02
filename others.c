#include <MKL25Z4.H>


void Servo_init (void);
void ADC0_init(void);
void DCMOTOR_init (void);


void Servo_init (void){
		SIM -> SCGC5 |= 0x2000;  /*enable clock port E */
		PORTE -> PCR[31] = 0x0300; /* PTE31 used by TPM0*/
		SIM -> SCGC6 |= 0x01000000; /*enable clock to TPM0 */
		SIM -> SOPT2 |= 0x01000000; /*enable clock to TPM0 */
		TPM0 -> SC = 0; /* disable timer */
		TPM0 -> CONTROLS[4].CnSC = 0x20 | 0x08;  /* edge aligned, pulse high clear output at match*/
		TPM0 -> MOD = 6562; /* set up modulo with frequency1*/
		TPM0 -> CONTROLS[4].CnV = 492;  /* set up channel value for duty1% duty cycle*/
		TPM0 -> SC = 0x0E; /* enable TPM0 with prescaler 2^6 and cmod = 1*/
}
void ADC0_init(void){
	PORTE->PCR[20] = 0; /* PTE20 analog input */
	PORTE->PCR[21] = 0; /* PTE21 analog input */
	PORTE->PCR[22] = 0; /* PTE30 analog input */
	SIM->SCGC6 |= 0x8000000; /* clock to ADC0*/
	ADC0->SC2 &= 0xbf; /* software trigger */
	/* clock div by 4, long sample time, single ended 10 bit, bus clock */
	ADC0->CFG1 = 0x40 | 0x10 | 0x08;
}
void DCMOTOR_init (void){
	PORTE->PCR[5] = 0x100;      /* make PTE5 pin as GPIO */
	PORTE->PCR[4] = 0x100;      /* make PTE5 pin as GPIO */
	PTE->PDDR |= 0x30;           /* make PTE5 and PTE4 as output pin */
}