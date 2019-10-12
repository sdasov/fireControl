/*
 * SetDateTime.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: sdaso
 */
#include <string.h>
#include "SetDateTime.h"

SetDateTime::SetDateTime() {
	// TODO Auto-generated constructor stub
	strcpy(itemName,"Set Date&Time");
}

SetDateTime::~SetDateTime() {
	// TODO Auto-generated destructor stub
}

char* SetDateTime::getName()
{
	 return itemName;
}
