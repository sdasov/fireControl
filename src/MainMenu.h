/*
 * MainMenu.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: sdaso
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "MenuItem.h"
#include "LedDisplay.h"

class MainMenu {
public:
	MainMenu(LedDisplay *ledDisplay);
	~MainMenu();
	void addMenuItem(MenuItem *menuItem);
	void dispalayMenu();
	bool isWork();
	void setKey(char key);

private:
	char itemName[10];
	MenuItem *menuItems[10];
	int currentMenuItemIndex;
	int sizeMenuItems;
	bool isWorking;
	LedDisplay *led;

	void drawCurrentItem();
};

#endif /* MAINMENU_H_ */
