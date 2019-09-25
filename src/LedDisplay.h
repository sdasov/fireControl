/*
 * LedDisplay.h
 *
 *  Created on: 23 сент. 2019 г.
 *      Author: serg
 */

#ifndef LEDDISPLAY_H_
#define LEDDISPLAY_H_

class LedDisplay {
private:
	void initDisplay();
	void delayDisplay(int timeInMks);
	void pulseE();
	void writeDisplay(unsigned char byte, bool isData);
public:
	LedDisplay();
	~LedDisplay();
	void clearLcdDisplay();
	void setCursor(char Row, char Col);
	void putChar(char byte);
	void printString(char *buf);
};

#endif /* LEDDISPLAY_H_ */
