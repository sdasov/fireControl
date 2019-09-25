/*
 * KeyboardDriver.cpp
 *
 *  Created on: 25 сент. 2019 г.
 *      Author: sdaso
 */

#include "KeyboardDriver.h"

KeyboardDriver::KeyboardDriver() {currentPtr=0;}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::setKey(char key)
{
	keysBuffer[currentPtr&0xf]=0;
	__disable_irq();
	currentPtr++;
	__enable_irq();

}

char KeyboardDriver::getKey()
{
	char key;
	if(currentPtr==0)return 0;
	__disable_irq();
	key=keysBuffer[(currentPtr-1)&0xf];
	currentPtr--;
	__enable_irq();

	return key;
}
