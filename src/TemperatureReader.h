/*
 * TemperatureReader.h
 *
 *  Created on: 30 сент. 2019 г.
 *      Author: sdaso
 */

#ifndef TEMPERATUREREADER_H_
#define TEMPERATUREREADER_H_
#include <stm32f103xb.h>

//Working with temp sensors DS18B20
//CMD = 1 Initialization sensors
#define DSTCMD_INIT 0x10
#define DSTCMD_WRITEBYTE 0x21
#define DSTCMD_WITECONVERSION 0x30
#define DSTCMD_READBYTE 0x41

#define DSTERROR_NO_SENSOR 1

#define DSTLASTCMD 41

class TemperatureReader {
public:
	TemperatureReader();
	~TemperatureReader();
	void timer4Interrupt();
	signed char get8bitTemperature(unsigned char sensor);
	signed short get16bitTemperature(unsigned char sensor);
private:
	static const unsigned int DSTBRPins[3];
	static const unsigned int DSTBSPins[3];
	static const unsigned int DSTIDRPins[3];
	static const unsigned short DSTCommands[43];
	unsigned char DSTReadBuffer[6];
	signed short frontReadBuffer[3];

	unsigned char DSTCurrentCmdPtr;
	unsigned char DSTLastCmdPtr;
	unsigned char DSTCommandStage;
	unsigned char DSTError;
	unsigned char bitCounter;
	unsigned char readedByte;

	inline void setTimer4(unsigned short timeInMks);
	void DSTInitSensor(unsigned char pinPtr);
	void DSTWriteByte(unsigned char pinPtr);
	void DSTWaitConversion();
	void DSTReadByte(unsigned char pinPtr);
};

#endif /* TEMPERATUREREADER_H_ */
