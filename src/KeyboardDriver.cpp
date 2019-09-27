/*
 * KeyboardDriver.cpp
 *
 *  Created on: 25 сент. 2019 г.
 *      Author: sdaso
 */

#include <stm32f103xb.h>
#include "KeyboardDriver.h"

KeyboardDriver::KeyboardDriver() {currentPtr=0;}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::addKey(char key)
{
	keysBuffer[currentPtr]=key;
	if(currentPtr<0xf)currentPtr++;
}

char KeyboardDriver::getKey()
{
	char key;
	if(currentPtr==0)return 0;
	__disable_irq();
	currentPtr--;
	key=keysBuffer[currentPtr];
	__enable_irq();

	return key;
}

bool KeyboardDriver::isKey()
{
	return currentPtr;
}
