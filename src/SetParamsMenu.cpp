/*
 * SetParamsMenu.cpp
 *
 *  Created on: 12 ���. 2019 �.
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
