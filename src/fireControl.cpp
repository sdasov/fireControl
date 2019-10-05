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
	rtc.getDateTimeString(dateTimeString,rtc.getRawDateTime());
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
	char key;
	char preSaveDTValue;
	char dateStr[20], timeStr[20];
	char rawDateTimeData[7];
	const unsigned char paramToLedPozition[]={0x16,0x13,0x10,0x00,0x02,0x04,0x06};
	enum menuState {mainMenuState=0,prepareDTedit,selektDateTime,editParmDateTime};
	unsigned char currentDTParamEdit=3;
	menuState currentState=mainMenuState;
    while(1)
	{
		//GPIOC->BSRR=GPIO_BSRR_BR13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");
		//GPIOC->BSRR=GPIO_BSRR_BS13;
		delay(100000);//for(int i=0;i<1000000;i++)asm("NOP");

		key=keyboardDriver.getKey();
		//if((key==KEY3)&&(isEditDateTimeMode==false)){isEditDateTimeMode=true;key=0;}

		switch(currentState)
		{
			case mainMenuState:
				printDateTime(mrtc,led);
				if(key==KEY3)currentState=prepareDTedit;
				break;
			case prepareDTedit:
				char *rawDT;
				rawDT=mrtc.getRawDateTime();
				for(int i=0;i<7;i++)
				{
					rawDateTimeData[i]=rawDT[i];
				}
				mrtc.getDateString(dateStr,rawDateTimeData);
				mrtc.getTimeString(timeStr,rawDateTimeData);
				led.clearLcdDisplay();
				led.setCursor(0,0);
				led.printString(dateStr);
				led.setCursor(1,0);
				led.printString(timeStr);
				led.setCursor(paramToLedPozition[currentDTParamEdit]>>4,paramToLedPozition[currentDTParamEdit]&0xf);
				currentState=selektDateTime;
				break;
			case selektDateTime:
				switch(key)
				{
					case KEY1:
						if(currentDTParamEdit==0)currentDTParamEdit=6;else currentDTParamEdit--;
						led.setCursor(paramToLedPozition[currentDTParamEdit]>>4,paramToLedPozition[currentDTParamEdit]&0xf);
						break;
					case KEY2:
						if(currentDTParamEdit==6)currentDTParamEdit=0;else currentDTParamEdit++;
						led.setCursor(paramToLedPozition[currentDTParamEdit]>>4,paramToLedPozition[currentDTParamEdit]&0xf);
						break;
					case KEY3:
						preSaveDTValue=rawDateTimeData[currentDTParamEdit];
						led.onBlinkCursor();
						currentState=editParmDateTime;
						break;
					case KEY4:currentState=mainMenuState;break;
				}
				break;
				case editParmDateTime:
					switch(key)
					{
						case KEY1:
							mrtc.decParam(rawDateTimeData,currentDTParamEdit);
							mrtc.getDateString(dateStr,rawDateTimeData);
							mrtc.getTimeString(timeStr,rawDateTimeData);
							led.clearLcdDisplay();
							led.setCursor(0,0);
							led.printString(dateStr);
							led.setCursor(1,0);
							led.printString(timeStr);
							led.setCursor(paramToLedPozition[currentDTParamEdit]>>4,paramToLedPozition[currentDTParamEdit]&0xf);
							break;
						case KEY2:
							mrtc.incParam(rawDateTimeData,currentDTParamEdit);
							mrtc.getDateString(dateStr,rawDateTimeData);
							mrtc.getTimeString(timeStr,rawDateTimeData);
							led.clearLcdDisplay();
							led.setCursor(0,0);
							led.printString(dateStr);
							led.setCursor(1,0);
							led.printString(timeStr);
							led.setCursor(paramToLedPozition[currentDTParamEdit]>>4,paramToLedPozition[currentDTParamEdit]&0xf);
							break;
						case KEY3:
							mrtc.setRawDateTime(rawDateTimeData);led.offBlinkCursor();currentState=prepareDTedit;break;
						case KEY4:rawDateTimeData[currentDTParamEdit]=preSaveDTValue;led.offBlinkCursor();currentState=prepareDTedit;break;
					}

					break;
		}


		//led.setCursor(0,1);
		//char outbuf[15];
		//sprintf(outbuf,"%+-2d",temperatureReader.get8bitTemperature(0));
		//led.printString(outbuf);
		//led.putChar(' ');


//		led.setCursor(0,1);
//		while(keyboardDriver.isKey())
//			led.putChar(halfToChar(keyboardDriver.getKey()));

		///delay(300000);
	}
}

