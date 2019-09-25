/*
 * KeyboardDriver.h
 *
 *  Created on: 25 ����. 2019 �.
 *      Author: sdaso
 */

#ifndef KEYBOARDDRIVER_H_
#define KEYBOARDDRIVER_H_

class KeyboardDriver {
public:
	KeyboardDriver();
	~KeyboardDriver();
	void setKey(char key);
	char getKey();
private:
	char keysBuffer[16];
	unsigned char currentPtr;
};

#endif /* KEYBOARDDRIVER_H_ */