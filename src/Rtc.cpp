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
		while (!(I2C2->SR1 & I2C_SR1_RXNE)){} // ожидаем окончания приема данных
		RTCdata[i]=I2C2->DR;
		I2C2->CR1&=~I2C_CR1_ACK;
		(void)I2C2->SR1;
		(void)I2C2->SR2;
    }

	while (!(I2C2->SR1 & I2C_SR1_RXNE)){}
    RTCdata[i]=I2C2->DR;
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Rtc::readRTC()
{
	writeRTCZero();
	readRTCData();
}
//DFddmm20yy hh:mm0 - 17 bytes
void Rtc::getDateTimeString(char *buf)
{
	readRTC();

	//Day of week
	buf[0]=daysOfWeek[RTCdata[3]&0xf][0];
	buf[1]=daysOfWeek[RTCdata[3]&0xf][1];

	//date
	buf[2]=halfToChar(RTCdata[4]>>4);
	buf[3]=halfToChar(RTCdata[4]&0xf);

	//month
	buf[4]=halfToChar(RTCdata[5]>>4);
	buf[5]=halfToChar(RTCdata[5]&0xf);

	//year
	buf[6]='2';
	buf[7]='0';
	buf[8]=halfToChar(RTCdata[6]>>4);
	buf[9]=halfToChar(RTCdata[6]&0xf);

	//space
	buf[10]=' ';

	//hours
	buf[11]=halfToChar(RTCdata[2]>>4);
	buf[12]=halfToChar(RTCdata[2]&0xf);

	buf[13]=':';
	//minutes
	buf[14]=halfToChar(RTCdata[1]>>4);
	buf[15]=halfToChar(RTCdata[1]&0xf);
	buf[16]=0;
}
