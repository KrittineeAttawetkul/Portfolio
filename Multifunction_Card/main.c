#include "MK64F12.h"

void party(void);

void enable_clk(void);

void LED_init(void);

void Switch_init(void);

void Buz_init(void);
void tone(int n);

void InterruptA_init(void);
void InterruptB_init(void);

void word_init(void);
void word(unsigned data,unsigned pos);

void delayMs(int n);

void love(void);
void Mery_Xmas(void);
void Halo(void);
void HNY(void);
void Y23(void);
void Y24(void);
void ms1(void);
void ms2(void);

int main (void){
	
	enable_clk();
	LED_init();
	Switch_init();
	InterruptB_init();
	word_init();

	while(1){
		Halo();
	}
}
	
//-----------------------------FUNCTION--------------------------------------

void PORTB_IRQHandler(void) {

	if(PTB->PDIR & 0x08){
		PTD->PSOR = 0x08;
		word(0xFF,0xF0);
		PORTB->ISFR = 0x08; 
	}
	else{
		Mery_Xmas();
		Y23();
		ms1();

	}
	
	if(PTB->PDIR & 0x400){
		PTD->PSOR = 0x04;
		word(0xFF,0xF0);
		PORTB->ISFR = 0x400;
	}
	else{
		HNY();
		Y24();
		ms2();
	}
	
	if(PTB->PDIR & 0x800){
		PTD->PSOR = 0x01;
		word(0xFF,0xF0);
		PORTB->ISFR = 0x800;
	}
	else{
		love();
		Buz_init();
		
	for(int i = 1;i <= 4 ; i++){
		word(0xFF,0xF0);
		tone(0x8888);
		delayMs(200);
		PTD->PCOR = 0x02;
		PTD->PCOR = 0x04;
		PTD->PSOR = 0x01;
		PTD->PSOR = 0x08;
		delayMs(50);
		PTD->PCOR = 0x01;
		PTD->PCOR = 0x08;
		PTD->PSOR = 0x02;
		PTD->PSOR = 0x04;
		delayMs(50);
		FTM0->SC = 0x00;
	}
		
	}
	
}

void party(void){

	PTB->PTOR = 0x400000;
	delayMs(125);
	PTE->PTOR = 0x4000000;
	delayMs(125);
	PTB->PTOR = 0x200000; 
	delayMs(125);
}

void enable_clk(void) {
	SIM->SCGC5 |= 0x0200; /* enable clock to Port A */
	SIM->SCGC5 |= 0x0400; /* enable clock to Port B */
	SIM->SCGC5 |= 0x0800; /* enable clock to Port C */
	SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
	SIM->SCGC5 |= 0x2000; /* enable clock to Port E */
}

void LED_init(void) {

//		Multifunction Shield
	PORTD->PCR[0] = 0x0100;
	PORTD->PCR[1] = 0x0100;
	PORTD->PCR[2] = 0x0100;
	PORTD->PCR[3] = 0x0100;
	PTD->PDDR |= 0x0F; /* make output pin */
	 
//		RED
	PORTB->PCR[22] = 0x0100; /* make PTB22 pin as GPIO */
	PTB->PDDR |= 0x400000; /* make PTB21, 22 as output pin */
	
//		GREEN		
	PORTE->PCR[26] = 0x0100; /* make PTE26 pin as GPIO */
	PTE->PDDR |= 0x4000000; /* make PTE26 as output pin */
	
//		BLUE	
	PORTB->PCR[21] = 0x0100; /* make PTB21 pin as GPIO */
	PTB->PDDR |= 0x200000; /* make PTB21, 22 as output pin */
	
// 		Start-up
	PTB->PSOR = 0x400000; /* turn off red LED */
	PTE->PSOR = 0x4000000; /* turn off green LED */
	PTB->PSOR = 0x200000; /* turn off blue LED */
	PTD->PSOR = 0x0F;
}

