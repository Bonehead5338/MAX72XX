// MAX72XX.h

#ifndef _MAX72XX_h
#define _MAX72XX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>

//MAX72XX Register Address Map (Ref Datasheet Table 2)
#define MAX72XX_REG_NOOP		0x00
#define MAX72XX_REG_DIGIT0		0x01
#define MAX72XX_REG_DIGIT1		0x02
#define MAX72XX_REG_DIGIT2		0x03
#define MAX72XX_REG_DIGIT3		0x04
#define MAX72XX_REG_DIGIT4		0x05
#define MAX72XX_REG_DIGIT5		0x06
#define MAX72XX_REG_DIGIT6		0x07
#define MAX72XX_REG_DIGIT7		0x08
#define MAX72XX_REG_DECODEMODE	0x09
#define MAX72XX_REG_INTENSITY	0x0A
#define MAX72XX_REG_SCANLIMIT	0x0B
#define MAX72XX_REG_SHUTDOWN	0x0C
#define MAX72XX_REG_DISPLAYTEST	0x0F

//Shutdown Regsister Values (Ref Datasheet Table 3)
#define MAX72XX_SDMODE_SHUTDOWN	0x00
#define MAX72XX_SDMODE_NORMAL		0x01

//Code B Font (Ref Datasheet Table 5)
#define MAX72XX_7SEG_0		0x00
#define MAX72XX_7SEG_1		0x01
#define MAX72XX_7SEG_2		0x02
#define MAX72XX_7SEG_3		0x03
#define MAX72XX_7SEG_4		0x04
#define MAX72XX_7SEG_5		0x05
#define MAX72XX_7SEG_6		0x06
#define MAX72XX_7SEG_7		0x07
#define MAX72XX_7SEG_8		0x08
#define MAX72XX_7SEG_9		0x09
#define MAX72XX_7SEG_DASH	0x0A
#define MAX72XX_7SEG_E		0x0B
#define MAX72XX_7SEG_H		0x0C
#define MAX72XX_7SEG_L		0x0D
#define MAX72XX_7SEG_P		0x0E
#define MAX72XX_7SEG_BLANK	0x0F
#define MAX72XX_7SEG_DP		0xE0	//OR with 7-SEG characters above for DP

//Scan-limit Register
#define MAX72XX_SCANLIMIT_0	0x00
#define MAX72XX_SCANLIMIT_1	0x01
#define MAX72XX_SCANLIMIT_2	0x02
#define MAX72XX_SCANLIMIT_3	0x03
#define MAX72XX_SCANLIMIT_4 0x04
#define MAX72XX_SCANLIMIT_5 0x05
#define MAX72XX_SCANLIMIT_6 0x06
#define MAX72XX_SCANLIMIT_7 0x07

//Display Test values
#define MAX72XX_TESTMODE_NORMAL	0x00
#define MAX72XX_TESTMODE_TEST	0x01

//display dimension
#define MATRIX_DIM 8

class MAX72XXClass
{
 protected:
	 //pin used for LOAD/CS
	 int LoadPin;

	 //configurable SPI clock setting
	 uint32_t SPIClock;

	 //MAX72XX settings
	 uint8_t DecodeMode;
	 uint8_t Intensity;
	 uint8_t ScanLimit;
	 bool ShutdownMode;
	 bool TestMode;

	 //current data
	 uint8_t MAXData[8];

	 //transmit method
	 void Transmit(uint8_t address, uint8_t data);
	 uint8_t DigitFromRow(uint8_t row) { return row + 1; }

 public:
	MAX72XXClass
	(
		int LOAD_CS_PIN, 
		uint32_t spi_clock = 16000000,
		uint8_t decode_mode = 0x00,
		uint8_t intensity = 0xFF,
		uint8_t scan_limit = 0x07,
		bool shutdown = false,
		bool test_mode = false
	) :
	LoadPin(LOAD_CS_PIN),
	SPIClock(spi_clock),
	DecodeMode(decode_mode),
	Intensity(intensity),
	ScanLimit(scan_limit),
	ShutdownMode(shutdown),
	TestMode(test_mode)
	{}

	//initialization method
	void init();

	//configuration setters
	void setDecodeMode(uint8_t decode_mode) { Transmit(MAX72XX_REG_DECODEMODE, decode_mode); DecodeMode = decode_mode; }
	void setIntensity(uint8_t intensity) { Transmit(MAX72XX_REG_INTENSITY, intensity); Intensity = intensity; }
	void setScanLimit(uint8_t scan_limit) { Transmit(MAX72XX_REG_SCANLIMIT, scan_limit); ScanLimit = scan_limit; }
	void setShutDownMode(bool shutdown) { Transmit(MAX72XX_REG_SHUTDOWN, shutdown ? MAX72XX_SDMODE_SHUTDOWN : MAX72XX_SDMODE_NORMAL); ShutdownMode = shutdown; }
	void setTestMode(bool test_mode) { Transmit(MAX72XX_REG_DISPLAYTEST, test_mode ? MAX72XX_TESTMODE_TEST : MAX72XX_TESTMODE_NORMAL); TestMode = test_mode; }

	//configuration getters
	uint8_t getDecodeMode(uint8_t decode_mode) { return DecodeMode; }
	uint8_t getIntensity(uint8_t intensity) { return Intensity; }
	uint8_t getScanLimit(uint8_t scan_limit) { return ScanLimit; }
	bool getShutDownMode(bool shutdown) { return ShutdownMode; }
	bool getTestMode(bool test_mode) { return TestMode; }

	//send data to display
	void sendData(uint8_t* data);
	bool sendRow(uint8_t row, uint8_t data);
	bool setPixel(uint8_t row, uint8_t col, bool value);
};



#endif

