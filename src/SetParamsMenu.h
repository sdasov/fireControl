/*
 * SetParamsMenu.h
 *
 *  Created on: 12 ���. 2019 �.
 *      Author: sdaso
 */

#ifndef SETPARAMSMENU_H_
#define SETPARAMSMENU_H_

#include "MenuItem.h"

class SetParamsMenu: public MenuItem {
public:
	SetParamsMenu();
	~SetParamsMenu();
	virtual char* getName();
private:
	char itemName[17];
};

#endif /* SETPARAMSMENU_H_ */