void Switch_init(void){

// SW1
	PORTB->PCR[3] |= 0x0100;
	PTB->PDDR &= ~(0x08);
	
// SW2
	PORTB->PCR[10] |= 0x0100;
	PTB->PDDR &= ~(0x400);
	
// SW3
	PORTB->PCR[11] |= 0x0100;
	PTB->PDDR &= ~(0x800);

//--------------ON Board SW---------------------
// SW3
	PORTA->PCR[4] = 0x0100; /* make it GPIO */
	PTA->PDDR &= ~0x0010; /* make pin input */
// SW2
	PORTA->PCR[6] = 0x0100; /* make it GPIO */
	PTA->PDDR &= ~0x0040; /* make pin input */
	
}

void Buz_init(void){
	
	PORTA->PCR[1] = 0x300;
	PTA->PDDR |= 0x2;

}
void tone(int n){

SIM->SCGC6 |= 0x1000000;
SIM->SOPT2 |= 0x00;
FTM0->SC = 0x00;
FTM0->MOD = n;
FTM0->CNTIN = 0x00;
	
FTM0->CONTROLS[6].CnSC = 0x14;
FTM0->CONTROLS[6].CnV = FTM0->CNT;
FTM0->SC = 0x0F;
	
}

void InterruptA_init(void){
__disable_irq(); /* Step 6.2: disable all IRQs */
PORTA->PCR[4] &= ~0xF0000; /* Step 6.3: clear interrupt selection */
PORTA->PCR[4] |= 0xA0000; /* Step 6.4: enable falling edge interrupt */
NVIC->ISER[1] |= 0x8000000; /* Step 6.5: enable interrupt for PORTA at NVIC */
	
NVIC->IP[59] |= 0x10; 					/* set priority for GPIOA (IRQ 59) */
__enable_irq(); /* Step 6.6: global enable IRQs */
}

void InterruptB_init(void){
	__disable_irq();
	PORTB->PCR[3] &= ~0xF0000;
	PORTB->PCR[3] |= 0xA0000;
	NVIC->ISER[1] |= 0x10000000; 

	PORTB->PCR[10] &= ~0xF0000; 
	PORTB->PCR[10] |= 0xA0000; 
	NVIC->ISER[1] |= 0x10000000; 

	PORTB->PCR[11] &= ~0xF0000; 
	PORTB->PCR[11] |= 0xA0000; 
	NVIC->ISER[1] |= 0x10000000; 
		
	NVIC->IP[60] |= 0x20;
	__enable_irq(); 
}

void PORTA_IRQHandler(void) {

for(int i = 1;i <= 100 ; i++){
	FTM0->SC = 0x00;
	word(0xFF,0xF0);
	PTD->PCOR = 0x0F;
	delayMs(100);
	PTD->PSOR = 0x0F;
	delayMs(100);
	
}
PORTA->ISFR = 0x10; /* Step clear interrupt flag */
}

void word_init(void){

	PORTC->PCR[12] |= 0x100;
	PORTC->PCR[3] |= 0x100;
	PORTB->PCR[23] |= 0x100;
	
	PTC->PDDR |= 0x08;
	PTC->PDDR |= 0x1000;
	PTB->PDDR |= 0x800000;
	
	PTC->PDOR &= ~0x08;
	
	word(0xFF,0xF0);

}

void word(unsigned data,unsigned pos){

unsigned int i;
unsigned int j;
	
	PTB->PDOR &= ~0x800000;

for(i = 0; i < 8 ; i++){
	
	PTC->PDOR = ((data >> i) & 1) << 12;
	
	PTC->PDOR |= 0x08;

	PTC->PDOR &= ~0x08;

	
}

for(j = 0;j < 8 ;j++){

	PTC->PDOR = ((pos >> j) & 1) << 12;
	
	PTC->PDOR |= 0x08;

	PTC->PDOR &= ~0x08;
	
}

	PTB->PDOR |= 0x800000;
	delayMs(1);

}

