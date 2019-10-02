/*
 * Rtc.h
 *
 *  Created on: 23 сент. 2019 г.
 *      Author: serg
 */

#ifndef RTC_H_
#define RTC_H_

class Rtc {

private:
	void readRTC();
	void writeRTCZero();
	void readRTCData();
	char halfToChar(char val);
	void writeRTCData();
	void writeRTC();

	char RTCdata[7];

public:
	Rtc();
	~Rtc();
	void getDateTimeString(char *buf);
	void incMin();

};

#endif /* RTC_H_ */
