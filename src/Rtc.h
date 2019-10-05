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
	char incbcd(char digit);
	char decbcd(char digit);


	char RTCdata[7];

public:
	Rtc();
	~Rtc();
	void getDateTimeString(char *buf, char *dateTime);
	void getDateString(char *buf, char *dateTime);
	void getTimeString(char *buf, char *dateTime);
	void incParam(char *dateTime, unsigned char paramNumber);
	void decParam(char *dateTime, unsigned char paramNumber);

	char *getRawDateTime();
	void setRawDateTime(char *rawDT);

};

#endif /* RTC_H_ */