void love(void){
	
	for(int i = 1; i <= 100;i++){
		word(0x61,0x10);
		word(0xC7,0x20);
		word(0xC5,0x40);
		word(0xE3,0x80);
	}
		for(int j = 1; j <= 100;j++){
		word(0xE3,0x20);
		word(0x61,0x40);
	}
		for(int k = 1; k <= 100;k++){
		word(0x99,0x10);
		word(0x9F,0x20);
		word(0x0D,0x40);
	}
}

void Mery_Xmas(void){
	
	for(int i = 1; i <= 100;i++){
		word(0x89,0x10);
		word(0xF5,0x20);
		word(0x61,0x40);
		word(0x57,0x80);
 }
	for(int j = 1; j <= 100;j++){
		word(0x49,0x10);
		word(0x11,0x20);
		word(0x57,0x40);
		word(0x91,0x80);
 }
}
	
void Halo(void){
	
		for(int i = 1; i <= 100;i++){
		word(0x03,0x10);
		word(0xE3,0x20);
		word(0x11,0x40);
		word(0x91,0x80);
 }
		for(int j = 1; j <= 100;j++){
		word(0x70,0x10);
		word(0xC5,0x20);
		word(0xF5,0x40);
		word(0x31,0x80);
 }
		for(int k = 1; k <= 100;k++){
		word(0xF3,0x20);
		word(0xAB,0x40);
 }
		for(int n = 1; n <= 100;n++){
		word(0xD5,0x10);
		word(0xF5,0x20);
		word(0x11,0x40);
		word(0x49,0x80);
 }
}
void HNY(void){
	
		for(int i = 1; i <= 100;i++){
		word(0x89,0x20);
		word(0xD5,0x40);
		word(0x91,0x80);
 }

}
void Y23(void){
	
	for(int x = 1; x <= 10;x++){
			word(0x25,0x80);
	 }
	for(int y = 1; y <= 10;y++){
			word(0x03,0x40);
			word(0x25,0x80);
	 }
	for(int j = 1; j <= 10;j++){
			word(0x25,0x20);
			word(0x03,0x40);
			word(0x25,0x80);
	 }
	for(int i = 1; i <= 100;i++){
			word(0x0D,0x10);
			word(0x25,0x20);
			word(0x03,0x40);
			word(0x25,0x80);
	 }
}
void Y24(void){
	
	for(int x = 1; x <= 10;x++){
			word(0x25,0x80);
	 }
	for(int y = 1; y <= 10;y++){
			word(0x03,0x40);
			word(0x25,0x80);
	 }
	for(int k = 1; k <= 10;k++){
			word(0x25,0x20);
			word(0x03,0x40);
			word(0x25,0x80);
	 }
	for(int i = 1; i <= 100;i++){
			word(0x99,0x10);
			word(0x25,0x20);
			word(0x03,0x40);
			word(0x25,0x80);
	 }
}
void ms1(void){

	word(0xFF,0xF0);
	Buz_init();
	
	tone(0xEEEE);
	PTD->PCOR = 0x02;
	delayMs(200);
	tone(0x000E);
	PTD->PCOR = 0x08;
	delayMs(100);
	tone(0x0001);
	PTD->PCOR = 0x04;
	delayMs(80);
	PTD->PCOR = 0x01;
	delayMs(10);
	PTD->PSOR = 0x0F;
	FTM0->SC = 0x00;
}
void ms2(void){
	
	word(0xFF,0xF0);
	Buz_init();
	
	tone(0xDDDD);
	PTD->PCOR = 0x01;
	delayMs(100);
	tone(0xEEEE);
	PTD->PCOR = 0x04;
	delayMs(200);
	tone(0x0001);
	PTD->PCOR = 0x08;
	delayMs(80);
	PTD->PCOR = 0x02;
	delayMs(10);
	PTD->PSOR = 0x0F;
	FTM0->SC = 0x00;
}
	
void delayMs(int n) {
int i;
int j;
for(i = 0 ; i < n; i++)
for (j = 0; j < 7000; j++) {}
}
