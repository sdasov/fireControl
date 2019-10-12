/*
 * MainMenu.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: sdaso
 */
#include <string.h>
#include "MainMenu.h"
#include "KeyboardDriver.h"

MainMenu::MainMenu(LedDisplay *ledDisplay) {
	sizeMenuItems=0;
	currentMenuItemIndex=0;
	isWorking=false;
	led=ledDisplay;
}

MainMenu::~MainMenu() {
	// TODO Auto-generated destructor stub
}

void MainMenu::addMenuItem(MenuItem *menuItem){menuItems[sizeMenuItems++]=menuItem;}

void MainMenu::dispalayMenu()
{
	char space16[]="                ";
	isWorking=true;
	//clear display
	led->setCursor(1,0);led->printString(space16);
	currentMenuItemIndex=0;
	drawCurrentItem();
}

bool MainMenu::isWork() {
	return isWorking;
}

void MainMenu::setKey(char key)
{
	switch (key) {
		case KEY1:
			if(currentMenuItemIndex==0)currentMenuItemIndex=sizeMenuItems-1; else currentMenuItemIndex--;
			drawCurrentItem();
			break;
		case KEY2:
			if(currentMenuItemIndex==sizeMenuItems-1)currentMenuItemIndex=0; else currentMenuItemIndex++;
			drawCurrentItem();
			break;
		case KEY3:
			break;
		case KEY4:
			isWorking=false;
			break;
	}
}

void MainMenu::drawCurrentItem()
{
	char space16[]="                ";
	led->setCursor(0,0);led->printString(space16);
	led->setCursor(0,0);led->printString(menuItems[currentMenuItemIndex]->getName());

}
