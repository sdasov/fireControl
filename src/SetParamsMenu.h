/*
 * SetParamsMenu.h
 *
 *  Created on: 12 окт. 2019 г.
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
