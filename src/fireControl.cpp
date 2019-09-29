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

//unsigned char rccData[40];
//unsigned char keyboardCntr[4];

KeyboardDriver keyboardDriver;
//Working with temp sensors DS18B20
//CMD = 1 Initialization sensors
#define DSTCMD_INIT 0x10
#define DSTCMD_WRITEBYTE 0x21
#define DSTCMD_WITECONVERSION 0x30
#define DSTCMD_READBYTE 0x41


const unsigned int DSTBRPins[]={GPIO_BSRR_BR3,GPIO_BSRR_BR8,GPIO_BSRR_BR9};
const unsigned int DSTBSPins[]={GPIO_BSRR_BS3,GPIO_BSRR_BS8,GPIO_BSRR_BS9};
const unsigned int DSTIDRPins[]={GPIO_IDR_IDR3,GPIO_IDR_IDR8,GPIO_IDR_IDR9};
//0-4 5 10 13
#define DSTLASTCMD 13
unsigned short DSTCommands[]={0x0200|DSTCMD_INIT,0x0200|DSTCMD_WRITEBYTE,0xCC,0x0200|DSTCMD_WRITEBYTE,0x44,\
		DSTCMD_WITECONVERSION,\
		0x0200|DSTCMD_INIT,0x0200|DSTCMD_WRITEBYTE,0xCC,0x0200|DSTCMD_WRITEBYTE,0xBE,\
		0x0200|DSTCMD_READBYTE,0x0,0x0200|DSTCMD_READBYTE,0x0};
unsigned char DSTCurrentCmdPtr;
unsigned char DSTLastCmdPtr;
unsigned char DSTCommandStage;
unsigned char DSTError;
unsigned char bitCounter;
unsigned char readedByte;

#define DSTERROR_NO_SENSOR 1

inline void setTimer4(unsigned short timeInMks)
{
	//TIM4->CNT=0;//timeInMks-2;
	TIM4->ARR=timeInMks;
	TIM4->CR1=TIM_CR1_CEN;
}

extern "C" void SystemInit(void){for(int i=0;i<0xffff;i++)asm("NOP");}

void DSTInitSensor(unsigned char pinPtr)
{
	switch(DSTCommandStage)
	{
		//Pin to low, wait minimum 480 mks
		case 0:
			GPIOB->BSRR=DSTBRPins[pinPtr];//GPIO_BSRR_BR3;
			DSTCommandStage=1;setTimer4(500);
			break;
		//Pin to high, wait minimum 60 mks
		case 1:
			GPIOB->BSRR=DSTBSPins[pinPtr];//GPIO_BSRR_BS3;
			DSTCommandStage=2;setTimer4(70);
			break;
		//Pin to high, wait minimum 60 mks
		case 2:
			if((GPIOB->IDR&DSTIDRPins[pinPtr])!=0)
			{
				DSTError=DSTERROR_NO_SENSOR;
			}
			DSTCommandStage=0;
			setTimer4(250);
			break;
	}
}

void DSTWriteByte(unsigned char pinPtr)
{
	unsigned char byteToSend=DSTCommands[DSTCurrentCmdPtr+1];
	switch(DSTCommandStage)
	{
		case 0:
			bitCounter=0;
			// no break

		case 1: //Pin to low, wait minimum 1 mks
			GPIOB->BSRR=DSTBRPins[pinPtr];//GPIO_BSRR_BR3;
			DSTCommandStage=2;setTimer4(2);
			break;
		//
		case 2:
			if(((byteToSend>>bitCounter)&1)==1)
				GPIOB->BSRR=DSTBSPins[pinPtr];//GPIO_BSRR_BS3;
			DSTCommandStage=3;setTimer4(70);
			break;
		//Pin to high, wait minimum 1 mks
		case 3:
			GPIOB->BSRR=DSTBSPins[pinPtr];//GPIO_BSRR_BS3;
			if(bitCounter<7)
			{
				DSTCommandStage=1;
				bitCounter++;
			}
			else
			{
				DSTCommandStage=0;
			}
			setTimer4(2);
			break;
	}
}

void DSTWaitConversion()
{
	DSTCommandStage++;
	setTimer4(40000);
	if(DSTCommandStage>20)DSTCommandStage=0;
}

void DSTReadByte(unsigned char pinPtr)
{
	switch(DSTCommandStage)
	{
		case 0:
			bitCounter=0;
			DSTCommands[DSTCurrentCmdPtr+1]=0;
			// no break

		case 1: //Pin to low, wait minimum 1 mks
			GPIOB->BSRR=DSTBRPins[pinPtr];//GPIO_BSRR_BR3;
			DSTCommandStage=2;setTimer4(2);
			break;
		//Pin to high
		case 2:
			GPIOB->BSRR=DSTBSPins[pinPtr];//GPIO_BSRR_BS3;
			DSTCommandStage=3;setTimer4(14);
			break;
		//Read bit
		case 3:
			if((GPIOB->IDR&DSTIDRPins[pinPtr])!=0)
			{
				DSTCommands[DSTCurrentCmdPtr+1]=DSTCommands[DSTCurrentCmdPtr+1]|(1<<bitCounter);
			}
			if(bitCounter<7)
			{
				DSTCommandStage=1;
				bitCounter++;
			}
			else
			{
				DSTCommandStage=0;
			}
			setTimer4(50);
			break;
	}
}

extern "C"  void TIM4_IRQHandler(void) //ds18b20
{
	TIM4->CR1=0;
	TIM4->SR &= ~TIM_SR_UIF;
	GPIOC->ODR=GPIOC->ODR^GPIO_ODR_ODR13; //blink
	unsigned short command=DSTCommands[DSTCurrentCmdPtr];
	switch(command&0xff)
	{
		//Initialization sensors
		case DSTCMD_INIT:
			DSTInitSensor(command>>8);
			break;
		case DSTCMD_WRITEBYTE:
			DSTWriteByte(command>>8);
			break;
		case DSTCMD_WITECONVERSION:
			DSTWaitConversion();
			break;
		case DSTCMD_READBYTE:
			DSTReadByte(command>>8);
			break;

	}
	if(DSTCommandStage==0)
	{
		if(DSTError!=0)
		{
			DSTCurrentCmdPtr=0;
		}
		else
		if(DSTCurrentCmdPtr!=DSTLastCmdPtr)	DSTCurrentCmdPtr+=(command&0xf)+1;
			else DSTCurrentCmdPtr=0;
	}
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

	DSTCurrentCmdPtr=0;
	DSTLastCmdPtr=DSTLASTCMD;//6
	DSTCommandStage=0;
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
		signed short tempreche=(DSTCommands[14]<<8)|(DSTCommands[12]);
//		float f=(((float) tempreche / 16.0) * 10.0);
		float f=(((float) tempreche / 16.0));

		sprintf(outbuf,"%+.3f",f);
		led.printString(outbuf);



//		led.setCursor(0,1);
//		while(keyboardDriver.isKey())
//			led.putChar(halfToChar(keyboardDriver.getKey()));

		delay(300000);
	}
}

