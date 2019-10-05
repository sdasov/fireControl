/*
 * KeyboardDriver.h
 *
 *  Created on: 25 сент. 2019 г.
 *      Author: sdaso
 */

#ifndef KEYBOARDDRIVER_H_
#define KEYBOARDDRIVER_H_

#define KEY1 0x1
#define KEY2 0x2
#define KEY3 0x4
#define KEY4 0x8


class KeyboardDriver {
public:
	KeyboardDriver();
	~KeyboardDriver();
	void addKey(char key);
	char getKey();
	bool isKey();
private:
	char keysBuffer[16];
	unsigned char currentPtr;
};

#endif /* KEYBOARDDRIVER_H_ */
