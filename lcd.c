#include <MKL25Z4.H>
#include <stdio.h>

#define RS 0x04     /* PTA2 mask */ 
#define RW 0x1000     /* PTA12 mask */ 
#define EN 0x20     /* PTA5 mask */

void Delay(volatile unsigned int time_del);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_ready(void);
void custom_Generator(void);
void watering(int i);
void Symbols(void);

void LCD_init(void)
{
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */ 
    PORTD->PCR[0] = 0x100;      /* make PTD0 pin as GPIO */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PORTD->PCR[2] = 0x100;      /* make PTD2 pin as GPIO */
    PORTD->PCR[3] = 0x100;      /* make PTD3 pin as GPIO */
    PORTD->PCR[4] = 0x100;      /* make PTD4 pin as GPIO */
    PORTD->PCR[5] = 0x100;      /* make PTD5 pin as GPIO */
    PORTD->PCR[6] = 0x100;      /* make PTD6 pin as GPIO */
    PORTD->PCR[7] = 0x100;      /* make PTD7 pin as GPIO */
    PTD->PDDR = 0xFF;           /* make PTD7-0 as output pins */
    
    SIM->SCGC5 |= 0x0200;       /* enable clock to Port A */ 
    PORTA->PCR[2] = 0x100;      /* make PTA2 pin as GPIO */
    PORTA->PCR[12] = 0x100;      /* make PTA12 pin as GPIO */
    PORTA->PCR[5] = 0x100;      /* make PTA5 pin as GPIO */
    PTA->PDDR |= 0x1024;          /* make PTA5, 12, 2 as output pins */
    
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* This function waits until LCD controller is ready to
 * accept a new command/data before returns.
 */
void LCD_ready(void)
{
    uint32_t status;
    
    PTD->PDDR = 0x00;          /* PortD input */
    PTA->PCOR = RS;         /* RS = 0 for status */
    PTA->PSOR = RW;         /* R/W = 1, LCD output */
    
    do {    /* stay in the loop until it is not busy */
			  PTA->PCOR = EN;
			  Delay(500);
        PTA->PSOR = EN;     /* raise E */
        Delay(500);
        status = PTD->PDIR; /* read status register */
        PTA->PCOR = EN;
        Delay(500);			/* clear E */
    } while (status & 0x80UL);    /* check busy bit */
    
    PTA->PCOR = RW;         /* R/W = 0, LCD input */
    PTD->PDDR = 0xFF;       /* PortD output */
}

void LCD_command(unsigned char command)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PCOR = RS | RW;    /* RS = 0, R/W = 0 */
    PTD->PDOR = command;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

void LCD_data(unsigned char data)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PSOR = RS;         /* RS = 1, R/W = 0 */
    PTA->PCOR = RW;
    PTD->PDOR = data;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */

/* delay n microseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */


void Delay(volatile unsigned int time_del) {
  while (time_del--) 
		{
  }
}


void custom_Generator(void){
char a[8]={0b11011,0b01110,0b01110,0b00100,0b00100,0b11111,0b01110,0b01110};
char b[8]={0b01110,0b11111,0b01110,0b00100,0b00100,0b11111,0b01110,0b01110};
int i = 0;
LCD_command(0x40+8);
while(i!=8){                //Placing 'b' By sending 'b' pattern to cg-ram
			LCD_data(b[i]);
			i++;
		}
		i = 0;
		LCD_command(0x40+16);
while(i!=8){                //Placing 'b' By sending 'b' pattern to cg-ram
		LCD_data(a[i]);
		i++;
}
}

void watering (int i){	
if (i == 1){
	LCD_command(0x87);
	LCD_data('W');
	LCD_data('A');
	LCD_data('T');
	LCD_data('E');
	LCD_data('R');
	LCD_data('I');
	LCD_data('N');
	LCD_data('G');
	LCD_command(0xC7);
	LCD_data(' ');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data(' ');
	LCD_command(0x8F);
}
if (i==0){
	LCD_command(0x87);
	LCD_data('W');
	LCD_data('A');
	LCD_data('I');
	LCD_data('T');
	LCD_data('I');
	LCD_data('N');
	LCD_data('G');
	LCD_data(' ');
	LCD_command(0x8E);
}
if (i == 2){
LCD_command(0x01);
LCD_command(0x80);
	LCD_data('W');
	LCD_data('A');
	LCD_data('T');
	LCD_data('E');
	LCD_data('R');
	LCD_data(' ');
	LCD_data('L');
	LCD_data('E');
	LCD_data('V');
	LCD_data('E');
	LCD_data('L');
	LCD_command(0xC0);
	LCD_data('C');
	LCD_data('R');
	LCD_data('I');
	LCD_data('T');
	LCD_data('I');
	LCD_data('C');
}
if (i == 3){
	LCD_command(0x87);
	LCD_data('N');
	LCD_data('E');
	LCD_data('E');
	LCD_data('D');
	LCD_command(0xC7);
	LCD_data('W');
	LCD_data('A');
	LCD_data('T');
	LCD_data('E');
	LCD_data('R');
}
if (i == 4){
	LCD_command(0x01);
	LCD_command(0x80);
	LCD_data('I');
	LCD_data('N');
	LCD_data('I');
	LCD_data('T');
	LCD_data('I');
	LCD_data('A');
	LCD_data('L');
	LCD_data('I');
	LCD_data('Z');
	LCD_data('I');
	LCD_data('N');
	LCD_data('G');
	LCD_data('.');
	LCD_data('.');
	LCD_data('.');
}
if (i==5){
	LCD_command(0x01);
	LCD_command(0x80);
	LCD_data(' ');
	LCD_data(' ');	
	LCD_data('S');
	LCD_data('E');
	LCD_data('L');
	LCD_data('E');
	LCD_data('C');
	LCD_data('T');
	LCD_data(' ');
	LCD_data('M');
	LCD_data('O');
	LCD_data('D');
	LCD_data('E');
	LCD_command(0xC4);
	LCD_data('1');
	LCD_command(0xCA);
	LCD_data('2');
}
if (i==6){
	LCD_command(0x01);
	LCD_command(0x80);
	LCD_data('M');
	LCD_data('O');
	LCD_data('D');
	LCD_data('E');
	LCD_data('1');
}
if (i==7){
	LCD_command(0x01);
	LCD_command(0x80);
	LCD_data('M');
	LCD_data('O');
	LCD_data('D');
	LCD_data('E');
	LCD_data('2');
}
}
void Symbols(void){
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_data(0x01);
			LCD_data(':');
			LCD_command(0xC0);
			LCD_data(0x02);
			LCD_data(':');




}
