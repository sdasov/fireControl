/*
 * fireControl.cpp
 *
 *  Created on: 24 сент. 2019 г.
 *      Author: sdaso
 */




/*
 * fSTM32F103c8.cpp
 *
 *  Created on: 20 СЃРµРЅС‚. 2019 Рі.
 *      Author: serg
 */
#include <stm32f103xb.h>
#include "perephConfig.h"
#include "Rtc.h"
#include "LedDisplay.h"

unsigned char rccData[40];

unsigned char keyboardCntr[4];

extern "C" void SystemInit(void){for(int i=0;i<0xffff;i++)asm("NOP");}
extern "C"  void TIM3_IRQHandler(void)
{

	TIM3->SR &= ~TIM_SR_UIF;
	unsigned int keyboardData=GPIOB->IDR;
	GPIOC->ODR=GPIOC->ODR^GPIO_ODR_ODR13;

	if((keyboardData&GPIO_IDR_IDR4)!=0)
	{
		if(keyboardCntr[0]<255)keyboardCntr[0]++;
	}else keyboardCntr[0]=0;

	if((keyboardData&GPIO_IDR_IDR5)!=0)
	{
		if(keyboardCntr[1]<255)keyboardCntr[1]++;
	}else keyboardCntr[1]=0;

	if((keyboardData&GPIO_IDR_IDR6)!=0)
	{
		if(keyboardCntr[2]<255)keyboardCntr[2]++;
	}else keyboardCntr[2]=0;

	if((keyboardData&GPIO_IDR_IDR7)!=0)
	{
		if(keyboardCntr[3]<255)keyboardCntr[3]++;
	}else keyboardCntr[3]=0;
}
void delay(int a)
{
    int f = 0;
    while(f < a)
    {
    	for(int i=0;i<4;i++){__NOP();}
        f++;
    }
}


unsigned char halfToChar(unsigned char val){if(val<10)return val+'0';else return val+'A'-10;}

void printDateTime(Rtc &rtc, LedDisplay &led)
{
	char dateTimeString[20];

	//led.clearLcdDisplay();
	led.setCursor(1,0);
	rtc.getDateTimeString(dateTimeString);
	led.printString(dateTimeString);
}


int main(void)
{

	RCCConfig();
	portConfig();
	initI2C2();
	NVICConfig();

	Rtc mrtc;
	LedDisplay led;

    //displayReset();
    //clearLCDScreen();
	led.clearLcdDisplay();
	led.putChar('A');
    //writeDisplay('A', true);

	//start timer3
	int timeInMs=250;
	TIM3->CNT=timeInMs*2-2;
	TIM3->PSC=36000;
	TIM3->ARR=timeInMs*2;
	TIM3->DIER=TIM_DIER_UIE;
	TIM3->CR1=TIM_CR1_CEN;

	//stop otdacha
	//TIM3->CR1&=~TIM_CR1_CEN;

    while(1)
	{
		//GPIOC->BSRR=GPIO_BSRR_BR13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");
		//GPIOC->BSRR=GPIO_BSRR_BS13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");

		printDateTime(mrtc,led);
//		led.clearLcdDisplay();
//		led.setCursor(1,0);
//		mrtc.getDateTimeString(dateTimeString);
//		led.printString(dateTimeString);

		led.setCursor(0,1);
		led.putChar(halfToChar(keyboardCntr[0]>>4));
		led.putChar(halfToChar(keyboardCntr[0]&0xf));
		led.putChar(halfToChar(keyboardCntr[1]>>4));
		led.putChar(halfToChar(keyboardCntr[1]&0xf));
		led.putChar(halfToChar(keyboardCntr[2]>>4));
		led.putChar(halfToChar(keyboardCntr[2]&0xf));
		led.putChar(halfToChar(keyboardCntr[3]>>4));
		led.putChar(halfToChar(keyboardCntr[3]&0xf));

		delay(300000);
	}
}

