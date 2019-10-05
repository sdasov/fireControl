/*
 * LedDisplay.cpp
 *
 *  Created on: 23 сент. 2019 г.
 *      Author: serg
 */

#include "LedDisplay.h"
#include <stm32f103xb.h>

#define DRS1 GPIO_BSRR_BS8
#define DRS0 GPIO_BSRR_BR8
#define DRW1 GPIO_BSRR_BS9
#define DRW0 GPIO_BSRR_BR9
#define DE1 GPIO_BSRR_BS10
#define DE0 GPIO_BSRR_BR10
#define DBUS GPIOB->ODR
#define DBUSMASK 0xf000

LedDisplay::LedDisplay() {initDisplay();}

LedDisplay::~LedDisplay() {}

void LedDisplay::delayDisplay(int timeInMks)
{
    int f = 0;
    while(f<timeInMks)
    {
    	for(int i=0;i<4;i++){__NOP();}
        f++;
    }
}

void LedDisplay::pulseE(){GPIOA->BSRR=DE1;delayDisplay(2);GPIOA->BSRR=DE0;}

void LedDisplay::initDisplay()
{
    delayDisplay(15000);
    GPIOA->BSRR=DRW0;
	GPIOA->BSRR=DRS0;
    DBUS=(DBUS&(~DBUSMASK))|((0x3)<<12);
	pulseE();
	delayDisplay(4200);

	pulseE();
	delayDisplay(110);

	pulseE();
	delayDisplay(110);

	DBUS=(DBUS&(~DBUSMASK))|((0x2)<<12);
	pulseE();
	delayDisplay(40);

	DBUS=(DBUS&(~DBUSMASK))|((0x2)<<12);
	pulseE();
	DBUS=(DBUS&(~DBUSMASK))|((0x8)<<12);
	pulseE();
	delayDisplay(400);

	//Display off
	DBUS=(DBUS&(~DBUSMASK))|((0x0)<<12);
	pulseE();
	DBUS=(DBUS&(~DBUSMASK))|((0x8)<<12);
	pulseE();
	delayDisplay(40);

	//Display clear
	DBUS=(DBUS&(~DBUSMASK))|((0x0)<<12);
	pulseE();
	DBUS=(DBUS&(~DBUSMASK))|((0x1)<<12);
	pulseE();
	delayDisplay(1550);
	//Entry Mode Set
	DBUS=(DBUS&(~DBUSMASK))|((0x0)<<12);
	pulseE();
	DBUS=(DBUS&(~DBUSMASK))|((0x6)<<12);
	pulseE();
	delayDisplay(40);
	//Display on/off control
	DBUS=(DBUS&(~DBUSMASK))|((0x0)<<12);
	pulseE();
	DBUS=(DBUS&(~DBUSMASK))|((0xe)<<12);
	pulseE();
	delayDisplay(40);
}

void LedDisplay::writeDisplay(unsigned char byte, bool isData)
{
	//DBUS &=(~DBUSMASK);
	//DBUS |=(byte&0xf0)<<8;
	DBUS=(DBUS&(~DBUSMASK))|((byte&0xf0)<<8);


	if(isData)
		GPIOA->BSRR=DRS1;
	else
		GPIOA->BSRR=DRS0;
	pulseE();

//	DBUS &=(~DBUSMASK);
//	DBUS |=(byte&0x0f)<<12;
	DBUS=(DBUS&(~DBUSMASK))|((byte&0x0f)<<12);

	pulseE();
	delayDisplay(40);
}

void LedDisplay::setCursor(char Row, char Col)
{
   char address;
   if (Row == 0)address = 0;else address = 0x40;
   address |= Col;
   writeDisplay(0x80 | address, false);
}

void LedDisplay::clearLcdDisplay()
{
	writeDisplay(0x01, false);
	writeDisplay(0x02, false);
	delayDisplay(1550);
}

void LedDisplay::putChar(char byte)
{
	writeDisplay(byte, true);
}

void LedDisplay::printString(char *buf)
{
	while(*buf!=0){putChar(*buf);buf++;}
}

void LedDisplay::onBlinkCursor()
{
	   writeDisplay(0x0f, false);
}

void LedDisplay::offBlinkCursor()
{
	   writeDisplay(0x0e, false);
}


