/*
 * SetParamsMenu.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: sdaso
 */
#include <string.h>
#include "SetParamsMenu.h"

SetParamsMenu::SetParamsMenu() {
	// TODO Auto-generated constructor stub
	strcpy(itemName,"Set Params");
}

SetParamsMenu::~SetParamsMenu() {
	// TODO Auto-generated destructor stub
}

 char* SetParamsMenu::getName()
{
	 return itemName;
}
