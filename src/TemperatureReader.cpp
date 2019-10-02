/*
 * TemperatureReader.cpp
 *
 *  Created on: 30 сент. 2019 г.
 *      Author: sdaso
 */
#include <stm32f103xb.h>
#include "TemperatureReader.h"

const unsigned int TemperatureReader::DSTBRPins[3]={GPIO_BSRR_BR0,GPIO_BSRR_BR8,GPIO_BSRR_BR9};
const unsigned int TemperatureReader::DSTBSPins[3]={GPIO_BSRR_BS0,GPIO_BSRR_BS8,GPIO_BSRR_BS9};
const unsigned int TemperatureReader::DSTIDRPins[3]={GPIO_IDR_IDR0,GPIO_IDR_IDR8,GPIO_IDR_IDR9};
const unsigned short TemperatureReader::DSTCommands[43]={
		0x0000|DSTCMD_INIT,0x0100|DSTCMD_INIT,0x0200|DSTCMD_INIT,
		0x0000|DSTCMD_WRITEBYTE,0xCC,0x0100|DSTCMD_WRITEBYTE,0xCC,0x0200|DSTCMD_WRITEBYTE,0xCC,
		0x0000|DSTCMD_WRITEBYTE,0x44,0x0100|DSTCMD_WRITEBYTE,0x44,0x0200|DSTCMD_WRITEBYTE,0x44,
		DSTCMD_WITECONVERSION,
		0x0000|DSTCMD_INIT,0x0100|DSTCMD_INIT,0x0200|DSTCMD_INIT,
		0x0000|DSTCMD_WRITEBYTE,0xCC,0x0100|DSTCMD_WRITEBYTE,0xCC,0x0200|DSTCMD_WRITEBYTE,0xCC,
		0x0000|DSTCMD_WRITEBYTE,0xBE,0x0100|DSTCMD_WRITEBYTE,0xBE,0x0200|DSTCMD_WRITEBYTE,0xBE,
		0x0000|DSTCMD_READBYTE,0x0,0x0000|DSTCMD_READBYTE,0x1,
		0x0100|DSTCMD_READBYTE,0x2,0x0100|DSTCMD_READBYTE,0x3,
		0x0200|DSTCMD_READBYTE,0x4,0x0200|DSTCMD_READBYTE,0x5};


TemperatureReader::TemperatureReader()
{
	DSTCurrentCmdPtr=0;
	DSTLastCmdPtr=DSTLASTCMD;
	DSTCommandStage=0;
	DSTError=0;
	bitCounter=0;
	readedByte=0;
}

TemperatureReader::~TemperatureReader() {}

inline void TemperatureReader::setTimer4(unsigned short timeInMks)
{
	//TIM4->CNT=0;//timeInMks-2;
	TIM4->ARR=timeInMks;
	TIM4->CR1=TIM_CR1_CEN;
}

void TemperatureReader::DSTInitSensor(unsigned char pinPtr)
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

void TemperatureReader::DSTWriteByte(unsigned char pinPtr)
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

void TemperatureReader::DSTWaitConversion()
{
	DSTCommandStage++;
	setTimer4(40000);
	if(DSTCommandStage>20)DSTCommandStage=0;
}

void TemperatureReader::DSTReadByte(unsigned char pinPtr)
{
	switch(DSTCommandStage)
	{
		case 0:
			bitCounter=0;
			readedByte=0;
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
				readedByte=readedByte|(1<<bitCounter);
			}
			if(bitCounter<7)
			{
				DSTCommandStage=1;
				bitCounter++;
			}
			else
			{
				DSTCommandStage=0;
				unsigned short addressToRead=DSTCommands[DSTCurrentCmdPtr+1];
				DSTReadBuffer[addressToRead]=readedByte;
				if((addressToRead&1)!=0)
				{
					frontReadBuffer[addressToRead>>1]=(DSTReadBuffer[addressToRead-1])|(readedByte<<8);
				}
			}
			setTimer4(50);
			break;
	}
}


void TemperatureReader::timer4Interrupt()
{
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
			else
				DSTCurrentCmdPtr=0;
	}
}

signed char TemperatureReader::get8bitTemperature(unsigned char sensor)
{
	//signed char temperature=(frontReadBuffer[sensor*2+1]<<4)|(frontReadBuffer[sensor*2]>>4);
	signed char temperature=frontReadBuffer[sensor]>>4;
	return temperature;
}

signed short TemperatureReader::get16bitTemperature(unsigned char sensor)
{
	return frontReadBuffer[sensor];
}
