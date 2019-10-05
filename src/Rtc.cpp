/*
 * Rtc.cpp
 *
 *  Created on: 23 сент. 2019 г.
 *      Author: serg
 */
#include <stm32f103xb.h>
#include "Rtc.h"

const char *daysOfWeek[]={"OO","Su","Mo","Tu","We","Th","Fr","Sa"};

Rtc::Rtc() {}
Rtc::~Rtc() {}

char Rtc::halfToChar(char val){if(val<10)return val+'0';else return val+'A'-10;}

void Rtc::writeRTCZero()
{
    I2C2->CR1 |= I2C_CR1_PE;
    I2C2->CR1 |= I2C_CR1_START;

    while (!(I2C2->SR1 & I2C_SR1_SB)){__NOP();}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    // send device address
    I2C2->DR = 0xd0;
    while (!(I2C2->SR1 & I2C_SR1_ADDR)){__NOP();}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    I2C2->DR = 0;
    while (!(I2C2->SR1 & I2C_SR1_BTF));
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Rtc::readRTCData()
{
	unsigned char i;
	I2C2->CR1 |= I2C_CR1_PE;
    I2C2->CR1 |= I2C_CR1_START;

    while (!(I2C2->SR1 & I2C_SR1_SB)){__NOP();}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    // send device address
    I2C2->DR = 0xd1;
    while (!(I2C2->SR1 & I2C_SR1_ADDR)){/*__NOP();*/}
    (void)I2C2->SR1;
    (void)I2C2->SR2;

    for(i=0;i<6;i++)
    {
		I2C2->CR1 |= I2C_CR1_ACK;
		while (!(I2C2->SR1 & I2C_SR1_RXNE)){} //
		RTCdata[i]=I2C2->DR;
		I2C2->CR1&=~I2C_CR1_ACK;
		(void)I2C2->SR1;
		(void)I2C2->SR2;
    }

	while (!(I2C2->SR1 & I2C_SR1_RXNE)){}
    RTCdata[i]=I2C2->DR;
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Rtc::writeRTCData()
{
	unsigned char i;
	I2C2->CR1 |= I2C_CR1_PE;
    I2C2->CR1 |= I2C_CR1_START;

    while (!(I2C2->SR1 & I2C_SR1_SB)){__NOP();}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    // send device address
    I2C2->DR = 0xd0;
    while (!(I2C2->SR1 & I2C_SR1_ADDR)){/*__NOP();*/}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    I2C2->DR=0;
    while (!(I2C2->SR1 & I2C_SR1_BTF));
    (void)I2C2->SR1;
    (void)I2C2->SR2;

    for(i=0;i<7;i++)
    {
    	I2C2->DR =RTCdata[i];
        while (!(I2C2->SR1 & I2C_SR1_BTF));
        (void)I2C2->SR1;
        (void)I2C2->SR2;
    }
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Rtc::readRTC()
{
	writeRTCZero();
	readRTCData();
}
//DFddmm20yy hh:mm0 - 17 bytes
void Rtc::getDateTimeString(char *buf, char *dateTime)
{
	//Day of week
	buf[0]=daysOfWeek[dateTime[3]&0xf][0];
	buf[1]=daysOfWeek[dateTime[3]&0xf][1];

	//date
	buf[2]=halfToChar(dateTime[4]>>4);
	buf[3]=halfToChar(dateTime[4]&0xf);

	//month
	buf[4]=halfToChar(dateTime[5]>>4);
	buf[5]=halfToChar(dateTime[5]&0xf);

	//year
	buf[6]='2';
	buf[7]='0';
	buf[8]=halfToChar(dateTime[6]>>4);
	buf[9]=halfToChar(dateTime[6]&0xf);

	//space
	buf[10]=' ';

	//hours
	buf[11]=halfToChar(dateTime[2]>>4);
	buf[12]=halfToChar(dateTime[2]&0xf);

	buf[13]=':';
	//minutes
	buf[14]=halfToChar(dateTime[1]>>4);
	buf[15]=halfToChar(dateTime[1]&0xf);
	buf[16]=0;
}

void Rtc::getDateString(char *buf, char *dateTime)
{
	//Day of week
	buf[0]=daysOfWeek[dateTime[3]&0xf][0];buf[1]=daysOfWeek[dateTime[3]&0xf][1];
	//date
	buf[2]=halfToChar(dateTime[4]>>4);buf[3]=halfToChar(dateTime[4]&0xf);
	//month
	buf[4]=halfToChar(dateTime[5]>>4);buf[5]=halfToChar(dateTime[5]&0xf);
	//year
	buf[6]='2';	buf[7]='0';
	buf[8]=halfToChar(dateTime[6]>>4);buf[9]=halfToChar(dateTime[6]&0xf);
	buf[10]=0;
}

void Rtc::getTimeString(char *buf, char *dateTime)
{
	//hours
	buf[0]=halfToChar(dateTime[2]>>4);buf[1]=halfToChar(dateTime[2]&0xf);
	buf[2]=':';
	//minutes
	buf[3]=halfToChar(dateTime[1]>>4);buf[4]=halfToChar(dateTime[1]&0xf);
	buf[5]=':';
	//Seconds
	buf[6]=halfToChar(dateTime[0]>>4);buf[7]=halfToChar(dateTime[0]&0xf);
	buf[8]=0;
}

char *Rtc::getRawDateTime(){readRTC();return RTCdata;}

void Rtc::setRawDateTime(char *rawDT)
{
	for(int i=0;i<7;i++)
	{
		RTCdata[i]=rawDT[i];
	}
	writeRTCData();
}

char Rtc::incbcd(char digit)
{
	digit++;
	if((digit&0xf)>9)
	{
		digit=digit+0x10-0x0a;
		if(digit>0x90)digit=0;
	}
	return digit;
}

char Rtc::decbcd(char digit)
{
	digit--;
	if((digit&0xf)>9)
	{
		digit=digit-0x10-0x06;
		if(digit>0x90)digit=0x99;
	}
	return digit;
}

void Rtc::incParam(char *dateTime, unsigned char paramNumber)
{
	char val=incbcd(dateTime[paramNumber]);
	switch(paramNumber)
	{
		//seconds, minutes
		case 0:
		case 1: if(val>0x59)val=0;break;
		//hours
		case 2: if(val>0x23)val=0;break;
		//days
		case 3: if(val>0x07)val=1;break;
		//date
		case 4: if(val>0x31)val=1;break;
		//months
		case 5: if(val>0x12)val=1;break;
		//years
		case 6: break;
	}
	dateTime[paramNumber]=val;
}

void Rtc::decParam(char *dateTime, unsigned char paramNumber)
{
	char val=decbcd(dateTime[paramNumber]);
	switch(paramNumber)
	{
		//seconds, minutes
		case 0:
		case 1: if(val>0x59)val=0x59;break;
		//hours
		case 2: if(val>0x23)val=0x23;break;
		//days
		case 3: if(val==0x0)val=7;break;
		//date
		case 4: if(val==0x0)val=0x31;break;
		//months
		case 5: if(val==0x0)val=0x12;break;
		//years
		case 6: break;
	}
	dateTime[paramNumber]=val;
}
