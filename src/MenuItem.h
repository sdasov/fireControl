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
	virtual ~MenuItem();
	virtual char *getName()=0;
private:
	MenuItem *parent;
	//std::vector<MenuItem*> children;
	MenuItem *prev;
	MenuItem *next;

	MenuItem *firstChild;
	MenuItem *lastChild;
};

#endif /* MENUITEM_H_ */
