/*
 * MenuItem.h
 *
 *  Created on: 27 сент. 2019 г.
 *      Author: sdaso
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include <Vector>

class MenuItem {
public:
	MenuItem();
	~MenuItem();
	char *getName();
private:
	MenuItem *parent;
	std::vector<MenuItem*> children;

};

#endif /* MENUITEM_H_ */
