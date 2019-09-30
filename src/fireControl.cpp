/*
 * fireControl.cpp
 *
 *  Created on: 24 сент. 2019 г.
 *      Author: sdaso
 */

#include <stm32f103xb.h>
#include <stdio.h>
#include "perephConfig.h"
#include "Rtc.h"
#include "LedDisplay.h"
#include "KeyboardDriver.h"
#include "TemperatureReader.h"

KeyboardDriver keyboardDriver;
TemperatureReader temperatureReader;

extern "C" void SystemInit(void){for(int i=0;i<0xffff;i++)asm("NOP");}
extern "C"  void TIM4_IRQHandler(void) //ds18b20
{
	TIM4->CR1=0;
	TIM4->SR &= ~TIM_SR_UIF;
//	GPIOC->ODR=GPIOC->ODR^GPIO_ODR_ODR13; //blink
	temperatureReader.timer4Interrupt();
}

extern "C"  void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~TIM_SR_UIF;
	unsigned int keyboardData=GPIOB->IDR;
	keyboardData=(keyboardData>>4)&0xf;
	if(keyboardData!=0)keyboardDriver.addKey(keyboardData);
	//GPIOC->ODR=GPIOC->ODR^GPIO_ODR_ODR13; //blink
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

	led.clearLcdDisplay();
	led.putChar('A');


	//start timer3
	int timeInMs=250;
	TIM3->CNT=0;//timeInMs*2-2;
	TIM3->PSC=36000;
	TIM3->ARR=timeInMs*2;
	TIM3->DIER=TIM_DIER_UIE;
	TIM3->CR1=TIM_CR1_CEN;

//	DSTCurrentCmdPtr=0;
//	DSTLastCmdPtr=DSTLASTCMD;//6
//	DSTCommandStage=0;
	//start timer4 ds18b20
	int timeInMks=100;
	TIM4->CNT=2;//timeInMks-1;
	TIM4->PSC=72;
	TIM4->ARR=timeInMks;
	TIM4->DIER=TIM_DIER_UIE;
	TIM4->CR1=TIM_CR1_CEN;


	//stop otdacha
	//TIM3->CR1&=~TIM_CR1_CEN;

    while(1)
	{
		//GPIOC->BSRR=GPIO_BSRR_BR13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");
		//GPIOC->BSRR=GPIO_BSRR_BS13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");

		printDateTime(mrtc,led);
		led.setCursor(0,1);
		char outbuf[15];
		//signed char tempreche=(DSTCommands[14]<<4)|(DSTCommands[12]>>4);
		//signed short tempreche=(DSTCommands[14]<<8)|(DSTCommands[12]);
//		float f=(((float) tempreche / 16.0) * 10.0);
		//float f=(((float) tempreche / 16.0));

		sprintf(outbuf,"%+-2d",temperatureReader.get8bitTemperature(0));
		led.printString(outbuf);
		led.putChar(' ');

		sprintf(outbuf,"%+-2d",temperatureReader.get8bitTemperature(1));
		led.printString(outbuf);
		led.putChar(' ');

		sprintf(outbuf,"%+-2d",temperatureReader.get8bitTemperature(2));
		led.printString(outbuf);


//		led.setCursor(0,1);
//		while(keyboardDriver.isKey())
//			led.putChar(halfToChar(keyboardDriver.getKey()));

		delay(300000);
	}
}

