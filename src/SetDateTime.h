/*
 * SetDateTime.h
 *
 *  Created on: 12 ���. 2019 �.
 *      Author: sdaso
 */

#ifndef SETDATETIME_H_
#define SETDATETIME_H_

#include "MenuItem.h"

class SetDateTime: public MenuItem {
public:
	SetDateTime();
	~SetDateTime();
	virtual char* getName();
private:
	char itemName[17];

};

#endif /* SETDATETIME_H_ */
